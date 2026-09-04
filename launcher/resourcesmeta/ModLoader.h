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

#include <QList>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <array>
#include <cstdint>
#include <utility>

#include "EnumWrapper.h"

namespace Resources {

enum class ModLoaderValue : std::uint16_t {
    Unknown = 0U,
    NeoForge = 1U << 0U,
    Forge = 1U << 1U,
    Cauldron = 1U << 2U,
    LiteLoader = 1U << 3U,
    Fabric = 1U << 4U,
    Quilt = 1U << 5U,
    DataPack = 1U << 6U,
    Babric = 1U << 7U,
    BTA = 1U << 8U,
    LegacyFabric = 1U << 9U,
    Ornithe = 1U << 10U,
    Rift = 1U << 11U,
};

using enum ModLoaderValue;
Q_DECLARE_FLAGS(ModLoaderTypes, ModLoaderValue)

struct ModLoader : EnumWrapper<ModLoader, ModLoaderValue> {
    static constexpr auto invalid() { return Unknown; };

    static constexpr auto mapping()
    {
        return std::array{
            std::pair{ NeoForge, "neoforge" },         std::pair{ Forge, "forge" },     std::pair{ Cauldron, "cauldron" },
            std::pair{ LiteLoader, "liteloader" },     std::pair{ Fabric, "fabric" },   std::pair{ Quilt, "quilt" },
            std::pair{ DataPack, "datapack" },         std::pair{ Babric, "babric" },   std::pair{ BTA, "bta" },
            std::pair{ LegacyFabric, "legacyfabric" }, std::pair{ Ornithe, "ornithe" }, std::pair{ Rift, "rift" },
        };
    };

    static QList<ModLoaderValue> toList(ModLoaderTypes flags);
    static ModLoaderTypes fromList(const QStringList& loaders);
    static QStringList toStringList(ModLoaderTypes flags);

    ModLoaderTypes toFlags() const { return static_cast<ModLoaderTypes>(value()); }

    using enum ModLoaderValue;
    using Base = EnumWrapper<ModLoader, ModLoaderValue>;
    using Base::Base; /* inherit ctor */
};

ModLoaderValue operator|(ModLoaderValue lhs, ModLoaderValue rhs);
ModLoaderTypes operator|(ModLoader lhs, ModLoader rhs);

}  // namespace Resources

Q_DECLARE_METATYPE(Resources::ModLoaderValue)
