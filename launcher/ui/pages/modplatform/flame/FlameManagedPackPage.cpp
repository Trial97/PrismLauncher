// SPDX-FileCopyrightText: 2022 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *  Copyright (c) 2023 Trial97 <alexandru.tripon97@gmail.com>
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
 *
 */

#include "FlameManagedPackPage.h"
#include "ui_ManagedPackPage.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QListView>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QUrl>
#include <QUrlQuery>

#include "Application.h"
#include "BuildConfig.h"
#include "InstanceImportTask.h"
#include "InstanceList.h"
#include "InstanceTask.h"
#include "Json.h"

#include "ui/InstanceWindow.h"

#include "net/ApiDownload.h"

FlameManagedPackPage::FlameManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent)
    : ManagedPackPage(inst, instance_window, parent)
{
    Q_ASSERT(inst->isManagedPack());
    connect(ui->versionsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(suggestVersion()));
    connect(ui->updateButton, &QPushButton::clicked, this, &FlameManagedPackPage::update);
    connect(ui->updateFromFileButton, &QPushButton::clicked, this, &FlameManagedPackPage::updateFromFile);
}

void FlameManagedPackPage::parseManagedPack()
{
    qDebug() << "Parsing Flame pack";

    // We need to tell the user to redownload the pack, since we didn't save the required info previously
    if (m_inst->getManagedPackID().isEmpty()) {
        setFailState();
        QString message =
            tr("<h1>Hey there!</h1>"
               "<h4>"
               "It seems like your Pack ID is null. This is because of a bug in older versions of the launcher.<br/>"
               "Unfortunately, we can't do the proper API requests without this information.<br/>"
               "<br/>"
               "So, in order for this feature to work, you will need to re-download the modpack from the built-in downloader.<br/>"
               "<br/>"
               "Don't worry though, it will ask you to update this instance instead, so you'll not lose this instance!"
               "</h4>");

        ui->changelogTextBrowser->setHtml(message);
        return;
    }

    // No need for the extra work because we already have everything we need.
    if (m_loaded)
        return;

    if (m_fetch_job && m_fetch_job->isRunning())
        m_fetch_job->abort();

    m_fetch_job.reset(new NetJob(QString("Flame::PackVersions(%1)").arg(m_inst->getManagedPackName()), APPLICATION->network()));
    auto response = std::make_shared<QByteArray>();

    QString id = m_inst->getManagedPackID();

    m_fetch_job->addNetAction(Net::ApiDownload::makeByteArray(QString("%1/mods/%2/files").arg(BuildConfig.FLAME_BASE_URL, id), response));

    QObject::connect(m_fetch_job.get(), &NetJob::succeeded, this, [this, response, id] {
        QJsonParseError parse_error{};
        QJsonDocument doc = QJsonDocument::fromJson(*response, &parse_error);
        if (parse_error.error != QJsonParseError::NoError) {
            qWarning() << "Error while parsing JSON response from Flame at " << parse_error.offset
                       << " reason: " << parse_error.errorString();
            qWarning() << *response;

            setFailState();

            return;
        }

        try {
            auto obj = doc.object();
            auto data = Json::ensureArray(obj, "data");
            Flame::loadIndexedPackVersions(m_pack, data);
        } catch (const JSONValidationError& e) {
            qDebug() << *response;
            qWarning() << "Error while reading flame modpack version: " << e.cause();

            setFailState();
            return;
        }

        // We block signals here so that suggestVersion() doesn't get called, causing an assertion fail.
        ui->versionsComboBox->blockSignals(true);
        ui->versionsComboBox->clear();
        ui->versionsComboBox->blockSignals(false);

        for (auto version : m_pack.versions) {
            QString name = version.version;

            if (version.fileId == m_inst->getManagedPackVersionID().toInt())
                name = tr("%1 (Current)").arg(name);

            ui->versionsComboBox->addItem(name, QVariant(version.fileId));
        }

        suggestVersion();

        m_loaded = true;
    });
    QObject::connect(m_fetch_job.get(), &NetJob::failed, this, &FlameManagedPackPage::setFailState);
    QObject::connect(m_fetch_job.get(), &NetJob::aborted, this, &FlameManagedPackPage::setFailState);

    m_fetch_job->start();
}

QString FlameManagedPackPage::url() const
{
    // FIXME: We should display the websiteUrl field, but this requires doing the API request first :(
    return {};
}

void FlameManagedPackPage::suggestVersion()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    ui->changelogTextBrowser->setHtml(m_api.getModFileChangelog(m_inst->getManagedPackID().toInt(), version.fileId));

    ManagedPackPage::suggestVersion();
}

void FlameManagedPackPage::update()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    QMap<QString, QString> extra_info;
    extra_info.insert("pack_id", m_inst->getManagedPackID());
    extra_info.insert("pack_version_id", QString::number(version.fileId));
    extra_info.insert("original_instance_id", m_inst->id());

    auto extracted = new InstanceImportTask(version.downloadUrl, this, std::move(extra_info));

    extracted->setName(m_inst->name());
    extracted->setGroup(APPLICATION->instances()->getInstanceGroup(m_inst->id()));
    extracted->setIcon(m_inst->iconKey());
    extracted->setConfirmUpdate(false);

    auto did_succeed = runUpdateTask(extracted);

    if (m_instance_window && did_succeed)
        m_instance_window->close();
}

void FlameManagedPackPage::updateFromFile()
{
    auto output = QFileDialog::getOpenFileUrl(this, tr("Choose update file"), QDir::homePath(), "CurseForge pack (*.zip)");

    QMap<QString, QString> extra_info;
    extra_info.insert("pack_id", m_inst->getManagedPackID());
    extra_info.insert("pack_version_id", QString());
    extra_info.insert("original_instance_id", m_inst->id());

    auto extracted = new InstanceImportTask(output, this, std::move(extra_info));

    extracted->setName(m_inst->name());
    extracted->setGroup(APPLICATION->instances()->getInstanceGroup(m_inst->id()));
    extracted->setIcon(m_inst->iconKey());
    extracted->setConfirmUpdate(false);

    auto did_succeed = runUpdateTask(extracted);

    if (m_instance_window && did_succeed)
        m_instance_window->close();
}