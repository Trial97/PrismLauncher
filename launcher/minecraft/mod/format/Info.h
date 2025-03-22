#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include "minecraft/mod/format/License.h"

namespace PackwizV2 {
struct Info {
    QString imagePath;
    QString description;
    QString newFormatId;
    QString details;
    QString name;
    QString version;
    QString homeUrl;
    QStringList authors;
    QString status;
    QString provider;
    QList<License> licenses;
    QString issueTracker;
    QString id;
    QStringList dependencies;

    QJsonObject toJson() const;

    static Info fromJson(const QJsonObject& json);
};
}  // namespace PackwizV2