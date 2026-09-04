#include "HashUtils.h"

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QtConcurrentRun>
#include "resourcesmeta/HashAlgorithm.h"

namespace Hashing {

Hasher::Ptr createHasher(const QString& filePath, ModPlatform::ResourceProvider provider)
{
    switch (provider) {
        case ModPlatform::ResourceProvider::MODRINTH:
            return makeShared<Hasher>(filePath,
                                      ModPlatform::ProviderCapabilities::hashType(ModPlatform::ResourceProvider::MODRINTH).first());
        case ModPlatform::ResourceProvider::FLAME:
            return makeShared<Hasher>(filePath, Resources::HashAlgorithmValue::Murmur2);
        default:
            qCritical() << "[Hashing]" << "Unrecognized mod platform!";
            return nullptr;
    }
}

Hasher::Ptr createHasher(const QString& filePath, const QString& type)
{
    return makeShared<Hasher>(filePath, type);
}

void Hasher::executeTask()
{
    m_future = QtConcurrent::run(
        QThreadPool::globalInstance(), [](const QString& fileName, Resources::HashAlgorithm type) { return type.hash(fileName); }, m_path,
        m_alg);
    connect(&m_watcher, &QFutureWatcher<QString>::finished, this, [this] {
        if (m_future.isCanceled()) {
            emitAborted();
        } else if (m_result = m_future.result(); m_result.isEmpty()) {
            emitFailed("Empty hash!");
        } else {
            emit resultsReady(m_result);
            emitSucceeded();
        }
    });
    m_watcher.setFuture(m_future);
}

bool Hasher::abort()
{
    if (m_future.isRunning()) {
        m_future.cancel();
        // NOTE: Here we don't do `emitAborted()` because it will be done when `m_build_zip_future` actually cancels, which may not
        // occur immediately.
        return true;
    }
    return false;
}
}  // namespace Hashing
