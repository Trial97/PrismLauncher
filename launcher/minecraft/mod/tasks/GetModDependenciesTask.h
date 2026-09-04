// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2023 Trial97 <alexandru.tripon97@gmail.com>
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
#include <QList>
#include <QVariant>
#include <memory>
#include <utility>

#include "minecraft/mod/MetadataHandler.h"
#include "minecraft/mod/ModFolderModel.h"
#include "modplatform/ModIndex.h"
#include "modplatform/ResourceAPI.h"
#include "modplatform/flame/FlameAPI.h"
#include "modplatform/modrinth/ModrinthAPI.h"
#include "tasks/SequentialTask.h"
#include "tasks/Task.h"
#include "ui/pages/modplatform/ModModel.h"

class GetModDependenciesTask : public SequentialTask {
    Q_OBJECT
   public:
    using Ptr = shared_qobject_ptr<GetModDependenciesTask>;

    struct PackDependency {
        Resources::Dependency dependency;
        ModPlatform::IndexedPack::Ptr pack;
        ModPlatform::IndexedVersion version;
        PackDependency() = default;
        PackDependency(ModPlatform::IndexedPack::Ptr p, ModPlatform::IndexedVersion v) : pack(std::move(p)), version(std::move(v)) {}
    };

    struct PackDependencyExtraInfo {
        bool maybeInstalled{};
        QStringList requiredByNames;
        QStringList requiredByIds;
    };

    explicit GetModDependenciesTask(MinecraftInstance* instance, ModFolderModel* folder, QList<std::shared_ptr<PackDependency>> selected);

    auto getDependecies() const -> QList<std::shared_ptr<PackDependency>> { return m_packDependencies; }
    QHash<QString, PackDependencyExtraInfo> getExtraInfo();

   private:
    static const ResourceAPI* getAPI(Resources::Platform provider)
    {
        if (provider == Resources::Platform::Curseforge) {
            return &FlameAPI::get();
        }
        return &ModrinthAPI::get();
    }

   protected slots:
    Task::Ptr prepareDependencyTask(const Resources::Dependency&, Resources::Platform, int);
    QList<Resources::Dependency> getDependenciesForVersion(const ModPlatform::IndexedVersion&, Resources::Platform providerName);
    void prepare();
    Task::Ptr getProjectInfoTask(const std::shared_ptr<PackDependency>& pDep);
    Resources::Dependency getOverride(const Resources::Dependency&, Resources::Platform providerName);
    void removePack(const QVariant& addonId);

    bool isLocalyInstalled(const std::shared_ptr<PackDependency>& pDep);
    bool maybeInstalled(const std::shared_ptr<PackDependency>& pDep);

   private:
    QList<std::shared_ptr<PackDependency>> m_packDependencies;
    QList<std::shared_ptr<Metadata::ModStruct>> m_mods;
    QList<std::shared_ptr<PackDependency>> m_selected;
    QStringList m_modsFileNames;

    Version m_version;
    Resources::ModLoaders m_loaderType;
};
