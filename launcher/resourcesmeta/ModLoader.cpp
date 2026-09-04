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

ModLoaders operator|(ModLoaders lhs, ModLoaders rhs)
{
    auto l = static_cast<std::uint16_t>(lhs);
    auto r = static_cast<std::uint16_t>(rhs);
    return { static_cast<ModLoaderValue>(l | r) };
}

int ModLoader::toInt() const
{
    return static_cast<int>(value());
}

constexpr ModLoaders::ModLoaders(int value) : QFlags(static_cast<ModLoaderValue>(static_cast<std::uint16_t>(value))) {}
constexpr ModLoaders::ModLoaders(ModLoader loader) : QFlags(static_cast<ModLoaderValue>(loader)) {}

constexpr ModLoaders::operator ModLoader() const
{
    return { static_cast<ModLoaderValue>(static_cast<std::uint16_t>(*this)) };
}

bool ModLoaders::isSingle() const
{
    auto x = static_cast<std::uint16_t>(*this);
    return (x != 0U) && ((x & (x - 1U)) == 0U);
}

ModLoader ModLoaders::toSingle() const
{
    if (!isSingle()) {
        Q_ASSERT_X(false, "ModLoaders::toSingle", "called on a value that isn't exactly one flag");
        return ModLoader::invalid();
    }
    return { static_cast<ModLoaderValue>(static_cast<std::uint16_t>(*this)) };
}

QString ModLoaders::toString() const
{
    if (!isSingle()) {
        Q_ASSERT_X(false, "ModLoaders::toString", "called on a value that isn't exactly one flag; use toStringList() instead");
        return {};
    }
    return toSingle().toString();
}

ModLoaders ModLoaders::fromString(const QString& str)
{
    return { ModLoader::fromString(str).value() };
}

QList<ModLoader> ModLoaders::toList() const
{
    QList<ModLoader> out;
    for (std::uint16_t bit = 1U; bit != 0U; bit <<= 1U) {
        if (testFlag(static_cast<ModLoaderValue>(bit))) {
            out.append(static_cast<ModLoaderValue>(bit));
        }
    }
    return out;
}

ModLoaders ModLoaders::fromList(const QStringList& loaders)
{
    ModLoaders flags;
    for (const auto& loader : loaders) {
        flags |= ModLoader::fromString(loader).value();
    }
    return flags;
}

QStringList ModLoaders::toStringList() const
{
    QStringList out;
    for (const auto& loader : toList()) {
        out.append(ModLoader(loader).toString());
    }
    return out;
}

ModLoaders ModLoaders::fromStringList(const QStringList& strings)
{
    return fromList(strings);
}

}  // namespace Resources
