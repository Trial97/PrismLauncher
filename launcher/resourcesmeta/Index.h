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

#pragma once

#include <QHash>
#include <QList>
#include <QString>
#include <optional>

#include "Entry.h"

class QJsonArray;

namespace Resources {

// Matches the top-level array of the resource index schema.
//
// NOTE: findByPath()/upsert()/removeByPath() maintain a path -> position lookup cache for O(1)
// (amortized) access instead of a linear scan, which matters since a full folder scan calls
// findByPath()+upsert() once per resource. This cache is only kept consistent by those methods
// (and fromJson()/load()) - mutating the list directly via inherited QList methods (append,
// removeAt, clear, operator[]=, ...) will desync it. Stick to upsert()/removeByPath() to mutate.
struct Index : public QList<Entry> {
    using QList::QList;

    QJsonArray toJson() const;
    void fromJson(const QJsonArray& array);

    /** Loads an index from a JSON file on disk. Returns std::nullopt if the file is missing,
     *  unreadable, or its contents can't be parsed. */
    static std::optional<Index> load(const QString& filePath);

    /** Saves the index to a JSON file on disk, atomically. Returns whether it succeeded. */
    bool save(const QString& filePath) const;

    Entry* findByPath(const QString& path);
    const Entry* findByPath(const QString& path) const;

    /** Finds the entry (if any) whose Source for 'provider' has the given id - used to detect a
     *  previously-downloaded version of the same resource under a different file name. */
    Entry* findBySource(Platform provider, const QString& id);

    /** Replaces the entry with the same path, or appends it if none exists yet. */
    void upsert(Entry entry);

    /** Removes the entry with the given path, if any. Returns whether one was removed. */
    bool removeByPath(const QString& path);

   private:
    void ensurePathIndex() const;

    mutable QHash<QString, qsizetype> m_pathIndex;
    mutable bool m_pathIndexBuilt = false;
};

}  // namespace Resources
