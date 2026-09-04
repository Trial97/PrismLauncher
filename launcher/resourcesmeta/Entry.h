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

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "Details.h"
#include "Hashes.h"
#include "Side.h"
#include "Sources.h"
#include "Type.h"

namespace Resources {

// Matches an item of the top-level resource index array.
struct Entry {
    QString path;
    Type type{ Type::Unknown };
    bool enabled = false;
    Side side{ Side::Unknown };
    QStringList categories;
    Details info;
    Hashes hashes;
    Sources providers;
    QDateTime updatedAt;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);
};

}  // namespace Resources
