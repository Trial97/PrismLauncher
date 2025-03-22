
#include <QJsonArray>
#include <QJsonObject>
#include "modplatform/ModIndex.h"

#include "minecraft/mod/format/Provider.h"

namespace PackwizV2 {

QJsonObject Provider::toJson() const
{
    QJsonObject json;
    json["name"] = ModPlatform::ProviderCapabilities::name(name);
    json["id"] = id;
    json["version"] = version;
    json["url"] = url;
    json["side"] = ModPlatform::SideUtils::toString(side);

    QJsonArray loadersArray;
    for (auto loader : ModPlatform::modLoaderTypesToList(loaders)) {
        loadersArray.append(getModLoaderAsString(loader));
    }
    json["loaders"] = loadersArray;

    QJsonArray mcVersionsArray;
    for (const QString& version : mcVersions) {
        mcVersionsArray.append(version);
    }
    json["mcVersions"] = mcVersionsArray;

    json["releaseType"] = releaseType.toString();

    QJsonArray dependenciesArray;
    for (const auto& dep : dependencies) {
        dependenciesArray.append(dep.toJson());  // Assuming Dependency has toJson()
    }
    json["dependencies"] = dependenciesArray;

    return json;
}

Provider Provider::fromJson(const QJsonObject& json)
{
    Provider provider;
    provider.name = ModPlatform::ProviderCapabilities::fromString(json["name"].toString());
    provider.id = json["id"].toString();
    provider.version = json["version"].toString();
    provider.url = json["url"].toString();
    provider.side = ModPlatform::SideUtils::fromString(json["side"].toString());

    QJsonArray loadersArray = json["loaders"].toArray();
    for (const QJsonValue& value : loadersArray) {
        provider.loaders |= ModPlatform::getModLoaderFromString(value.toString());
    }

    QJsonArray mcVersionsArray = json["mcVersions"].toArray();
    for (const QJsonValue& value : mcVersionsArray) {
        provider.mcVersions.append(value.toString());
    }

    provider.releaseType = ModPlatform::IndexedVersionType::enumFromString(json["releaseType"].toString());

    QJsonArray dependenciesArray = json["dependencies"].toArray();
    for (const QJsonValue& value : dependenciesArray) {
        provider.dependencies.append(ModPlatform::Dependency::fromJson(value.toObject()));  // Assuming Dependency has fromJson()
    }

    return provider;
}
}  // namespace PackwizV2