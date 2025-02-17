#pragma once

#include <QLoggingCategory>
#include <QtCore/QDebug>
#include <QtLogging>

#include "rust/cxx.h"

Q_DECLARE_LOGGING_CATEGORY(hematiteLogC);

namespace prism {
namespace hematite {
namespace log {

void debug(rust::Str msg);
void warn(rust::Str msg);
void info(rust::Str msg);

}  // namespace log
}  // namespace hematite
}  // namespace prism
