#pragma once

#include <QJsonObject>
#include <QJsonValue>
#include "modplatform/helpers/HashUtils.h"

namespace PackwizV2 {

struct Hash {
    Hashing::Algorithm type;
    QString value;

    QJsonObject toJson() const;
    static Hash fromJson(const QJsonObject& json);
};

}  // namespace PackwizV2