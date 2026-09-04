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

#include <QImage>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <utility>

#include "ModLoader.h"

namespace Resources {

// Matches the "info" object of the resource index schema.
struct Details {
    QImage image;
    QString imagePath;
    QString description;
    // The resource's loader-specific id (e.g. a mod's mod_id), as declared by the resource itself.
    QString id;
    QString newFormatId;
    QString details;
    QString name;
    QString version;
    QString mcVersion;
    QUrl homeUrl;
    QStringList authors;
    ModLoaders loaders;
    QString status;
    QString provider;
    QStringList licenses;
    QUrl issueTracker;
    QStringList dependencies;  // ToDo: make this one support a specific version range

    // The numeric pack-format version (data packs / resource packs). Kept as raw ints (rather
    // than only the human `details` display string) so it can be losslessly restored without
    // re-parsing the file when its hash hasn't changed. Some pack.mcmeta files declare a single
    // `packFormat` instead; when they instead declare a min/max range, packFormat stays 0 and the
    // range is carried in packFormatMin/Max.
    int packFormat = 0;
    std::pair<int, int> packFormatMin;
    std::pair<int, int> packFormatMax;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);
};

}  // namespace Resources
