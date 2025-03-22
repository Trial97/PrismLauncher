#include "minecraft/mod/format/License.h"

#include <QJsonObject>
#include <QString>
#include "Json.h"

namespace PackwizV2 {

License License::parse(const QString& license)
{
    License l;
    // FIXME: come up with a better license parsing.
    // handle SPDX identifiers? https://spdx.org/licenses/
    auto parts = license.split(' ');
    QStringList notNameParts = {};
    for (auto part : parts) {
        auto _url = QUrl(part);
        if (part.startsWith("(") && part.endsWith(")"))
            _url = QUrl(part.mid(1, part.size() - 2));

        if (_url.isValid() && !_url.scheme().isEmpty() && !_url.host().isEmpty()) {
            l.url = _url.toString();
            notNameParts.append(part);
            continue;
        }
    }

    for (auto part : notNameParts) {
        parts.removeOne(part);
    }

    auto licensePart = parts.join(' ');
    l.name = licensePart;
    l.description = licensePart;

    if (parts.size() == 1) {
        l.id = parts.first();
    }
    return l;
}

License::License(const QString& name_, const QString& id_, const QString& url_, const QString& description_)
    : name(name_), id(id_), url(url_), description(description_)
{}

bool License::isEmpty()
{
    return this->name.isEmpty() && this->id.isEmpty() && this->url.isEmpty() && this->description.isEmpty();
}

QJsonObject License::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    json["id"] = id;
    json["url"] = url;
    json["description"] = description;
    return json;
}

License License::fromJson(const QJsonObject& json)
{
    License license;
    license.name = Json::requireString(json["name"], "info.license.name");
    license.id = Json::requireString(json["id"], "info.license.id");
    license.url = Json::requireString(json["url"], "info.license.url");
    license.description = Json::requireString(json["description"], "info.license.description");
    return license;
}
}  // namespace PackwizV2