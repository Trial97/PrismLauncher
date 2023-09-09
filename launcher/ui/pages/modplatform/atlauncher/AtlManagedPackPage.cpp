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

#include "AtlManagedPackPage.h"

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

#include "modplatform/atlauncher/ATLPackIndex.h"
#include "modplatform/atlauncher/ATLPackInstallTask.h"
#include "ui/pages/modplatform/atlauncher/AtlUserInteractionSupportImpl.h"

#include "net/ApiDownload.h"

AtlManagedPackPage::AtlManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent)
    : ManagedPackPage(inst, instance_window, parent)
{
    Q_ASSERT(inst->isManagedPack());
    connect(ui->versionsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(suggestVersion()));
    connect(ui->updateButton, &QPushButton::clicked, this, &AtlManagedPackPage::update);
    ui->updateFromFileButton->hide();  // no such pack file in the wild :D
}

void AtlManagedPackPage::parseManagedPack()
{
    qDebug() << "Parsing Atl pack";

    // We need to tell the user to redownload the pack, since we didn't save the required info previously
    auto id = m_inst->getManagedPackID().toInt();
    if (m_inst->getManagedPackID().isEmpty() || id == 0) {
        setFailState();
        QString message = tr(
            "<h1>Hey there!</h1>"
            "<h4>"
            "It seems like your Pack ID is null. This is because in older versions of the launcher we did not store that information.<br/>"
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

    m_fetch_job.reset(new NetJob(QString("Atl::PackVersions(%1)").arg(m_inst->getManagedPackName()), APPLICATION->network()));
    auto response = std::make_shared<QByteArray>();

    m_fetch_job->addNetAction(
        Net::ApiDownload::makeByteArray(QString(BuildConfig.ATL_DOWNLOAD_SERVER_URL + "launcher/json/packsnew.json"), response));

    QObject::connect(m_fetch_job.get(), &NetJob::succeeded, this, [this, response, id] {
        QJsonParseError parse_error;
        QJsonDocument doc = QJsonDocument::fromJson(*response, &parse_error);
        if (parse_error.error != QJsonParseError::NoError) {
            qWarning() << "Error while parsing JSON response from ATL at " << parse_error.offset
                       << " reason: " << parse_error.errorString();
            qWarning() << *response;
            return;
        }

        ;

        auto packs = doc.array();
        for (auto packRaw : packs) {
            auto packObj = packRaw.toObject();

            ATLauncher::IndexedPack pack;

            try {
                ATLauncher::loadIndexedPack(pack, packObj);
            } catch (const JSONValidationError& e) {
                qDebug() << QString::fromUtf8(*response);
                qWarning() << "Error while reading pack manifest from ATLauncher: " << e.cause();
                setFailState();
                return;
            }

            // ignore packs without a published version
            if (pack.versions.length() == 0)
                continue;
            // only display public packs (for now)
            if (pack.type != ATLauncher::PackType::Public)
                continue;
            // ignore "system" packs (Vanilla, Vanilla with Forge, etc)
            if (pack.system)
                continue;
            if (pack.id == id) {
                m_pack = pack;
                ui->changelogTextBrowser->setHtml(m_pack.description);
                break;
            }
        }

        // We block signals here so that suggestVersion() doesn't get called, causing an assertion fail.
        ui->versionsComboBox->blockSignals(true);
        ui->versionsComboBox->clear();
        ui->versionsComboBox->blockSignals(false);

        for (auto version : m_pack.versions) {
            QString name = version.version;

            if (version.version == m_inst->getManagedPackVersionID())
                name = tr("%1 (Current)").arg(name);

            ui->versionsComboBox->addItem(name, QVariant(version.version));
        }

        suggestVersion();

        m_loaded = true;
    });
    QObject::connect(m_fetch_job.get(), &NetJob::failed, this, &AtlManagedPackPage::setFailState);
    QObject::connect(m_fetch_job.get(), &NetJob::aborted, this, &AtlManagedPackPage::setFailState);

    m_fetch_job->start();
}

QString AtlManagedPackPage::url() const
{
    // FIXME: find a way to get the url
    return {};
}

void AtlManagedPackPage::suggestVersion()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    ManagedPackPage::suggestVersion();
}

void AtlManagedPackPage::update()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    auto uiSupport = new AtlUserInteractionSupportImpl(this);
    auto extracted = new ATLauncher::PackInstallTask(uiSupport, QString::number(m_pack.id), m_pack.name, version.version,
                                                     ATLauncher::InstallMode::Update, m_inst->id());

    extracted->setName(m_inst->name());
    extracted->setGroup(APPLICATION->instances()->getInstanceGroup(m_inst->id()));
    extracted->setIcon(m_inst->iconKey());
    extracted->setConfirmUpdate(false);

    auto did_succeed = runUpdateTask(extracted);

    if (m_instance_window && did_succeed)
        m_instance_window->close();
}
