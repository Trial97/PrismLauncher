// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2026 Trial97 <alexandru.tripon97@gmail.com>
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
#include <QStringList>
#include <QUrl>

#include "ModLoader.h"
#include "Dependency.h"
#include "ReleaseType.h"
#include "Side.h"

namespace Resources {

// Matches the "provider" definition of the resource index schema.
struct Source {
    QString id;
    // The provider's version identity: a human-readable version string for most providers, or
    // the provider's opaque file/version id when it has no such string (e.g. CurseForge, whose
    // file id doubles as its version identifier).
    QString version;
    QUrl url;
    Side side{ Side::Unknown };
    ModLoaders loaders;
    QStringList mcVersions;
    ReleaseType releaseType;
    QList<Dependency> dependencies;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);
};

}  // namespace Resources
