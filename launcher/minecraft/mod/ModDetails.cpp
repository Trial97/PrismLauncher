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
#include <string>
#include <toml++/impl/table.hpp>

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

QString getTomlString(toml::table table, QString key, QString def = {})
{
    return QString::fromStdString(table[key.toStdString()].value_or(def.toStdString()));
}

QStringList getTomlStringList(toml::array* arr)
{
    QStringList list;
    if (arr) {
        for (auto&& v : *arr) {
            list.push_back(QString::fromStdString(v.value_or("")));
        }
    }
    return list;
}

toml::array stringListToToml(QStringList list)
{
    toml::array arr;
    for (auto v : list) {
        arr.push_back(v.toStdString());
    }
    return arr;
}

ModLicense::ModLicense(toml::table table)
{
    name = getTomlString(table, "name");
    id = getTomlString(table, "id");
    url = getTomlString(table, "url");
    description = getTomlString(table, "description");
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
    mod_id = getTomlString(table, "mod_id");
    name = getTomlString(table, "name");
    version = getTomlString(table, "version");
    mcversion = getTomlString(table, "mcversion");
    homeurl = getTomlString(table, "homeurl");
    description = getTomlString(table, "description");
    issue_tracker = getTomlString(table, "issue_tracker");
    icon_path = getTomlString(table, "icon_path");
    new_format_id = table["format_id"].value_or(0);

    authors = getTomlStringList(table["authors"].as_array());
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
        { "authors", stringListToToml(authors) },
        { "licenses", licenseArr },
    };
}
