// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
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

#include <QDir>

#include "modplatform/ModIndex.h"
#include "resourcesmeta/Type.h"
#include "tasks/Task.h"

class MinecraftInstance;

/** Records a downloaded (or about-to-be-downloaded) resource's provider identity directly into
 *  the instance's resources.index.json - no packwiz/.pw.toml file is written. */
class LocalResourceUpdateTask : public Task {
    Q_OBJECT
   public:
    using Ptr = shared_qobject_ptr<LocalResourceUpdateTask>;

    LocalResourceUpdateTask(MinecraftInstance* instance,
                            QDir resource_dir,
                            Resources::Type type,
                            ModPlatform::IndexedPack project,
                            ModPlatform::IndexedVersion version);

    auto canAbort() const -> bool override { return true; }
    auto abort() -> bool override;

   protected slots:
    //! Entry point for tasks.
    void executeTask() override;

   signals:
    void hasOldResource(QString name, QString filename);

   private:
    MinecraftInstance* m_instance;
    QDir m_resource_dir;
    Resources::Type m_type;
    ModPlatform::IndexedPack m_project;
    ModPlatform::IndexedVersion m_version;
};
