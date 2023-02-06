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

#include "GameOptions.h"
#include "FileSystem.h"
#include <QDebug>
#include <QSaveFile>

static Qt::CheckState boolToState(bool b)
{
    return b ? Qt::Checked : Qt::Unchecked;
};

namespace {
bool load(const QString& path, std::vector<GameOptionItem> &contents, int & version)
{
    contents.clear();
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        qWarning() << "Failed to read options file.";
        return false;
    }
    version = 0;
    while(!file.atEnd())
    {
        QString line = QString::fromUtf8(file.readLine());
        if (line.endsWith('\n')) {
            line.chop(1);
        }
        auto parts = line.split(':');
        QString key = parts[0];
        QString value = parts[1];
        bool boolValue = false;
        int intValue = 0;
        float floatValue = 0;
        OptionType type = OptionType::String;
        qDebug() << "!!" << key << "!!";
        if (key == "version") {
            version = value.toInt();
            continue;
        };
        bool isInt = false;
        intValue = value.toInt(&isInt);
        if (isInt) {
            type = OptionType::Int;
        }
        bool isFloat = false;
        floatValue = value.toFloat(&isFloat);
        if (isFloat) {
            type = OptionType::Float;
        }
        if (QString::compare(value, "true") || QString::compare(value, "false")) {
            boolValue = QString::compare(value, "true") ? true : false;
            type = OptionType::Bool;
        }
        contents.emplace_back(GameOptionItem{ key, boolValue, intValue, floatValue, value, type });
    }
    qDebug() << "Loaded" << path << "with version:" << version;
    return true;
}
bool save(const QString& path, std::vector<GameOptionItem> &mapping, int version)
{
    QSaveFile out(path);
    if(!out.open(QIODevice::WriteOnly))
    {
        return false;
    }
    if(version != 0)
    {
        QString versionLine = QString("version:%1\n").arg(version);
        out.write(versionLine.toUtf8());
    }
    auto iter = mapping.begin();
    while (iter != mapping.end())
    {
        out.write(iter->key.toUtf8());
        out.write(":");
        out.write(iter->value.toUtf8());
        out.write("\n");
        iter++;
    }
    return out.commit();
}
}

GameOptions::GameOptions(const QString& path):
    path(path)
{
    reload();
}

QVariant GameOptions::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QAbstractListModel::headerData(section, orientation, role);
    }
    switch(section)
    {
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

    switch (role)
    {
        case Qt::DisplayRole: {
            switch (column) {
                case Column::Key: {
                    return contents[row].key;
                }
                case Column::Description: {
                    return "Description goes here!";
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
                    }
                }
                case Column::DefaultValue: {
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
                    if (contents[row].type == OptionType::Bool) {
                        return boolToState(contents[row].boolValue);
                    }
            }
        }
        case Qt::EditRole: {
        }
        default: {
            return QVariant();
        }
    }
    return QVariant();
}

int GameOptions::rowCount(const QModelIndex&) const
{
    return contents.size();
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
    loaded = load(path, contents, version);
    endResetModel();
    return loaded;
}

bool GameOptions::save()
{
    return ::save(path, contents, version);
}
