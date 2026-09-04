// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2026 Trial97 <alexandru.tripon97@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "HashAlgorithm.h"

#include <QBuffer>
#include <QDebug>
#include <QFile>

#include <memory>

#include <MurmurHash2.h>

namespace Resources {

namespace {
class QIODeviceReader : public Murmur2::Reader {
   public:
    explicit QIODeviceReader(QIODevice* device) : m_device(device) {}
    ~QIODeviceReader() override = default;
    int read(char* s, int n) override { return static_cast<int>(m_device->read(s, n)); }
    bool eof() override { return m_device->atEnd(); }
    void goToBeginning() override { m_device->seek(0); }
    virtual void close() { m_device->close(); }

   private:
    QIODevice* m_device;
};
}  // namespace

QString HashAlgorithm::hash(QIODevice* device) const
{
    if (!device->isOpen() && !device->open(QFile::ReadOnly)) {
        return "";
    }
    if (value() == HashAlgorithmValue::Murmur2) {
        auto shouldFilterOut = [](char c) { return (c == 9 || c == 10 || c == 13 || c == 32); };
        auto reader = std::make_unique<QIODeviceReader>(device);
        auto result = QString::number(Murmur2::hash(reader.get(), static_cast<std::size_t>(4) * MiB, shouldFilterOut));
        device->close();
        return result;
    }
    auto alg = toCrypto();
    if (alg == QCryptographicHash::Algorithm::NumAlgorithms) {
        device->close();
        return "";
    }
    QCryptographicHash hash(alg);
    if (!hash.addData(device)) {
        qCritical() << "Failed to read JAR to create hash!";
    }

    auto result = hash.result().toHex();
    device->close();
    return result;
}

QString HashAlgorithm::hash(const QString& fileName) const
{
    QFile file(fileName);
    return hash(&file);
}

QString HashAlgorithm::hash(QByteArray data) const
{
    QBuffer buff(&data);
    return hash(&buff);
}

QCryptographicHash::Algorithm HashAlgorithm::toCrypto() const
{
    switch (value()) {
        case HashAlgorithmValue::Md4:
            return QCryptographicHash::Algorithm::Md4;
        case HashAlgorithmValue::Md5:
            return QCryptographicHash::Algorithm::Md5;
        case HashAlgorithmValue::Sha1:
            return QCryptographicHash::Algorithm::Sha1;
        case HashAlgorithmValue::Sha256:
            return QCryptographicHash::Algorithm::Sha256;
        case HashAlgorithmValue::Sha512:
            return QCryptographicHash::Algorithm::Sha512;
        default:
            return QCryptographicHash::Algorithm::NumAlgorithms;
    }
}

}  // namespace Resources