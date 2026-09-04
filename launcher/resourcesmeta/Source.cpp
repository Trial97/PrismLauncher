// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2026 Trial97 <alexandru.tripon97@gmail.com>
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

#include "Source.h"

#include "Json.h"

namespace Resources {

QJsonObject Source::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("id"), id);
    obj.insert(QStringLiteral("version"), version);
    if (!url.isEmpty()) {
        obj.insert(QStringLiteral("url"), url.toString());
    }
    if (side.isValid()) {
        obj.insert(QStringLiteral("side"), side.toString());
    }
    if (loaders) {
        obj.insert(QStringLiteral("loaders"), Json::toJsonArray<QString>(loaders.toStringList()));
    }
    if (!mcVersions.isEmpty()) {
        obj.insert(QStringLiteral("mcVersions"), Json::toJsonArray<QString>(mcVersions));
    }
    if (releaseType.isValid()) {
        obj.insert(QStringLiteral("releaseType"), releaseType.toString());
    }
    if (!dependencies.isEmpty()) {
        QJsonArray array;
        for (const auto& dependency : dependencies) {
            array.append(dependency.toJson());
        }
        obj.insert(QStringLiteral("dependencies"), array);
    }
    return obj;
}

void Source::fromJson(const QJsonObject& obj)
{
    id = obj.value(QStringLiteral("id")).toString();
    version = obj.value(QStringLiteral("version")).toString();
    url = QUrl(obj.value(QStringLiteral("url")).toString());
    side = Side::fromString(obj.value(QStringLiteral("side")).toString());
    {
        QStringList loaderNames;
        for (const auto& value : obj.value(QStringLiteral("loaders")).toArray()) {
            loaderNames.append(value.toString());
        }
        loaders = ModLoaders::fromList(loaderNames);
    }
    for (const auto& value : obj.value(QStringLiteral("mcVersions")).toArray()) {
        mcVersions.append(value.toString());
    }
    releaseType = ReleaseType::fromString(obj.value(QStringLiteral("releaseType")).toString());
    for (const auto& value : obj.value(QStringLiteral("dependencies")).toArray()) {
        Dependency dependency;
        dependency.fromJson(value.toObject());
        dependencies.append(dependency);
    }
}

}  // namespace Resources
