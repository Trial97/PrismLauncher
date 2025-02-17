#include "hematite-static/debug.h"
// #include "hematite/include/debug.h"

Q_LOGGING_CATEGORY(hematiteLogC, "launcher.hematite")

namespace prism {
namespace hematite {
namespace log {

void debug(rust::Str msg)
{
    qCDebug(hematiteLogC) << msg.data();
}
void warn(rust::Str msg)
{
    qCWarning(hematiteLogC) << msg.data();
}
void info(rust::Str msg)
{
    qCInfo(hematiteLogC) << msg.data();
}

}  // namespace log
}  // namespace hematite
}  // namespace prism
