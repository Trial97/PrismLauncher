#pragma once

#include "minecraft/mod/format/Hash.h"
#include "minecraft/mod/format/Info.h"
#include "minecraft/mod/format/Provider.h"
#include "modplatform/ModIndex.h"

namespace PackwizV2 {

struct Entry {
    QString path;
    ModPlatform::ResourceType type;

    bool managedByPack;
    bool enabled;
    ModPlatform::Side side;
    bool lockVersion;

    QStringList categories;
    Info info;
    QList<Hash> hashes;
    QList<Provider> providers;

    QJsonObject toJson() const;

    static Entry fromJson(const QJsonObject& json);
};
}  // namespace PackwizV2
