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

#include <memory>

#include <QString>
#include <QStringList>
#include <QUrl>

#include <toml++/toml.hpp>

#include "minecraft/mod/MetadataHandler.h"
#include "modplatform/ModIndex.h"
#include "modplatform/helpers/HashUtils.h"

enum class ModStatus {
    Installed,     // Both JAR and Metadata are present
    NotInstalled,  // Only the Metadata is present
    NoMetadata,    // Only the JAR is present
    Unknown,       // Default status
};

struct ModLicense {
    QString name = {};
    QString id = {};
    QString url = {};
    QString description = {};

    ModLicense() = default;
    ModLicense(QString license);
    ModLicense(const QString& name_, const QString& id_, const QString& url_, const QString& description_);
    ModLicense(const ModLicense& other);
    ModLicense(toml::table table);

    ModLicense& operator=(const ModLicense& other);
    ModLicense& operator=(const ModLicense&& other);

    bool isEmpty();
    toml::table toToml();
};

struct ModDetails {
    /* Mod ID as defined in the ModLoader-specific metadata */
    QString mod_id = {};

    /* Human-readable name */
    QString name = {};

    /* Human-readable mod version */
    QString version = {};

    /* Human-readable minecraft version */
    QString mcversion = {};

    /* URL for mod's home page */
    QString homeurl = {};

    /* Human-readable description */
    QString description = {};

    /* List of the author's names */
    QStringList authors = {};

    /* Issue Tracker URL */
    QString issue_tracker = {};

    /* License */
    QList<ModLicense> licenses = {};

    /* Path of mod logo */
    QString icon_path = {};

    /* Installation status of the mod */
    ModStatus status = ModStatus::Unknown;

    /* Metadata information, if any */
    std::shared_ptr<Metadata::ModStruct> metadata = nullptr;

    int new_format_id;  // for resourcepacks

    ModDetails() = default;
    ModDetails(const ModDetails& other);
    ModDetails& operator=(const ModDetails& other);
    ModDetails& operator=(const ModDetails&& other);
    ModDetails(toml::table table);

    toml::table toToml();
};

struct ResourceHash {
    Hashing::Algorithm alg = Hashing::Algorithm::Sha1;
    QString hash = {};

    ResourceHash(toml::table table);
    toml::table toToml();
};

enum class Side { ClientSide = 1 << 0, ServerSide = 1 << 1, UniversalSide = ClientSide | ServerSide };
QString sideToString(Side side);
Side stringToSide(QString side);

struct ProviderInfo {
    ModPlatform::ResourceProvider name;
    QString id = {};
    QString version = {};
    QString url = {};
    Side side = Side::UniversalSide;
    ModPlatform::ModLoaderTypes loaders;
    QStringList mcVersions = {};
    ModPlatform::IndexedVersionType releaseType;
    QList<ModPlatform::Dependency> dependencies = {};

    ProviderInfo(toml::table table);
    toml::table toToml();
};

enum class ResourceInfoType { Mod, Coremod, Resourcepack, Shaderpack, Datapack, Worlds, Screenshots, Extra };
struct ResourceInfo {
    QString path;
    ResourceInfoType type;
    ResourceInfoType fileType;
    bool enabled;
    bool managedByPack;
    Side side;
    bool lockVersion = true;
    QStringList categories;
    ModDetails info;
    QList<ResourceHash> hashes;
    QList<ProviderInfo> providers;
};