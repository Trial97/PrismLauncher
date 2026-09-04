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

#include "Sources.h"

namespace Resources {

QJsonObject Sources::toJson() const
{
    QJsonObject obj;
    for (auto it = constBegin(); it != constEnd(); ++it) {
        if (!it.value().id.isEmpty()) {
            obj.insert(it.key().toString(), it.value().toJson());
        }
    }
    return obj;
}

void Sources::fromJson(const QJsonObject& obj)
{
    clear();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        auto providerType = Platform::fromString(it.key());
        if (providerType.isValid()) {
            Source provider;
            provider.fromJson(it.value().toObject());
            insert(providerType, provider);
        }
    }
}

}  // namespace Resources
