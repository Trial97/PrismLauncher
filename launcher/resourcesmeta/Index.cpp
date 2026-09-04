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

#include "Index.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <algorithm>

#include "FileSystem.h"

namespace Resources {

QJsonArray Index::toJson() const
{
    QJsonArray array;
    for (const auto& resource : *this) {
        array.append(resource.toJson());
    }
    return array;
}

void Index::fromJson(const QJsonArray& array)
{
    clear();
    for (const auto& value : array) {
        Entry item;
        item.fromJson(value.toObject());
        append(item);
    }
}

std::optional<Index> Index::load(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return std::nullopt;
    }

    QJsonParseError parseError{};
    auto doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return std::nullopt;
    }

    Index index;
    index.fromJson(doc.object().value("resources").toArray());
    return index;
}

bool Index::save(const QString& filePath) const
{
    if (!FS::ensureFolderPathExists(QFileInfo(filePath).absolutePath())) {
        return false;
    }

    QJsonObject root;
    root["formatVersion"] = 1;
    root["resources"] = toJson();

    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
    return file.commit();
}

Entry* Index::findByPath(const QString& path)
{
    auto it = std::ranges::find_if(*this, [&path](const Entry& entry) { return entry.path == path; });
    return it == end() ? nullptr : &*it;
}

const Entry* Index::findByPath(const QString& path) const
{
    auto it = std::ranges::find_if(*this, [&path](const Entry& entry) { return entry.path == path; });
    return it == end() ? nullptr : &*it;
}

void Index::upsert(Entry entry)
{
    if (auto* existing = findByPath(entry.path)) {
        *existing = std::move(entry);
    } else {
        append(std::move(entry));
    }
}

bool Index::removeByPath(const QString& path)
{
    return removeIf([&path](const Entry& entry) { return entry.path == path; }) > 0;
}

}  // namespace Resources
