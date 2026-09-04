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

enum class SideValue : std::uint8_t {
    Unknown = 0,
    ClientSide = 1U << 0U,
    ServerSide = 1U << 1U,
    UniversalSide = ClientSide | ServerSide,
};

struct Side : EnumWrapper<Side, SideValue> {
    static constexpr auto invalid() { return Unknown; };

    static constexpr auto mapping()
    {
        return std::array{
            std::pair{ ClientSide, "client" },
            std::pair{ ServerSide, "server" },
            std::pair{ UniversalSide, "both" },
        };
    };

    using enum SideValue;
    using Base = EnumWrapper<Side, SideValue>;
    using Base::Base; /* inherit ctor */
};

}  // namespace Resources
