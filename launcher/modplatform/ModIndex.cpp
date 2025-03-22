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

#include "modplatform/ModIndex.h"

#include <QJsonObject>
#include <QVariant>

namespace ModPlatform {

static const QMap<QString, IndexedVersionType::VersionType> s_indexed_version_type_names = {
    { "release", IndexedVersionType::VersionType::Release },
    { "beta", IndexedVersionType::VersionType::Beta },
    { "alpha", IndexedVersionType::VersionType::Alpha }
};

static const QList<ModLoaderType> loaderList = { NeoForge, Forge, Cauldron, LiteLoader, Quilt, Fabric };

QList<ModLoaderType> modLoaderTypesToList(ModLoaderTypes flags)
{
    QList<ModLoaderType> flagList;
    for (auto flag : loaderList) {
        if (flags.testFlag(flag)) {
            flagList.append(flag);
        }
    }
    return flagList;
}

IndexedVersionType::IndexedVersionType(const QString& type) : IndexedVersionType(enumFromString(type)) {}

IndexedVersionType::IndexedVersionType(const IndexedVersionType::VersionType& type)
{
    m_type = type;
}

IndexedVersionType::IndexedVersionType(const IndexedVersionType& other)
{
    m_type = other.m_type;
}

IndexedVersionType& IndexedVersionType::operator=(const IndexedVersionType& other)
{
    m_type = other.m_type;
    return *this;
}

const QString IndexedVersionType::toString(const IndexedVersionType::VersionType& type)
{
    return s_indexed_version_type_names.key(type, "unknown");
}

IndexedVersionType::VersionType IndexedVersionType::enumFromString(const QString& type)
{
    return s_indexed_version_type_names.value(type, IndexedVersionType::VersionType::Unknown);
}

const char* ProviderCapabilities::name(ResourceProvider p)
{
    switch (p) {
        case ResourceProvider::MODRINTH:
            return "modrinth";
        case ResourceProvider::FLAME:
            return "curseforge";
    }
    return {};
}

QString ProviderCapabilities::readableName(ResourceProvider p)
{
    switch (p) {
        case ResourceProvider::MODRINTH:
            return "Modrinth";
        case ResourceProvider::FLAME:
            return "CurseForge";
    }
    return {};
}

QStringList ProviderCapabilities::hashType(ResourceProvider p)
{
    switch (p) {
        case ResourceProvider::MODRINTH:
            return { "sha512", "sha1" };
        case ResourceProvider::FLAME:
            // Try newer formats first, fall back to old format
            return { "sha1", "md5", "murmur2" };
    }
    return {};
}

ResourceProvider ProviderCapabilities::fromString(QString str)
{
    if (str == "modrinth")
        return ResourceProvider::MODRINTH;
    return ResourceProvider::FLAME;
}

QString getMetaURL(ResourceProvider provider, QVariant projectID)
{
    return ((provider == ModPlatform::ResourceProvider::FLAME) ? "https://www.curseforge.com/projects/" : "https://modrinth.com/mod/") +
           projectID.toString();
}

auto getModLoaderAsString(ModLoaderType type) -> const QString
{
    switch (type) {
        case NeoForge:
            return "neoforge";
        case Forge:
            return "forge";
        case Cauldron:
            return "cauldron";
        case LiteLoader:
            return "liteloader";
        case Fabric:
            return "fabric";
        case Quilt:
            return "quilt";
        default:
            break;
    }
    return "";
}

auto getModLoaderFromString(QString type) -> ModLoaderType
{
    if (type == "neoforge")
        return NeoForge;
    if (type == "forge")
        return Forge;
    if (type == "cauldron")
        return Cauldron;
    if (type == "liteloader")
        return LiteLoader;
    if (type == "fabric")
        return Fabric;
    if (type == "quilt")
        return Quilt;
    return {};
}

QString DependencyUtils::toString(DependencyType type)
{
    switch (type) {
        case DependencyType::REQUIRED:
            return "required";
        case DependencyType::OPTIONAL:
            return "optional";
        case DependencyType::INCOMPATIBLE:
            return "incompatible";
        case DependencyType::EMBEDDED:
            return "embedded";
        case DependencyType::TOOL:
            return "tool";
        case DependencyType::INCLUDE:
            return "include";
        case DependencyType::UNKNOWN:
        default:
            return "unknown";
    }
}

DependencyType DependencyUtils::fromString(const QString& str)
{
    if (str == "required")
        return DependencyType::REQUIRED;
    if (str == "optional")
        return DependencyType::OPTIONAL;
    if (str == "incompatible")
        return DependencyType::INCOMPATIBLE;
    if (str == "embedded")
        return DependencyType::EMBEDDED;
    if (str == "tool")
        return DependencyType::TOOL;
    if (str == "include")
        return DependencyType::INCLUDE;
    return DependencyType::UNKNOWN;
}

QJsonObject Dependency::toJson() const
{
    QJsonObject json;
    json["addonId"] = QJsonValue::fromVariant(addonId);
    json["type"] = DependencyUtils::toString(type);
    json["version"] = version;
    return json;
}

Dependency Dependency::fromJson(const QJsonObject& json)
{
    Dependency dep;
    dep.addonId = json["addonId"].toVariant();
    dep.type = DependencyUtils::fromString(json["type"].toString());
    dep.version = json["version"].toString();
    return dep;
}

QString SideUtils::toString(Side side)
{
    switch (side) {
        case Side::ClientSide:
            return "client";
        case Side::ServerSide:
            return "server";
        case Side::UniversalSide:
            return "both";
        default:
            return "unknown";
    }
}

Side SideUtils::fromString(const QString& str)
{
    if (str == "client")
        return Side::ClientSide;
    if (str == "server")
        return Side::ServerSide;
    if (str == "both")
        return Side::UniversalSide;
    return Side::UniversalSide;  // Default to universal if unknown
}
QString ResourceTypeUtils::toString(ResourceType type)
{
    switch (type) {
        case ResourceType::MOD:
            return "mod";
        case ResourceType::RESOURCE_PACK:
            return "resource_pack";
        case ResourceType::SHADER_PACK:
            return "shader_pack";
        case ResourceType::MODPACK:
            return "modpack";
        case ResourceType::DATAPACK:
            return "datapack";
        case ResourceType::WORLD:
            return "world";
        case ResourceType::SCREENSHOTS:
            return "screenshots";
        default:
            return "unknown";
    }
}
ResourceType ResourceTypeUtils::fromString(const QString& str)
{
    if (str == "mod")
        return ResourceType::MOD;
    if (str == "resource_pack")
        return ResourceType::RESOURCE_PACK;
    if (str == "shader_pack")
        return ResourceType::SHADER_PACK;
    if (str == "modpack")
        return ResourceType::MODPACK;
    if (str == "datapack")
        return ResourceType::DATAPACK;
    if (str == "world")
        return ResourceType::WORLD;
    if (str == "screenshots")
        return ResourceType::SCREENSHOTS;
    return ResourceType::UNKNOWN;
}
}  // namespace ModPlatform
