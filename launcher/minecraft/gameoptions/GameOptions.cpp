// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2023 Tayou <tayou@gmx.net>
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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include <QDebug>
#include <QSaveFile>

#include "GameOptions.h"
#include "FileSystem.h"

static Qt::CheckState boolToState(bool b)
{
    return b ? Qt::Checked : Qt::Unchecked;
};

namespace {
bool load(const QString& path,
          std::vector<GameOptionItem>& contents,
          int& version,
          QMap<QString, std::shared_ptr<GameOption>>* knownOptions)
{
    contents.clear();
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to read options file.";
        return false;
    }

    knownOptions = GameOptionsSchema::getKnownOptions();

    version = 0;
    while (!file.atEnd()) {
        // This should be handled by toml++ or some other toml parser rather than this manual parsing
        QString line = QString::fromUtf8(file.readLine());
        if (line.endsWith('\n')) {
            line.chop(1);
        }
        if (line.endsWith('\r')) {
            line.chop(1);
        }
        GameOptionItem item = GameOptionItem();

        auto parts = line.split(':');

        item.key = parts[0];
        item.value = parts[1];
        item.type = OptionType::String;
        qDebug() << "Reading Game Options Key:" << item.key;

        if (item.key == "version") {
            version = item.value.toInt();
            continue;
        };

        bool isInt = false;
        bool isFloat = false;
        item.intValue = item.value.toInt(&isInt);
        item.floatValue = item.value.toFloat(&isFloat);
        if (isInt) {
            item.type = OptionType::Int;
            // qDebug() << "The value" << value << "is a int";
        } else if (isFloat) {
            item.type = OptionType::Float;
            // qDebug() << "The value" << value << "is a float";
        } else if (item.value == "true" || item.value == "false") {
            item.boolValue = item.value == "true" ? true : false;
            item.type = OptionType::Bool;
            qDebug() << "The value" << item.value << "is a bool";
        } else if (item.value.endsWith("]") && item.value.startsWith("[")) {
            for (QString part : item.value.mid(1, item.value.size() - 2).split(", ")) {
                item.children.append(part);
            }
        }

        item.knownOption = knownOptions->find(item.key) != knownOptions->end() ? knownOptions->find(item.key).value() : nullptr;
        contents.emplace_back(item);
    }
    qDebug() << "Loaded" << path << "with version:" << version;
    return true;
}
bool save(const QString& path, std::vector<GameOptionItem>& mapping, int version)
{
    QSaveFile out(path);
    if (!out.open(QIODevice::WriteOnly)) {
        return false;
    }
    if (version != 0) {
        QString versionLine = QString("version:%1\n").arg(version);
        out.write(versionLine.toUtf8());
    }
    auto iter = mapping.begin();
    while (iter != mapping.end()) {
        out.write(iter->key.toUtf8());
        out.write(":");
        out.write(iter->value.toUtf8());
        out.write("\n");
        iter++;
    }
    return out.commit();
}
}  // namespace

GameOptions::GameOptions(const QString& path) : path(path)
{
    reload();
}

QVariant GameOptions::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QAbstractListModel::headerData(section, orientation, role);
    }
    switch (section) {
        case 0:
            return tr("Key");
        case 1:
            return tr("Description");
        case 2:
            return tr("Value");
        case 3:
            return tr("Default Value");
        default:
            return QVariant();
    }
}
bool GameOptions::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto row = index.row();
    auto column = (Column)index.column();
    if (column == Column::Value) {
        switch (contents[row].type) {
            case OptionType::String: {
                contents[row].value = value.toString();
            }
            case OptionType::Int: {
                contents[row].intValue = value.toInt();
            }
            case OptionType::Bool: {
                contents[row].boolValue = value.toBool();
            }
            case OptionType::Float: {
                contents[row].floatValue = value.toFloat();
            }
        }
    }

    return true;
}

Qt::ItemFlags GameOptions::flags(const QModelIndex& index) const
{
    Column column = (Column)index.column();

    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    if (column == Column::Key) {
        return flags;
    }

    flags = flags | Qt::ItemFlag::ItemIsEditable;
    if (column == Column::Value || column == Column::DefaultValue) {
        flags = flags | Qt::ItemFlag::ItemIsUserCheckable;
    }
    if (column == Column::DefaultValue) {
        flags = flags & ~Qt::ItemFlag::ItemIsEnabled;
    }

    return flags;
}

QVariant GameOptions::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    Column column = (Column)index.column();

    if (row < 0 || row >= int(contents.size()))
        return QVariant();

    switch (role) {
        case Qt::DisplayRole: {
            switch (column) {
                case Column::Key: {
                    return contents[row].key;
                }
                case Column::Description: {
                    if (contents[row].knownOption != nullptr) {
                        return contents[row].knownOption->description;
                    } else {
                        return "";
                    }
                }
                case Column::Value: {
                    switch (contents[row].type) {
                        case OptionType::String: {
                            return contents[row].value;
                        }
                        case OptionType::Int: {
                            return contents[row].intValue;
                        }
                        case OptionType::Bool: {
                            return contents[row].boolValue;
                        }
                        case OptionType::Float: {
                            return contents[row].floatValue;
                        }
                        case OptionType::KeyBind: {
                            return contents[row].value;
                        }
                    }
                }
                case Column::DefaultValue: {
                    if (contents[row].knownOption != nullptr) {
                        switch (contents[row].type) {
                            case OptionType::String: {
                                return contents[row].knownOption->defaultString;
                            }
                            case OptionType::Int: {
                                return contents[row].knownOption->defaultValue.intValue;
                            }
                            case OptionType::Bool: {
                                return contents[row].knownOption->defaultValue.boolValue;
                            }
                            case OptionType::Float: {
                                return contents[row].knownOption->defaultValue.floatValue;
                            }
                            case OptionType::KeyBind: {
                                return contents[row].knownOption->defaultString;
                            }
                        }
                    } else {
                        return "";
                    }
                }
            }
        }
        case Qt::CheckStateRole: {
            switch (column) {
                case Column::Value:
                    if (contents[row].type == OptionType::Bool) {
                        return boolToState(contents[row].boolValue);
                    }
                case Column::DefaultValue:
                    if (contents[row].knownOption != nullptr && contents[row].type == OptionType::Bool) {
                        return contents[row].knownOption->defaultValue.boolValue *2; // checkboxes are tristate, this 1(true) needs to be 2 for fully checked
                    }
            }
        }
        default: {
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex GameOptions::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (parent.isValid()) {
        // return child index
        GameOptionItem* item = static_cast<GameOptionItem*>(parent.internalPointer());

        auto child = item->children[row];
        return createIndex(row, column, (void*)&child);
    } else {
        //return root index
        return createIndex(row, column);
    }
    return QModelIndex();
}

int GameOptions::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return contents.size();
    }
    if (parent.column() > 0)
        return 0;
    return contents[parent.row()].children.count();
}

int GameOptions::columnCount(const QModelIndex&) const
{
    return 4;
}

bool GameOptions::isLoaded() const
{
    return loaded;
}

bool GameOptions::reload()
{
    beginResetModel();
    loaded = load(path, contents, version, knownOptions);
    endResetModel();
    return loaded;
}

bool GameOptions::save()
{
    return ::save(path, contents, version);
}
