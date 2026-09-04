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
#include "modplatform/packwiz/Packwiz.h"
#include "resourcesmeta/HashAlgorithm.h"

namespace {

Resources::Source fromPackwiz(const Packwiz::V1::Mod& mod)
{
    Resources::Source src;
    src.id = mod.project_id.toString();
    src.version = mod.version_number;
    src.url = mod.url;
    src.side = mod.side;
    src.loaders = mod.loaders;
    src.mcVersions = mod.mcVersions;
    src.releaseType = mod.releaseType;
    src.dependencies = mod.dependencies;
    return src;
}

}  // namespace

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
    Resources::Entry entry;
    entry.path = canonicalRelativePath(resource.fileinfo(), instanceRootPath);
    entry.type = type;
    entry.enabled = resource.enabled();
    entry.info = resource.toIndexDetails();
    entry.hashes = resource.hashes();
    entry.updatedAt = QDateTime::currentDateTimeUtc();

    if (auto metadata = resource.metadata()) {
        entry.side = metadata->side;
        entry.info.loaders = metadata->loaders;
        entry.providers.insert(metadata->provider, fromPackwiz(*metadata));

        if (!metadata->hash.isEmpty() && !metadata->hash_format.isEmpty()) {
            auto alg = Resources::HashAlgorithm::fromString(metadata->hash_format);
            if (alg.isValid()) {
                entry.hashes.insert(alg, metadata->hash);
            }
        }
    }

    return entry;
}

}  // namespace ResourceIndexEntry
