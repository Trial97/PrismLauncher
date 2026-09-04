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

#include "Dependency.h"

namespace Resources {

QJsonObject Dependency::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("addonId"), addonId);
    if (type.isValid()) {
        obj.insert(QStringLiteral("type"), type.toString());
    }
    obj.insert(QStringLiteral("version"), version);
    return obj;
}

void Dependency::fromJson(const QJsonObject& obj)
{
    addonId = obj.value(QStringLiteral("addonId")).toString();
    type = DependencyType::fromString(obj.value(QStringLiteral("type")).toString());
    version = obj.value(QStringLiteral("version")).toString();
}

}  // namespace Resources
