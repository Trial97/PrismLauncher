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

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "Details.h"
#include "Hashes.h"
#include "Side.h"
#include "Sources.h"
#include "Type.h"

namespace Resources {

// Matches an item of the top-level resource index array.
struct Entry {
    QString path;
    Type type{ Type::Unknown };
    bool enabled = false;
    Side side{ Side::Unknown };
    QStringList categories;
    Details info;
    Hashes hashes;
    Sources providers;
    QDateTime updatedAt;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    /** The provider this resource is known from - Modrinth if available, otherwise whichever
     *  other provider is known, or Platform::Unknown if it has none. */
    Platform primaryProvider() const
    {
        if (providers.contains(Platform::Modrinth)) {
            return Platform::Modrinth;
        }
        return providers.isEmpty() ? Platform(Platform::Unknown) : providers.constBegin().key();
    }

    /** The Source for primaryProvider(), or nullptr if this resource has no known provider. */
    const Source* primarySource() const
    {
        auto it = providers.constFind(primaryProvider());
        return it == providers.constEnd() ? nullptr : &it.value();
    }
};

}  // namespace Resources
