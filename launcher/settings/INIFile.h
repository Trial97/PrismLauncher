// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *  Copyright (C) 2023 flowln <flowlnlnln@gmail.com>
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

#pragma once

#include <QString>
#include <QVariant>

class SettingsFile {
   public:
    virtual ~SettingsFile() = default;
    virtual bool loadFile(QString fileName) = 0;
    virtual bool loadFile(QByteArray data) = 0;
    virtual bool saveFile(QString fileName) = 0;

    virtual QVariant get(QString key, QVariant def = {}) const = 0;
    virtual void set(QString key, QVariant val) = 0;
    virtual void remove(QString key) = 0;
    virtual bool contains(QString key) const = 0;
    virtual QVariant operator[](const QString& key) const = 0;
    virtual QStringList keys() = 0;
};

// Sectionless INI parser (for instance config files)
class INIFile : public SettingsFile {
   public:
    explicit INIFile() = default;

    bool loadFile(QString fileName);
    bool loadFile(QByteArray data);
    bool saveFile(QString fileName);

    QVariant get(QString key, QVariant def = {}) const;
    void set(QString key, QVariant val);
    void remove(QString key);
    bool contains(QString key) const;
    QVariant operator[](const QString& key) const;
    QStringList keys();

   private:
    QMap<QString, QVariant> m_values;
};
