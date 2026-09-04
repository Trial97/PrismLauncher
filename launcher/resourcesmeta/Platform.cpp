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

#include "Platform.h"

namespace Resources {

QString Platform::readableName() const
{
    switch (value()) {
        case Curseforge:
            return "Curseforge";
        case Modrinth:
            return "Modrinth";
        case Unknown:
            return "";
    }
    return "";
}

QStringList Platform::hashType() const
{
    switch (value()) {
        case Modrinth:
            return { "sha512", "sha1" };
        case Curseforge:
            // Try newer formats first, fall back to old format
            return { "sha1", "md5", "murmur2" };
        default:
            return {};
    }
}

}  // namespace Resources
