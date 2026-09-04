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

#include <QByteArray>
#include <QString>
#include <array>
#include <cstdint>
#include <utility>

#include "EnumWrapper.h"

class QIODevice;

namespace Resources {

enum class HashAlgorithmValue : std::uint8_t { Md4, Md5, Sha1, Sha256, Sha512, Murmur2, Unknown };

struct HashAlgorithm : EnumWrapper<HashAlgorithm, HashAlgorithmValue> {
    static constexpr auto invalid() { return Unknown; };

    static constexpr auto mapping()
    {
        return std::array{
            std::pair{ Md4, "md4" },       std::pair{ Md5, "md5" },       std::pair{ Sha1, "sha1" },
            std::pair{ Sha256, "sha256" }, std::pair{ Sha512, "sha512" }, std::pair{ Murmur2, "murmur2" },
        };
    };

    using enum HashAlgorithmValue;
    using Base = EnumWrapper<HashAlgorithm, HashAlgorithmValue>;
    using Base::Base; /* inherit ctor */

    QString hash(QIODevice* device) const;
    QString hash(const QString& fileName) const;
    QString hash(QByteArray data) const;
};

}  // namespace Resources