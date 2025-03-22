
#include "minecraft/mod/format/Info.h"
#include "Json.h"
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
    fileInfo.imagePath = Json::requireString(json["imagePath"], "info.imagePath");
    fileInfo.description = Json::requireString(json["description"], "info.description");
    fileInfo.newFormatId = Json::requireString(json["newFormatId"], "info.newFormatId");
    fileInfo.details = Json::requireString(json["details"], "info.details");
    fileInfo.name = Json::requireString(json["name"], "info.name");
    fileInfo.version = Json::requireString(json["version"], "info.version");
    fileInfo.homeUrl = Json::requireString(json["homeUrl"], "info.homeUrl");
    QJsonArray authorsArray = Json::requireArray(json["authors"], "info.authors");
    for (const QJsonValue& value : authorsArray) {
        fileInfo.authors.append(value.toString());
    }
    fileInfo.status = Json::requireString(json["status"], "info.status");
    fileInfo.provider = Json::requireString(json["provider"], "info.provider");
    QJsonArray licensesArray = Json::requireArray(json["licenses"], "info.licenses");
    for (const QJsonValue& value : licensesArray) {
        fileInfo.licenses.append(License::fromJson(Json::requireObject(value, "info.license")));
    }
    fileInfo.issueTracker = Json::requireString(json["issueTracker"], "info.issueTracker");
    fileInfo.id = Json::requireString(json["id"], "info.id");

    QJsonArray depsArray = Json::requireArray(json["dependencies"], "info.dependencies");
    for (const QJsonValue& value : depsArray) {
        fileInfo.dependencies.append(Json::requireString(value, "info.dependency"));
    }

    return fileInfo;
}
}  // namespace PackwizV2