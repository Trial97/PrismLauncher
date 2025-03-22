
#include <QJsonArray>
#include <QJsonObject>
#include "Json.h"
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
    provider.name = ModPlatform::ProviderCapabilities::fromString(Json::requireString(json["name"], "provider.name"));
    provider.id = Json::requireString(json["id"], "provider.id");
    provider.version = Json::requireString(json["version"], "provider.version");
    provider.url = Json::requireString(json["url"], "provider.url");
    provider.side = ModPlatform::SideUtils::fromString(Json::requireString(json["side"], "provider.side"));

    QJsonArray loadersArray = Json::requireArray(json["loaders"], "provider.loaders");
    for (const QJsonValue& value : loadersArray) {
        provider.loaders |= ModPlatform::getModLoaderFromString(Json::requireString(value, "provider.loader"));
    }

    QJsonArray mcVersionsArray = Json::requireArray(json["mcVersions"], "provider.mcVersions");
    for (const QJsonValue& value : mcVersionsArray) {
        provider.mcVersions.append(Json::requireString(value, "provider.mcVersion"));
    }

    provider.releaseType = ModPlatform::IndexedVersionType::enumFromString(json["releaseType"].toString());

    QJsonArray dependenciesArray = Json::requireArray(json["dependencies"], "provider.dependencies");
    for (const QJsonValue& value : dependenciesArray) {
        provider.dependencies.append(
            ModPlatform::Dependency::fromJson(Json::requireObject(value, "provider.dependency")));  // Assuming Dependency has fromJson()
    }

    return provider;
}
}  // namespace PackwizV2