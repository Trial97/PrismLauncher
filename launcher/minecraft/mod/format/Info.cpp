
#include "minecraft/mod/format/Info.h"
#include "minecraft/mod/format/License.h"

namespace PackwizV2 {

QJsonObject Info::toJson() const
{
    QJsonObject json;
    json["imagePath"] = imagePath;
    json["description"] = description;
    json["newFormatId"] = newFormatId;
    json["details"] = details;
    json["name"] = name;
    json["version"] = version;
    json["homeUrl"] = homeUrl;
    QJsonArray authorsArray;
    for (auto dep : authors) {
        authorsArray.append(dep);
    }
    json["authors"] = authorsArray;
    json["status"] = status;
    json["provider"] = provider;
    QJsonArray licensesArray;
    for (auto dep : licenses) {
        licensesArray.append(dep.toJson());
    }
    json["licenses"] = licensesArray;
    json["issueTracker"] = issueTracker;
    json["id"] = id;

    QJsonArray depsArray;
    for (const QString& dep : dependencies) {
        depsArray.append(dep);
    }
    json["dependencies"] = depsArray;

    return json;
}

Info Info::fromJson(const QJsonObject& json)
{
    Info fileInfo;
    fileInfo.imagePath = json["imagePath"].toString();
    fileInfo.description = json["description"].toString();
    fileInfo.newFormatId = json["newFormatId"].toString();
    fileInfo.details = json["details"].toString();
    fileInfo.name = json["name"].toString();
    fileInfo.version = json["version"].toString();
    fileInfo.homeUrl = json["homeUrl"].toString();
    QJsonArray authorsArray = json["authors"].toArray();
    for (const QJsonValue& value : authorsArray) {
        fileInfo.authors.append(value.toString());
    }
    fileInfo.status = json["status"].toString();
    fileInfo.provider = json["provider"].toString();
    QJsonArray licensesArray = json["licenses"].toArray();
    for (const QJsonValue& value : licensesArray) {
        fileInfo.licenses.append(License::fromJson(value.toObject()));
    }
    fileInfo.issueTracker = json["issueTracker"].toString();
    fileInfo.id = json["id"].toString();

    QJsonArray depsArray = json["dependencies"].toArray();
    for (const QJsonValue& value : depsArray) {
        fileInfo.dependencies.append(value.toString());
    }

    return fileInfo;
}
}  // namespace PackwizV2