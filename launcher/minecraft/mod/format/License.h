#pragma once

#include <qjsonobject.h>
#include <QString>
namespace PackwizV2 {

struct License {
    QString name = {};
    QString id = {};
    QString url = {};
    QString description = {};

    License(const QString& name_ = {}, const QString& id_ = {}, const QString& url_ = {}, const QString& description_ = {});

    bool isEmpty();

    QJsonObject toJson() const;
    static License fromJson(const QJsonObject& json);
    static License parse(const QString& license);
};
}  // namespace PackwizV2