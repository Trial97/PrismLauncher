// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2024 Trial97 <alexandru.tripon97@gmail.com>
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

#include "ModDetails.h"
#include "Toml.h"
#include "modplatform/ModIndex.h"
#include "modplatform/helpers/HashUtils.h"

ModLicense::ModLicense(QString license)
{
    // FIXME: come up with a better license parsing.
    // handle SPDX identifiers? https://spdx.org/licenses/
    auto parts = license.split(' ');
    QStringList notNameParts = {};
    for (auto part : parts) {
        auto _url = QUrl(part);
        if (part.startsWith("(") && part.endsWith(")"))
            _url = QUrl(part.mid(1, part.size() - 2));

        if (_url.isValid() && !_url.scheme().isEmpty() && !_url.host().isEmpty()) {
            this->url = _url.toString();
            notNameParts.append(part);
            continue;
        }
    }

    for (auto part : notNameParts) {
        parts.removeOne(part);
    }

    auto licensePart = parts.join(' ');
    this->name = licensePart;
    this->description = licensePart;

    if (parts.size() == 1) {
        this->id = parts.first();
    }
}
ModLicense::ModLicense(const QString& name_, const QString& id_, const QString& url_, const QString& description_)
    : name(name_), id(id_), url(url_), description(description_)
{}
ModLicense::ModLicense(const ModLicense& other) : name(other.name), id(other.id), url(other.url), description(other.description) {}
ModLicense& ModLicense::operator=(const ModLicense& other)
{
    this->name = other.name;
    this->id = other.id;
    this->url = other.url;
    this->description = other.description;

    return *this;
}
ModLicense& ModLicense::operator=(const ModLicense&& other)
{
    this->name = other.name;
    this->id = other.id;
    this->url = other.url;
    this->description = other.description;

    return *this;
}
bool ModLicense::isEmpty()
{
    return this->name.isEmpty() && this->id.isEmpty() && this->url.isEmpty() && this->description.isEmpty();
}

ModLicense::ModLicense(toml::table table)
{
    name = Toml::getString(table, "name");
    id = Toml::getString(table, "id");
    url = Toml::getString(table, "url");
    description = Toml::getString(table, "description");
}

toml::table ModLicense::toToml()
{
    return toml::table{
        { "name", name.toStdString() },
        { "id", id.toStdString() },
        { "url", url.toStdString() },
        { "description", description.toStdString() },
    };
}

ModDetails::ModDetails(const ModDetails& other)
    : mod_id(other.mod_id)
    , name(other.name)
    , version(other.version)
    , mcversion(other.mcversion)
    , homeurl(other.homeurl)
    , description(other.description)
    , authors(other.authors)
    , issue_tracker(other.issue_tracker)
    , licenses(other.licenses)
    , icon_path(other.icon_path)
    , status(other.status)
    , new_format_id(other.new_format_id)
{}

ModDetails& ModDetails::operator=(const ModDetails& other)
{
    this->mod_id = other.mod_id;
    this->name = other.name;
    this->version = other.version;
    this->mcversion = other.mcversion;
    this->homeurl = other.homeurl;
    this->description = other.description;
    this->authors = other.authors;
    this->issue_tracker = other.issue_tracker;
    this->licenses = other.licenses;
    this->icon_path = other.icon_path;
    this->status = other.status;
    this->new_format_id = other.new_format_id;

    return *this;
}

ModDetails& ModDetails::operator=(const ModDetails&& other)
{
    this->mod_id = other.mod_id;
    this->name = other.name;
    this->version = other.version;
    this->mcversion = other.mcversion;
    this->homeurl = other.homeurl;
    this->description = other.description;
    this->authors = other.authors;
    this->issue_tracker = other.issue_tracker;
    this->licenses = other.licenses;
    this->icon_path = other.icon_path;
    this->status = other.status;
    this->new_format_id = other.new_format_id;

    return *this;
}

ModDetails::ModDetails(toml::table table)
{
    mod_id = Toml::getString(table, "mod_id");
    name = Toml::getString(table, "name");
    version = Toml::getString(table, "version");
    mcversion = Toml::getString(table, "mcversion");
    homeurl = Toml::getString(table, "homeurl");
    description = Toml::getString(table, "description");
    issue_tracker = Toml::getString(table, "issue_tracker");
    icon_path = Toml::getString(table, "icon_path");
    new_format_id = table["format_id"].value_or(0);

    authors = Toml::toStringList(table["authors"].as_array());
    if (auto licenseArr = table["licenses"].as_array()) {
        for (auto&& l : *licenseArr) {
            if (auto table = l.as_table()) {
                licenses.push_back({ *table });
            }
        }
    }
}

toml::table ModDetails::toToml()
{
    toml::array licenseArr;
    for (auto l : licenses) {
        if (!l.isEmpty()) {
            licenseArr.push_back(l.toToml());
        }
    }
    return toml::table{
        { "mod_id", mod_id.toStdString() },
        { "name", name.toStdString() },
        { "version", version.toStdString() },
        { "mcversion", mcversion.toStdString() },
        { "homeurl", homeurl.toStdString() },
        { "description", description.toStdString() },
        { "issue_tracker", issue_tracker.toStdString() },
        { "icon_path", icon_path.toStdString() },
        { "format_id", new_format_id },
        { "authors", Toml::fromStringList(authors) },
        { "licenses", licenseArr },
    };
}

ResourceHash::ResourceHash(toml::table table)
{
    alg = Hashing::algorithmFromString(Toml::getString(table, "alg"));
    hash = Toml::getString(table, "hash");
}

toml::table ResourceHash::toToml()
{
    return toml::table{
        { "alg", Hashing::algorithmToString(alg).toStdString() },
        { "hash", hash.toStdString() },
    };
}

ProviderInfo::ProviderInfo(toml::table table)
{
    name = ModPlatform::ProviderCapabilities::fromString(Toml::getString(table, "name"));
    id = Toml::getString(table, "id");
    version = Toml::getString(table, "version");
    url = Toml::getString(table, "url");
    side = stringToSide(Toml::getString(table, "side"));
    if (auto lds = table["loaders"]; lds && lds.is_array()) {
        for (auto&& loader : *lds.as_array()) {
            if (loader.is_string()) {
                loaders |= ModPlatform::getModLoaderFromString(QString::fromStdString(loader.as_string()->value_or("")));
            }
        }
    }
    mcVersions = Toml::toStringList(table.get_as<toml::array>("mcVersions"));
    releaseType = Toml::getString(table, "releaseType");
    if (auto deps = table.get_as<toml::array>("dependencies")) {
        for (auto&& d : *deps) {
            if (auto t = d.as_table()) {
                dependencies.push_back(ModPlatform::Dependency(*t));
            }
        }
    }
}

toml::table ProviderInfo::toToml()
{
    toml::array deps;
    for (auto dep : dependencies) {
        if (dep.type == ModPlatform::DependencyType::REQUIRED) {
            deps.push_back(dep.toToml());
        }
    }
    toml::array lds;
    for (auto loader : { ModPlatform::NeoForge, ModPlatform::Forge, ModPlatform::Cauldron, ModPlatform::LiteLoader, ModPlatform::Fabric,
                         ModPlatform::Quilt }) {
        if (loaders & loader) {
            lds.push_back(getModLoaderAsString(loader).toStdString());
        }
    }
    return toml::table{
        { "name", ModPlatform::ProviderCapabilities::name(name) },
        { "id", id.toStdString() },
        { "version", version.toStdString() },
        { "url", url.toStdString() },
        { "side", sideToString(side).toStdString() },
        { "loaders", lds },
        { "mcVersions", Toml::fromStringList(mcVersions) },
        { "releaseType", releaseType.toString().toStdString() },
        { "dependencies", deps },
    };
}
QString sideToString(Side side)
{
    switch (side) {
        case Side::ClientSide:
            return "client";
        case Side::ServerSide:
            return "server";
        case Side::UniversalSide:
            return "both";
    }
    return {};
}
Side stringToSide(QString side)
{
    if (side == "client")
        return Side::ClientSide;
    if (side == "server")
        return Side::ServerSide;
    if (side == "both")
        return Side::UniversalSide;
    return Side::UniversalSide;
}
