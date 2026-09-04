#pragma once

#include <QFuture>
#include <QFutureWatcher>
#include <QString>
#include <utility>

#include "modplatform/ModIndex.h"
#include "resourcesmeta/HashAlgorithm.h"
#include "tasks/Task.h"

namespace Hashing {
class Hasher : public Task {
    Q_OBJECT
   public:
    using Ptr = shared_qobject_ptr<Hasher>;

    Hasher(QString filePath, Resources::HashAlgorithm alg) : m_path(std::move(filePath)), m_alg(alg) {}
    Hasher(QString filePath, const QString& alg) : Hasher(std::move(filePath), Resources::HashAlgorithm::fromString(alg)) {}

    bool abort() override;

    void executeTask() override;

    QString getResult() const { return m_result; };
    QString getPath() const { return m_path; };

   signals:
    void resultsReady(QString hash);

   private:
    QString m_result;
    QString m_path;
    Resources::HashAlgorithm m_alg;

    QFuture<QString> m_future;
    QFutureWatcher<QString> m_watcher;
};

Hasher::Ptr createHasher(const QString& filePath, Resources::Platform provider);
Hasher::Ptr createHasher(const QString& filePath, const QString& type);

}  // namespace Hashing
