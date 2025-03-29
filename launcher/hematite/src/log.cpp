#include "hematite-static/log.h"
#include <QString>

Q_LOGGING_CATEGORY(hematiteLogC, "launcher.hematite")

namespace prism {
namespace hematite {
namespace log {

void debug(rust::Str msg)
{
    QString qmsg = QString::fromUtf8(msg.data(), msg.size());
    qCDebug(hematiteLogC).noquote() << qmsg;
}
void warn(rust::Str msg)
{
    QString qmsg = QString::fromUtf8(msg.data(), msg.size());
    qCWarning(hematiteLogC).noquote() << qmsg;
}
void info(rust::Str msg)
{
    QString qmsg = QString::fromUtf8(msg.data(), msg.size());
    qCInfo(hematiteLogC).noquote() << qmsg;
}

}  // namespace log
}  // namespace hematite
}  // namespace prism
