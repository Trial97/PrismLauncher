#pragma once

#include "modplatform/ModIndex.h"

namespace PackwizV2 {
struct Provider {
    ModPlatform::ResourceProvider name;
    QString id;
    QString version;
    QString url;
    ModPlatform::Side side;
    ModPlatform::ModLoaderTypes loaders;
    QStringList mcVersions;
    ModPlatform::IndexedVersionType releaseType;
    QList<ModPlatform::Dependency> dependencies;

    QJsonObject toJson() const;

    static Provider fromJson(const QJsonObject& json);
};
}  // namespace PackwizV2