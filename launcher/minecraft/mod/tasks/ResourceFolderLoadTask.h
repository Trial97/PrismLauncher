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

#pragma once

#include <QDir>
#include <QMap>
#include <QObject>
#include <QRunnable>
#include <memory>
#include "minecraft/mod/Resource.h"
#include "resourcesmeta/Entry.h"
#include "resourcesmeta/Type.h"
#include "tasks/Task.h"

class ResourceFolderLoadTask : public Task {
    Q_OBJECT
   public:
    struct Result {
        QMap<QString, Resource::Ptr> resources;
        // Resources::Entry values migrated from legacy packwiz (.pw.toml) metadata found during
        // this scan, to be upserted into the instance's resource index by the caller (this task
        // itself must not touch the shared index, since it runs off the main thread).
        QList<Resources::Entry> migratedEntries;
    };
    using ResultPtr = std::shared_ptr<Result>;
    ResultPtr result() const { return m_result; }

   public:
    ResourceFolderLoadTask(const QDir& resourceDir,
                           const QDir& indexDir,
                           bool isIndexed,
                           bool cleanOrphan,
                           Resources::Type type,
                           QString instanceRoot,
                           std::function<Resource*(const QFileInfo&)> createFunction);

    bool canAbort() const override { return true; }
    bool abort() override
    {
        m_aborted.store(true);
        return true;
    }

    void executeTask() override;

   private:
    /** Converts any remaining legacy packwiz (.pw.toml) metadata files into Resources::Entry
     *  values (see Result::migratedEntries) and deletes them - a one-time, self-terminating
     *  migration: once every legacy file has been converted, this becomes a no-op. */
    void migrateLegacyPackwizMetadata();

   private:
    QDir m_resource_dir, m_index_dir;
    bool m_is_indexed;
    bool m_clean_orphan;
    Resources::Type m_type;
    QString m_instance_root;
    std::function<Resource*(const QFileInfo&)> m_create_func;
    ResultPtr m_result;

    std::atomic<bool> m_aborted = false;

    /** This is the thread in which we should put new mod objects */
    QThread* m_mainThread;
};
