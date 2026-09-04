// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
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

#include <QList>
#include <QMetaType>
#include <QString>
#include <QVariant>
#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include "EnumWrapper.h"
#include "modplatform/ResourceType.h"
#include "resourcesmeta/Dependency.h"
#include "resourcesmeta/ModLoader.h"
#include "resourcesmeta/Platform.h"
#include "resourcesmeta/ReleaseType.h"
#include "resourcesmeta/Side.h"

class QIODevice;

namespace ModPlatform {

struct ModpackAuthor {
    QString name;
    QString url;
};

struct DonationData {
    QString id;
    QString platform;
    QString url;
};

struct IndexedVersion {
    QVariant addonId;
    QVariant fileId;
    QString version;
    QString versionNumber;
    Resources::ReleaseType versionType;
    QStringList mcVersion;
    QString downloadUrl;
    QString date;
    QString fileName;
    Resources::ModLoaders loaders;
    QString hashType;
    QString hash;
    bool isPreferred = true;
    QString changelog;
    QList<Resources::Dependency> dependencies;
    Resources::Side side = Resources::Side::Unknown;  // this is for flame API

    // For internal use, not provided by APIs
    bool isCurrentlySelected = false;

    QString getVersionDisplayString() const
    {
        auto releaseType = versionType.isValid() ? QString(" [%1]").arg(versionType.toString()) : "";
        auto versionStr = !version.contains(versionNumber) ? versionNumber : "";
        QString gameVersion = "";
        for (const auto& v : mcVersion) {
            if (version.contains(v)) {
                gameVersion = "";
                break;
            }
            if (gameVersion.isEmpty()) {
                gameVersion = QObject::tr(" for %1").arg(v);
            }
        }
        return QString("%1%2 — %3%4").arg(version, gameVersion, versionStr, releaseType);
    }
};

struct ExtraPackData {
    QList<DonationData> donate;

    QString issuesUrl;
    QString sourceUrl;
    QString wikiUrl;
    QString discordUrl;

    QString status;

    QString body;
};

struct IndexedPack {
    using Ptr = std::shared_ptr<IndexedPack>;

    QVariant addonId;
    Resources::Platform provider;
    QString name;
    QString slug;
    QString description;
    QList<ModpackAuthor> authors;
    QString logoName;
    QString logoUrl;
    QString websiteUrl;
    Resources::Side side = Resources::Side::Unknown;

    bool versionsLoaded = false;
    QList<IndexedVersion> versions;

    // Don't load by default, since some modplatform don't have that info
    bool extraDataLoaded = true;
    ExtraPackData extraData;

    ResourceType resourceType = ResourceType::Unknown;

    // For internal use, not provided by APIs
    bool isVersionSelected(int index) const
    {
        if (!versionsLoaded) {
            return false;
        }

        return versions.at(index).isCurrentlySelected;
    }
    bool isAnyVersionSelected() const
    {
        if (!versionsLoaded) {
            return false;
        }

        return std::any_of(versions.constBegin(), versions.constEnd(), [](const auto& v) { return v.isCurrentlySelected; });
    }
};

struct OverrideDep {
    QString quilt;
    QString fabric;
    QString slug;
    Resources::Platform provider;
};

inline auto getOverrideDeps() -> QList<OverrideDep>
{
    return {
        { .quilt = "634179", .fabric = "306612", .slug = "API", .provider = Resources::Platform::Curseforge },
        { .quilt = "720410", .fabric = "308769", .slug = "KotlinLibraries", .provider = Resources::Platform::Curseforge },

        { .quilt = "qvIfYCYJ", .fabric = "P7dR8mSH", .slug = "API", .provider = Resources::Platform::Modrinth },
        { .quilt = "lwVhp9o5", .fabric = "Ha28R6CL", .slug = "KotlinLibraries", .provider = Resources::Platform::Modrinth }
    };
}

QString getMetaURL(Resources::Platform provider, QVariant projectID);

struct Category {
    QString name;
    QString id;
};

}  // namespace ModPlatform

Q_DECLARE_METATYPE(ModPlatform::IndexedPack)
Q_DECLARE_METATYPE(ModPlatform::IndexedPack::Ptr)
