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

#include "Hashes.h"

namespace Resources {

QJsonObject Hashes::toJson() const
{
    QJsonObject obj;
    for (auto it = constBegin(); it != constEnd(); ++it) {
        obj.insert(it.key().toString(), it.value());
    }
    return obj;
}

void Hashes::fromJson(const QJsonObject& obj)
{
    clear();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        auto algorithm = HashAlgorithm::fromString(it.key());
        if (algorithm.isValid()) {
            insert(algorithm, it.value().toString());
        }
    }
}

}  // namespace Resources