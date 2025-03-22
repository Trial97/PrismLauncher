#include "minecraft/mod/format/Entry.h"
#include "Json.h"
#include "modplatform/ModIndex.h"

namespace PackwizV2 {

QJsonObject Entry::toJson() const
{
    QJsonObject json;
    json["path"] = path;
    json["type"] = ModPlatform::ResourceTypeUtils::toString(type);
    json["managedByPack"] = managedByPack;
    json["enabled"] = enabled;
    json["side"] = ModPlatform::SideUtils::toString(side);
    json["lockVersion"] = lockVersion;

    // Convert categories to JSON array
    QJsonArray categoriesArray;
    for (const QString& category : categories) {
        categoriesArray.append(category);
    }
    json["categories"] = categoriesArray;

    // Convert info to JSON
    json["info"] = info.toJson();

    // Convert hashes to JSON array
    QJsonArray hashesArray;
    for (auto hash : hashes) {
        hashesArray.append(hash.toJson());
    }
    json["hashes"] = hashesArray;

    // Convert providers to JSON array
    QJsonArray providersArray;
    for (auto provider : providers) {
        providersArray.append(provider.toJson());
    }
    json["providers"] = providersArray;

    return json;
}

Entry Entry::fromJson(const QJsonObject& json)
{
    Entry entry;
    entry.path = Json::requireString(json["path"], "path");
    entry.type = ModPlatform::ResourceTypeUtils::fromString(Json::requireString(json["type"], "type"));
    entry.managedByPack = Json::requireBoolean(json["managedByPack"], "managedByPack");
    entry.enabled = Json::requireBoolean(json["enabled"], "enabled");
    entry.side = ModPlatform::SideUtils::fromString(Json::requireString(json["side"], "side"));
    entry.lockVersion = Json::requireBoolean(json["lockVersion"], "lockVersion");

    // Parse categories
    QJsonArray categoriesArray = Json::requireArray(json["categories"], "categories");
    for (const QJsonValue& value : categoriesArray) {
        entry.categories.append(Json::requireString(value, "category"));
    }

    // Parse info
    entry.info = Info::fromJson(Json::requireObject(json["info"], "info"));

    // Parse hashes
    QJsonArray hashesArray = Json::requireArray(json["hashes"], "hashes");
    for (const QJsonValue& value : hashesArray) {
        entry.hashes.append(Hash::fromJson(Json::requireObject(value, "hash")));
    }

    // Parse providers
    QJsonArray providersArray = Json::requireArray(json["providers"], "providers");
    for (const QJsonValue& value : providersArray) {
        entry.providers.append(Provider::fromJson(Json::requireObject(value, "provider")));
    }

    return entry;
}
}  // namespace PackwizV2