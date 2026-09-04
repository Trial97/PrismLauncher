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

#include "Type.h"

#include <QMap>
#include <QObject>

namespace Resources {

static const QMap<Type, QString> g_packedTypeNames = {
    { Type::ResourcePack, QObject::tr("resource pack") },
    { Type::TexturePack, QObject::tr("texture pack") },
    { Type::DataPack, QObject::tr("data pack") },
    { Type::ShaderPack, QObject::tr("shader pack") },
    { Type::World, QObject::tr("world save") },
    { Type::Mod, QObject::tr("mod") },
    { Type::Unknown, QObject::tr("unknown") },
};

QString Type::getName() const
{
    return g_packedTypeNames.constFind(*this).value();
}

}  // namespace Resources
