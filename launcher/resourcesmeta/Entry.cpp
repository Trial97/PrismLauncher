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

#include "Entry.h"

#include "Json.h"

namespace Resources {

QJsonObject Entry::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("path"), path);
    obj.insert(QStringLiteral("type"), type.toString());
    obj.insert(QStringLiteral("enabled"), enabled);
    if (side.isValid()) {
        obj.insert(QStringLiteral("side"), side.toString());
    }
    obj.insert(QStringLiteral("categories"), Json::toJsonArray<QString>(categories));
    obj.insert(QStringLiteral("info"), info.toJson());
    obj.insert(QStringLiteral("hashes"), hashes.toJson());
    obj.insert(QStringLiteral("providers"), providers.toJson());
    obj.insert(QStringLiteral("updatedAt"), Json::toJson<QDateTime>(updatedAt));
    return obj;
}

void Entry::fromJson(const QJsonObject& obj)
{
    path = obj.value(QStringLiteral("path")).toString();
    type = Type::fromString(obj.value(QStringLiteral("type")).toString());
    enabled = obj.value(QStringLiteral("enabled")).toBool();
    side = Side::fromString(obj.value(QStringLiteral("side")).toString());
    for (const auto& value : obj.value(QStringLiteral("categories")).toArray()) {
        categories.append(value.toString());
    }
    info.fromJson(obj.value(QStringLiteral("info")).toObject());
    hashes.fromJson(obj.value(QStringLiteral("hashes")).toObject());
    providers.fromJson(obj.value(QStringLiteral("providers")).toObject());
    updatedAt = QDateTime::fromString(obj.value(QStringLiteral("updatedAt")).toString(), Qt::ISODate);
}

}  // namespace Resources
