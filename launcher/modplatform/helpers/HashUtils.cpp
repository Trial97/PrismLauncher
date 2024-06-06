#include "HashUtils.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QFile>

#include <MurmurHash2.h>

namespace Hashing {

Hasher::Ptr createHasher(QString file_path, ModPlatform::ResourceProvider provider)
{
    return makeShared<Hasher>(file_path, ModPlatform::ProviderCapabilities::hashType(provider).first());
}

QString hash(QString file_path, Algorithm alg)
{
    QCryptographicHash::Algorithm algo = QCryptographicHash::Sha1;
    switch (alg) {
        case Algorithm::Sha512:
            algo = QCryptographicHash::Sha512;
            break;
        case Algorithm::Sha1:
            algo = QCryptographicHash::Sha1;
            break;
        case Algorithm::Md5:
            algo = QCryptographicHash::Md5;
            break;
        case Algorithm::Murmur2: {
            auto should_filter_out = [](char c) { return (c == 9 || c == 10 || c == 13 || c == 32); };

            std::ifstream file_stream(file_path.toStdString(), std::ifstream::binary);
            // TODO: This is very heavy work, but apparently QtConcurrent can't use move semantics, so we can't boop this to another thread.
            // How do we make this non-blocking then?
            return QString::number(MurmurHash2(std::move(file_stream), 4 * MiB, should_filter_out));
        }
    }

    QFile file(file_path);
    if (!file.open(QFile::ReadOnly))
        return {};

    QCryptographicHash hash(algo);
    if (!hash.addData(&file))
        qCritical() << "Failed to read JAR to create hash!";
    file.close();

    Q_ASSERT(hash.result().length() == hash.hashLength(algo));
    return { hash.result().toHex() };
}

QString algorithmToString(Algorithm alg)
{
    switch (alg) {
        case Algorithm::Sha512:
            return "sha512";
        case Algorithm::Sha1:
            return "sha1";
        case Algorithm::Md5:
            return "md5";
        case Algorithm::Murmur2:
            return "murmur2";
    }
    return {};
}

Algorithm algorithmFromString(QString alg)
{
    if (alg == "sha512")
        return Algorithm::Sha512;
    if (alg == "sha1")
        return Algorithm::Sha1;
    if (alg == "md5")
        return Algorithm::Md5;
    if (alg == "murmur2")
        return Algorithm::Murmur2;
    return Algorithm::Sha1;
}

Hasher::Hasher(QString file_path, Algorithm algorithm) : m_file_path(file_path), m_algorithm(algorithm)
{
    setObjectName(QString("%1 Hasher: %2").arg(algorithmToString(m_algorithm), file_path));
}

bool Hasher::abort()
{
    return true;
}

void Hasher::executeTask()
{
    m_hash = hash(m_file_path, m_algorithm);
    if (m_hash.isEmpty()) {
        emitFailed("Empty hash!");
    } else {
        emitSucceeded();
        emit resultsReady(m_hash);
    }
}

QString Hasher::getResult() const
{
    return m_hash;
}

QString Hasher::getPath() const
{
    return m_file_path;
}

}  // namespace Hashing
QList<Hashing::Algorithm> ModPlatform::ProviderCapabilities::hashType(ModPlatform::ResourceProvider p)
{
    switch (p) {
        case ModPlatform::ResourceProvider::MODRINTH:
            return { Hashing::Algorithm::Sha512, Hashing::Algorithm::Sha1 };
        case ModPlatform::ResourceProvider::FLAME:
            return { Hashing::Algorithm::Sha1, Hashing::Algorithm::Md5, Hashing::Algorithm::Murmur2 };
    }
    return {};
}