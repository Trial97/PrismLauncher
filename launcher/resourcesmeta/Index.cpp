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
    m_pathIndexBuilt = false;
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

void Index::ensurePathIndex() const
{
    if (m_pathIndexBuilt) {
        return;
    }
    m_pathIndex.clear();
    m_pathIndex.reserve(size());
    for (qsizetype i = 0; i < size(); ++i) {
        m_pathIndex.insert(at(i).path, i);
    }
    m_pathIndexBuilt = true;
}

Entry* Index::findByPath(const QString& path)
{
    ensurePathIndex();
    auto it = m_pathIndex.constFind(path);
    if (it == m_pathIndex.constEnd()) {
        return nullptr;
    }
    return &(*this)[it.value()];
}

const Entry* Index::findByPath(const QString& path) const
{
    ensurePathIndex();
    auto it = m_pathIndex.constFind(path);
    if (it == m_pathIndex.constEnd()) {
        return nullptr;
    }
    return &at(it.value());
}

Entry* Index::findBySource(Platform provider, const QString& id)
{
    // Not path-keyed, and only ever called once per download (not on the per-resource scan hot
    // path that findByPath()/upsert() are), so a linear scan is fine here.
    auto it = std::ranges::find_if(*this, [provider, &id](const Entry& entry) {
        auto source = entry.providers.constFind(provider);
        return source != entry.providers.constEnd() && source->id == id;
    });
    return it == end() ? nullptr : &*it;
}

void Index::upsert(Entry entry)
{
    ensurePathIndex();
    auto it = m_pathIndex.constFind(entry.path);
    if (it != m_pathIndex.constEnd()) {
        (*this)[it.value()] = std::move(entry);
        return;
    }
    auto path = entry.path;
    append(std::move(entry));
    m_pathIndex.insert(path, size() - 1);
}

bool Index::removeByPath(const QString& path)
{
    ensurePathIndex();
    auto it = m_pathIndex.constFind(path);
    if (it == m_pathIndex.constEnd()) {
        return false;
    }
    removeAt(it.value());
    // Positions after the removed one all shifted down by one - rebuild lazily on next access
    // rather than eagerly reindexing here (removals are rare, not part of the scan hot path).
    m_pathIndexBuilt = false;
    return true;
}

}  // namespace Resources
