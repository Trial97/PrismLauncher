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

#include "resourcesmeta/Entry.h"
#include "resourcesmeta/Type.h"

class Resource;
class QFileInfo;

namespace ResourceIndexEntry {

/** Computes the instance-root-relative path for a resource file, stripping any trailing
 *  ".disabled" suffix so the path stays stable across enable/disable toggles. */
QString canonicalRelativePath(const QFileInfo& fileInfo, const QString& instanceRootPath);

/** Builds a Resources::Entry describing 'resource', migrating its packwiz metadata (if any)
 *  into the entry's Sources/hashes. */
Resources::Entry build(const Resource& resource, Resources::Type type, const QString& instanceRootPath);

}  // namespace ResourceIndexEntry
