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
 */

#include "LocalResourceUpdateTask.h"

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

#include "minecraft/MinecraftInstance.h"
#include "minecraft/mod/ResourceIndexEntry.h"
#include "resourcesmeta/HashAlgorithm.h"

LocalResourceUpdateTask::LocalResourceUpdateTask(MinecraftInstance* instance,
                                                 QDir resource_dir,
                                                 Resources::Type type,
                                                 ModPlatform::IndexedPack project,
                                                 ModPlatform::IndexedVersion version)
    : m_instance(instance)
    , m_resource_dir(std::move(resource_dir))
    , m_type(type)
    , m_project(std::move(project))
    , m_version(std::move(version))
{}

void LocalResourceUpdateTask::executeTask()
{
    setStatus(tr("Updating index for resource:\n%1").arg(m_project.name));

    if (m_project.addonId.isNull() || m_version.fileName.isEmpty()) {
        qCritical() << "Tried to update the resource index with invalid data!";
        emitFailed(tr("Invalid metadata"));
        return;
    }

    auto path = ResourceIndexEntry::canonicalRelativePath(QFileInfo(m_resource_dir.filePath(m_version.fileName)), m_instance->instanceRoot());

    auto* index = m_instance->resourcesIndex();

    if (auto* old = index->findBySource(m_project.provider, m_project.addonId.toString()); old != nullptr && old->path != path) {
        emit hasOldResource(old->info.name, QFileInfo(old->path).fileName());
    }

    auto* existing = index->findByPath(path);
    Resources::Entry entry = existing != nullptr ? *existing : Resources::Entry{};
    entry.path = path;
    entry.type = m_type;
    if (existing == nullptr) {
        entry.enabled = true;
    }
    entry.updatedAt = QDateTime::currentDateTimeUtc();
    if (entry.info.name.isEmpty()) {
        entry.info.name = m_project.name;
    }

    auto source = ResourceIndexEntry::sourceFromDownload(m_project, m_version);
    entry.providers.insert(m_project.provider, source);
    entry.side = source.side;
    entry.info.loaders = source.loaders;

    if (!m_version.hash.isEmpty() && !m_version.hashType.isEmpty()) {
        auto alg = Resources::HashAlgorithm::fromString(m_version.hashType);
        if (alg.isValid()) {
            entry.hashes.insert(alg, m_version.hash);
        }
    }

    index->upsert(entry);
    m_instance->saveResourcesIndex();

    emitSucceeded();
}

auto LocalResourceUpdateTask::abort() -> bool
{
    emitAborted();
    return true;
}
