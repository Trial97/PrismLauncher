#include "minecraft/mod/format/Hash.h"

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
    fileHash.type = Hashing::algorithmFromString(json["type"].toString());
    fileHash.value = json["value"].toString();
    return fileHash;
}

}  // namespace PackwizV2
