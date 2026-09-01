// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2026 Trial97 <alexandru.tripon97@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QString>

#include <functional>
#include <vector>

#include "Version.h"

/** Parses and evaluates a version range specification.
 *
 * The class itself is syntax-agnostic: it holds the OR'd compatibility rules
 * parsed from a spec and answers whether a given version satisfies any of them.
 * Build one with either factory, depending on the syntax being parsed.
 */
class VersionRange {
   public:
    VersionRange() = default;
    using IsVersionCompatible = std::function<bool(Version)>;

    /// Parses a Maven-style range spec and returns a range matching it.
    ///
    /// Maven Version Range
    ///  (https://maven.apache.org/pom.html#Dependency_Version_Requirement_Specification):
    ///  - `1.0`                    soft requirement, treated as `>= 1.0`
    ///  - `[1.0]`                  exactly 1.0
    ///  - `[1.0,2.0)`              1.0 <= x < 2.0
    ///  - `(1.0,2.0]`              1.0 < x <= 2.0
    ///  - `[1.5,)`                 x >= 1.5
    ///  - `(,1.0]`                 x <= 1.0
    ///  - `(,1.0],[1.2,)`          x <= 1.0 OR x >= 1.2 (comma-separated sets)
    static VersionRange fromMaven(QString spec);

    /// Parses a Fabric / Quilt style semver spec and returns a range matching it.
    ///
    /// A single atomic declaration (no space-separated or `||`-separated parts):
    ///  - `*`                      any version
    ///  - `1.2.3`                  exact version match
    ///  - `=1.2.3`, `>=1.2.3`, `>1.2.3`, `<=1.2.3`, `<1.2.3`
    ///  - `^1.2.3`                 >= 1.2.3 within the same major (< 2.0.0)
    ///  - `~1.2.3`                 >= 1.2.3 within the same minor (< 1.3.0)
    ///  - `1.x`, `1.2.x`, `1.*`    any version with the given major/minor prefix
    static VersionRange fromSemver(QString spec);

    /// Whether the given version falls within the range.
    bool contains(const Version& version) const;

    bool isValid() const { return m_isValid; }
    QString toString() const { return m_spec; }

   private:
    VersionRange(QString spec, std::vector<IsVersionCompatible> rules);

    QString m_spec = {};
    bool m_isValid = false;
    // The OR'd compatibility rules for this range (empty when invalid).
    std::vector<IsVersionCompatible> m_rules;
};
