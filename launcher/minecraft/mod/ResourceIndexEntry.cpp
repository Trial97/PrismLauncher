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

#include "ResourceIndexEntry.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>

#include "Resource.h"

namespace ResourceIndexEntry {

QString canonicalRelativePath(const QFileInfo& fileInfo, const QString& instanceRootPath)
{
    auto path = fileInfo.absoluteFilePath();
    if (path.endsWith(".disabled")) {
        path.chop(9);
    }
    return QDir(instanceRootPath).relativeFilePath(path);
}

Resources::Entry build(const Resource& resource, Resources::Type type, const QString& instanceRootPath)
{
    // Carry forward whatever this resource already knows about its provider(s) - build() only
    // refreshes the locally-derived parts (path/type/enabled/info/hashes); providers are written
    // separately (by a download, or by the legacy-packwiz migration step).
    Resources::Entry entry = resource.entry();
    entry.path = canonicalRelativePath(resource.fileinfo(), instanceRootPath);
    entry.type = type;
    entry.enabled = resource.enabled();
    entry.info = resource.toIndexDetails();
    entry.hashes = resource.hashes();
    entry.updatedAt = QDateTime::currentDateTimeUtc();

    if (const auto* src = entry.primarySource()) {
        entry.side = src->side;
        entry.info.loaders = src->loaders;
    }

    return entry;
}

Resources::Source sourceFromPackwiz(const Packwiz::V1::Mod& mod)
{
    Resources::Source src;
    src.id = mod.project_id.toString();
    // CurseForge has no human version string - packwiz's version_number for CF mods is actually
    // just the downloaded file's display name (see Packwiz::V1::createModFormat's fallback), not
    // a real version - so prefer the file id there. Other providers keep version_number as-is.
    if (mod.provider == Resources::Platform::Curseforge) {
        src.version = mod.file_id.toString();
    } else {
        src.version = !mod.version_number.isEmpty() ? mod.version_number : mod.file_id.toString();
    }
    src.url = mod.url;
    src.side = mod.side;
    src.loaders = mod.loaders;
    src.mcVersions = mod.mcVersions;
    src.releaseType = mod.releaseType;
    src.dependencies = mod.dependencies;
    return src;
}

Resources::Source sourceFromDownload(const ModPlatform::IndexedPack& pack, const ModPlatform::IndexedVersion& version)
{
    Resources::Source src;
    src.id = pack.addonId.toString();
    if (!version.versionNumber.isEmpty()) {
        src.version = version.versionNumber;
    } else if (pack.provider == Resources::Platform::Curseforge) {
        // CurseForge has no human version string - version.version is just the file's display
        // name (e.g. "MyMod-1.2.jar"), not a real version - so use its file id instead, which is
        // the actual version equivalent.
        src.version = version.fileId.toString();
    } else if (!version.version.isEmpty()) {
        src.version = version.version;
    } else {
        src.version = version.fileId.toString();
    }
    src.url = version.downloadUrl;
    src.side = version.side.isValid() ? version.side : pack.side;
    src.loaders = version.loaders;
    src.mcVersions = version.mcVersion;
    src.releaseType = version.versionType;
    src.dependencies = version.dependencies;
    return src;
}

}  // namespace ResourceIndexEntry
