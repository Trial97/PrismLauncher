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

#include <QString>
#include <array>
#include <cstdint>
#include <utility>

#include "EnumWrapper.h"

namespace Resources {

enum class TypeValue : std::uint8_t {
    Mod,
    CoreMod,
    ResourcePack,
    ShaderPack,
    Modpack,
    DataPack,
    World,
    Worlds,
    Screenshots,
    TexturePack,
    Extra,
    Unknown,
};

struct Type : EnumWrapper<Type, TypeValue> {
    static constexpr auto invalid() { return Unknown; };

    static constexpr auto mapping()
    {
        return std::array{
            std::pair{ Mod, "mod" },
            std::pair{ CoreMod, "coremod" },
            std::pair{ ResourcePack, "resourcepack" },
            std::pair{ ShaderPack, "shaderpack" },
            std::pair{ Modpack, "modpack" },
            std::pair{ DataPack, "datapack" },
            std::pair{ World, "world" },
            std::pair{ Worlds, "worlds" },
            std::pair{ Screenshots, "screenshots" },
            std::pair{ TexturePack, "texturepack" },
            std::pair{ Extra, "extra" },
        };
    };

    using enum TypeValue;
    using Base = EnumWrapper<Type, TypeValue>;
    using Base::Base; /* inherit ctor */

    QString getName() const;
};

}  // namespace Resources
