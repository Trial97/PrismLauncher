#include "minecraft/mod/format/Hash.h"
#include "Json.h"

namespace PackwizV2 {

QJsonObject Hash::toJson() const
{
    QJsonObject json;
    json["type"] = Hashing::algorithmToString(type);
    json["value"] = value;
    return json;
}

Hash Hash::fromJson(const QJsonObject& json)
{
    Hash fileHash;
    fileHash.type = Hashing::algorithmFromString(Json::requireString(json["type"], "hash.type"));
    fileHash.value = Json::requireString(json["value"], "hash.value");
    return fileHash;
}

}  // namespace PackwizV2
