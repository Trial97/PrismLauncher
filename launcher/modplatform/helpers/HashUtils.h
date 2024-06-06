#pragma once

#include <QString>

#include "modplatform/ModIndex.h"
#include "tasks/Task.h"

namespace Hashing {

enum class Algorithm { Sha512, Sha1, Md5, Murmur2 };

QList<Algorithm> hashType(ModPlatform::ResourceProvider);
QString hash(QString file_path, Algorithm alg);
QString algorithmToString(Algorithm alg);

class Hasher : public Task {
    Q_OBJECT
   public:
    using Ptr = shared_qobject_ptr<Hasher>;

    Hasher(QString file_path, Algorithm algorithm = Algorithm::Sha1);

    /* We can't really abort this task, but we can say we aborted and finish our thing quickly :) */
    bool abort() override;

    void executeTask() override;

    QString getResult() const;
    QString getPath() const;

   signals:
    void resultsReady(QString hash);

   protected:
    QString m_hash;
    QString m_file_path;
    Algorithm m_algorithm = Algorithm::Sha1;
};

Hasher::Ptr createHasher(QString file_path, ModPlatform::ResourceProvider provider);

}  // namespace Hashing
