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

#include <QFlags>
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

struct ModLoader;
struct ModLoaders : QFlags<ModLoaderValue> {
    using QFlags::QFlags;

    constexpr explicit ModLoaders(int value);
    constexpr explicit ModLoaders(ModLoader loader);
    constexpr explicit operator ModLoader() const;

    bool isSingle() const;
    ModLoader toSingle() const;

    QString toString() const;
    static ModLoaders fromString(const QString& str);

    QList<ModLoader> toList() const;
    static ModLoaders fromList(const QStringList& loaders);
    QStringList toStringList() const;
    static ModLoaders fromStringList(const QStringList& strings);
};

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

    constexpr explicit ModLoader(int value) : Base(static_cast<ModLoaderValue>(value)) {}
    int toInt() const;

    ModLoaders toFlags() const { return static_cast<ModLoaders>(value()); }

    using enum ModLoaderValue;
    using Base = EnumWrapper<ModLoader, ModLoaderValue>;
    using Base::Base; /* inherit ctor */
};

ModLoaders operator|(ModLoaders lhs, ModLoaders rhs);
// QFlags::operator| already covers these combos but returns the sliced base QFlags<ModLoaderValue>,
// losing ModLoaders' extra API (toStringList, isSingle, ...); these keep the result typed as ModLoaders.
inline ModLoaders operator|(ModLoaders lhs, ModLoaderValue rhs)
{
    return lhs | ModLoaders(rhs);
}
inline ModLoaders operator|(ModLoaderValue lhs, ModLoaders rhs)
{
    return ModLoaders(lhs) | rhs;
}

}  // namespace Resources

Q_DECLARE_METATYPE(Resources::ModLoaderValue)
Q_DECLARE_OPERATORS_FOR_FLAGS(Resources::ModLoaders)

inline uint qHash(const Resources::ModLoader& key, uint seed = 0)
{
    return qHash(static_cast<std::uint16_t>(key.value()), seed);
}
