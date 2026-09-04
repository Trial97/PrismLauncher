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

#include "Details.h"

#include <QBuffer>

#include "Json.h"

namespace Resources {

QJsonObject Details::toJson() const
{
    QJsonObject obj;
    if (imagePath.isEmpty() && !image.isNull()) {
        QByteArray data;
        QBuffer buffer(&data);
        if (buffer.open(QIODevice::WriteOnly) && image.save(&buffer, "PNG")) {
            obj.insert(QStringLiteral("image"), QString::fromLatin1(data.toBase64()));
        }
    } else {
        obj.insert(QStringLiteral("image"), QString());
    }
    obj.insert(QStringLiteral("imagPath"), imagePath);
    obj.insert(QStringLiteral("description"), description);
    obj.insert(QStringLiteral("new_format_id"), newFormatId);
    obj.insert(QStringLiteral("details"), details);
    obj.insert(QStringLiteral("name"), name);
    obj.insert(QStringLiteral("version"), version);
    obj.insert(QStringLiteral("mcVersion"), mcVersion);
    obj.insert(QStringLiteral("homeurl"), homeUrl.toString());
    if (ModLoader::toList(loaders) != QList<ModLoaderValue>{}) {
        obj.insert(QStringLiteral("loaders"), Json::toJsonArray<QString>(ModLoader::toStringList(loaders)));
    }
    obj.insert(QStringLiteral("status"), status);
    obj.insert(QStringLiteral("provider"), provider);
    obj.insert(QStringLiteral("issueTracker"), issueTracker.toString());
    obj.insert(QStringLiteral("authors"), Json::toJsonArray<QString>(authors));
    obj.insert(QStringLiteral("licenses"), Json::toJsonArray<QString>(licenses));
    obj.insert(QStringLiteral("dependencies"), Json::toJsonArray<QString>(dependencies));
    return obj;
}

void Details::fromJson(const QJsonObject& obj)
{
    imagePath = obj.value(QStringLiteral("imagPath")).toString();
    if (imagePath.isEmpty()) {
        const auto imageData = QByteArray::fromBase64(obj.value(QStringLiteral("image")).toString().toLatin1());
        if (!imageData.isEmpty()) {
            image.loadFromData(imageData);
        }
    }
    description = obj.value(QStringLiteral("description")).toString();
    newFormatId = obj.value(QStringLiteral("new_format_id")).toString();
    details = obj.value(QStringLiteral("details")).toString();
    name = obj.value(QStringLiteral("name")).toString();
    version = obj.value(QStringLiteral("version")).toString();
    mcVersion = obj.value(QStringLiteral("mcVersion")).toString();
    homeUrl = QUrl(obj.value(QStringLiteral("homeurl")).toString());
    for (const auto& value : obj.value(QStringLiteral("authors")).toArray()) {
        authors.append(value.toString());
    }
    {
        QStringList loaderNames;
        for (const auto& value : obj.value(QStringLiteral("loaders")).toArray()) {
            loaderNames.append(value.toString());
        }
        loaders = ModLoader::fromList(loaderNames);
    }
    status = obj.value(QStringLiteral("status")).toString();
    provider = obj.value(QStringLiteral("provider")).toString();
    for (const auto& value : obj.value(QStringLiteral("licenses")).toArray()) {
        licenses.append(value.toString());
    }
    issueTracker = QUrl(obj.value(QStringLiteral("issueTracker")).toString());
    for (const auto& value : obj.value(QStringLiteral("dependencies")).toArray()) {
        dependencies.append(value.toString());
    }
}

}  // namespace Resources
