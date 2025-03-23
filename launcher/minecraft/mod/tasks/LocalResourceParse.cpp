// SPDX-FileCopyrightText: 2022 Rachel Powers <508861+Ryex@users.noreply.github.com>
//
// SPDX-License-Identifier: GPL-3.0-only

/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Rachel Powers <508861+Ryex@users.noreply.github.com>
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

#include <QObject>

#include "LocalResourceParse.h"

#include "LocalDataPackParseTask.h"
#include "LocalModParseTask.h"
#include "LocalResourcePackParseTask.h"
#include "LocalShaderPackParseTask.h"
#include "LocalTexturePackParseTask.h"
#include "LocalWorldSaveParseTask.h"

static const QMap<ModPlatform::ResourceType, QString> s_packed_type_names = {
    { ModPlatform::ResourceType::RESOURCE_PACK, QObject::tr("resource pack") },
    { ModPlatform::ResourceType::TEXTURE_PACK, QObject::tr("texture pack") },
    { ModPlatform::ResourceType::DATAPACK, QObject::tr("data pack") },
    { ModPlatform::ResourceType::SHADER_PACK, QObject::tr("shader pack") },
    { ModPlatform::ResourceType::WORLD, QObject::tr("world save") },
    { ModPlatform::ResourceType::MOD, QObject::tr("mod") },
    { ModPlatform::ResourceType::UNKNOWN, QObject::tr("unknown") }
};

namespace ResourceUtils {
ModPlatform::ResourceType identify(QFileInfo file)
{
    if (file.exists() && file.isFile()) {
        if (ModUtils::validate(file)) {
            // mods can contain resource and data packs so they must be tested first
            qDebug() << file.fileName() << "is a mod";
            return ModPlatform::ResourceType::MOD;
        } else if (ResourcePackUtils::validate(file)) {
            qDebug() << file.fileName() << "is a resource pack";
            return ModPlatform::ResourceType::RESOURCE_PACK;
        } else if (TexturePackUtils::validate(file)) {
            qDebug() << file.fileName() << "is a pre 1.6 texture pack";
            return ModPlatform::ResourceType::TEXTURE_PACK;
        } else if (DataPackUtils::validate(file)) {
            qDebug() << file.fileName() << "is a data pack";
            return ModPlatform::ResourceType::DATAPACK;
        } else if (WorldSaveUtils::validate(file)) {
            qDebug() << file.fileName() << "is a world save";
            return ModPlatform::ResourceType::WORLD;
        } else if (ShaderPackUtils::validate(file)) {
            qDebug() << file.fileName() << "is a shader pack";
            return ModPlatform::ResourceType::SHADER_PACK;
        } else {
            qDebug() << "Can't Identify" << file.fileName();
        }
    } else {
        qDebug() << "Can't find" << file.absolutePath();
    }
    return ModPlatform::ResourceType::UNKNOWN;
}

QString getPackedTypeName(ModPlatform::ResourceType type)
{
    return s_packed_type_names.constFind(type).value();
}

}  // namespace ResourceUtils
