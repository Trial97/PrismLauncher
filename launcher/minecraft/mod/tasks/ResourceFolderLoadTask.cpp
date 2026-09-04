// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "ResourceFolderLoadTask.h"

#include "Application.h"
#include "FileSystem.h"
#include "minecraft/mod/ResourceIndexEntry.h"
#include "modplatform/packwiz/Packwiz.h"
#include "resourcesmeta/HashAlgorithm.h"

#include <QDateTime>
#include <QFile>
#include <QThread>
#include <utility>

ResourceFolderLoadTask::ResourceFolderLoadTask(const QDir& resourceDir,
                                               const QDir& indexDir,
                                               bool isIndexed,
                                               bool cleanOrphan,
                                               Resources::Type type,
                                               QString instanceRoot,
                                               std::function<Resource*(const QFileInfo&)> createFunction)
    : Task(false)
    , m_resource_dir(resourceDir)
    , m_index_dir(indexDir)
    , m_is_indexed(isIndexed)
    , m_clean_orphan(cleanOrphan)
    , m_type(type)
    , m_instance_root(std::move(instanceRoot))
    , m_create_func(std::move(createFunction))
    , m_result(new Result())
    , m_mainThread(thread())
{}

void ResourceFolderLoadTask::executeTask()
{
    if (thread() != m_mainThread) {
        connect(this, &Task::finished, this->thread(), &QThread::quit);
    }

    if (m_is_indexed) {
        // Migrate any remaining legacy packwiz metadata first
        migrateLegacyPackwizMetadata();
    }

    // Read JAR files that don't have metadata
    m_resource_dir.refresh();
    for (auto entry : m_resource_dir.entryInfoList()) {
        auto filePath = entry.absoluteFilePath();
        if (auto* app = APPLICATION_DYN; (app != nullptr) && app->checkQSavePath(filePath)) {
            continue;
        }
        auto newFilePath = FS::getUniqueResourceName(filePath);
        if (newFilePath != filePath) {
            FS::move(filePath, newFilePath);
            entry = QFileInfo(newFilePath);
        }

        Resource* resource = m_create_func(entry);

        if (resource->enabled()) {
            if (m_result->resources.contains(resource->internalId())) {
                m_result->resources[resource->internalId()]->setStatus(ResourceStatus::Installed);
                // Delete the object we just created, since a valid one is already in the mods list.
                delete resource;
            } else {
                m_result->resources[resource->internalId()].reset(resource);
                m_result->resources[resource->internalId()]->setStatus(ResourceStatus::NoMetadata);
            }
        } else {
            QString choppedId = resource->internalId().chopped(9);
            if (m_result->resources.contains(choppedId)) {
                m_result->resources[resource->internalId()].reset(resource);

                const auto& placeholderEntry = m_result->resources[choppedId]->entry();
                if (!placeholderEntry.providers.isEmpty()) {
                    resource->setEntry(placeholderEntry);

                    m_result->resources[resource->internalId()]->setStatus(ResourceStatus::Installed);
                    m_result->resources.remove(choppedId);
                }
            } else {
                m_result->resources[resource->internalId()].reset(resource);
                m_result->resources[resource->internalId()]->setStatus(ResourceStatus::NoMetadata);
            }
        }
    }

    // Remove orphan metadata to prevent issues
    // See https://github.com/PolyMC/PolyMC/issues/996
    if (m_clean_orphan) {
        QMutableMapIterator iter(m_result->resources);
        while (iter.hasNext()) {
            auto resource = iter.next().value();
            if (resource->status() == ResourceStatus::NotInstalled) {
                resource->destroy(false, false);
                iter.remove();
            }
        }
    }

    if (m_aborted) {
        emit finished();
    } else {
        emitSucceeded();
    }
}

void ResourceFolderLoadTask::migrateLegacyPackwizMetadata()
{
    m_index_dir.refresh();
    for (const auto& fileName : m_index_dir.entryList(QDir::Files)) {
        if (!fileName.endsWith(".pw.toml")) {
            continue;
        }

        auto metadata = Packwiz::V1::getIndexForMod(m_index_dir, fileName);

        if (!metadata.isValid()) {
            continue;
        }

        auto path = ResourceIndexEntry::canonicalRelativePath(QFileInfo(m_resource_dir.filePath(metadata.filename)), m_instance_root);

        Resources::Entry entry;
        entry.path = path;
        entry.type = m_type;
        entry.enabled = true;
        entry.updatedAt = QDateTime::currentDateTimeUtc();
        entry.info.name = metadata.name;

        auto source = ResourceIndexEntry::sourceFromPackwiz(metadata);
        entry.providers.insert(metadata.provider, source);
        entry.side = source.side;
        entry.info.loaders = source.loaders;

        if (!metadata.hash.isEmpty() && !metadata.hash_format.isEmpty()) {
            auto alg = Resources::HashAlgorithm::fromString(metadata.hash_format);
            if (alg.isValid()) {
                entry.hashes.insert(alg, metadata.hash);
            }
        }

        m_result->migratedEntries.append(entry);

        auto* resource = m_create_func(QFileInfo(m_resource_dir.filePath(metadata.filename)));
        resource->setEntry(entry);
        resource->setStatus(ResourceStatus::NotInstalled);
        m_result->resources[resource->internalId()].reset(resource);

        QFile::remove(m_index_dir.absoluteFilePath(fileName));
    }
}
