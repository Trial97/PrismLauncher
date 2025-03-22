#include "minecraft/mod/format/Entry.h"
#include "modplatform/ModIndex.h"

namespace PackwizV2 {

QJsonObject Entry::toJson() const
{
    QJsonObject json;
    json["path"] = path;
    json["type"] = ModPlatform::ResourceTypeUtils::toString(type);
    json["managedByPack"] = managedByPack;
    json["enabled"] = enabled;
    json["side"] = side;
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
    entry.path = json["path"].toString();
    entry.type = ModPlatform::ResourceTypeUtils::fromString(json["type"].toString());
    entry.managedByPack = json["managedByPack"].toBool();
    entry.enabled = json["enabled"].toBool();
    entry.side = json["side"].toString();
    entry.lockVersion = json["lockVersion"].toBool();

    // Parse categories
    QJsonArray categoriesArray = json["categories"].toArray();
    for (const QJsonValue& value : categoriesArray) {
        entry.categories.append(value.toString());
    }

    // Parse info
    entry.info = Info::fromJson(json["info"].toObject());

    // Parse hashes
    QJsonArray hashesArray = json["hashes"].toArray();
    for (const QJsonValue& value : hashesArray) {
        entry.hashes.append(Hash::fromJson(value.toObject()));
    }

    // Parse providers
    QJsonArray providersArray = json["providers"].toArray();
    for (const QJsonValue& value : providersArray) {
        entry.providers.append(Provider::fromJson(value.toObject()));
    }

    return entry;
}
}  // namespace PackwizV2