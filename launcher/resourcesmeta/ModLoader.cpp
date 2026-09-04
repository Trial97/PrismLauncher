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

#include "ModLoader.h"

#include <QtGlobal>

namespace Resources {

ModLoaderValue operator|(ModLoaderValue lhs, ModLoaderValue rhs)
{
    return static_cast<ModLoaderValue>(static_cast<std::uint16_t>(lhs) | static_cast<std::uint16_t>(rhs));
}

ModLoaderTypes operator|(ModLoader lhs, ModLoader rhs)
{
    return static_cast<ModLoaderTypes>(static_cast<ModLoaderValue>(lhs) | static_cast<ModLoaderValue>(rhs));
}

QList<ModLoaderValue> ModLoader::toList(ModLoaderTypes flags)
{
    QList<ModLoaderValue> out;
    for (std::uint16_t bit = 1U; bit != 0U; bit <<= 1U) {
        if (flags.testFlag(static_cast<ModLoaderValue>(bit))) {
            out.append(static_cast<ModLoaderValue>(bit));
        }
    }
    return out;
}

ModLoaderTypes ModLoader::fromList(const QStringList& loaders)
{
    ModLoaderTypes flags;
    for (const auto& loader : loaders) {
        flags |= ModLoader::fromString(loader).value();
    }
    return flags;
}

QStringList ModLoader::toStringList(ModLoaderTypes flags)
{
    QStringList out;
    for (const auto& loader : toList(flags)) {
        out.append(ModLoader(loader).toString());
    }
    return out;
}

}  // namespace Resources