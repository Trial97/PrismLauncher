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

#include "ModrinthManagedPackPage.h"
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
#include "Markdown.h"

#include "modplatform/modrinth/ModrinthPackManifest.h"

#include "ui/InstanceWindow.h"

#include "net/ApiDownload.h"

ModrinthManagedPackPage::ModrinthManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent)
    : ManagedPackPage(inst, instance_window, parent)
{
    Q_ASSERT(inst->isManagedPack());
    connect(ui->versionsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(suggestVersion()));
    connect(ui->updateButton, &QPushButton::clicked, this, &ModrinthManagedPackPage::update);
    connect(ui->updateFromFileButton, &QPushButton::clicked, this, &ModrinthManagedPackPage::updateFromFile);
}

void ModrinthManagedPackPage::parseManagedPack()
{
    qDebug() << "Parsing Modrinth pack";

    // No need for the extra work because we already have everything we need.
    if (m_loaded)
        return;

    if (m_fetch_job && m_fetch_job->isRunning())
        m_fetch_job->abort();

    m_fetch_job.reset(new NetJob(QString("Modrinth::PackVersions(%1)").arg(m_inst->getManagedPackName()), APPLICATION->network()));
    auto response = std::make_shared<QByteArray>();

    QString id = m_inst->getManagedPackID();

    m_fetch_job->addNetAction(
        Net::ApiDownload::makeByteArray(QString("%1/project/%2/version").arg(BuildConfig.MODRINTH_PROD_URL, id), response));

    QObject::connect(m_fetch_job.get(), &NetJob::succeeded, this, [this, response, id] {
        QJsonParseError parse_error{};
        QJsonDocument doc = QJsonDocument::fromJson(*response, &parse_error);
        if (parse_error.error != QJsonParseError::NoError) {
            qWarning() << "Error while parsing JSON response from Modrinth at " << parse_error.offset
                       << " reason: " << parse_error.errorString();
            qWarning() << *response;

            setFailState();

            return;
        }

        try {
            Modrinth::loadIndexedVersions(m_pack, doc);
        } catch (const JSONValidationError& e) {
            qDebug() << *response;
            qWarning() << "Error while reading modrinth modpack version: " << e.cause();

            setFailState();
            return;
        }

        // We block signals here so that suggestVersion() doesn't get called, causing an assertion fail.
        ui->versionsComboBox->blockSignals(true);
        ui->versionsComboBox->clear();
        ui->versionsComboBox->blockSignals(false);

        for (auto version : m_pack.versions) {
            QString name = version.version;

            if (!version.name.contains(version.version))
                name = QString("%1 — %2").arg(version.name, version.version);

            // NOTE: the id from version isn't the same id in the modpack format spec...
            // e.g. HexMC's 4.4.0 has versionId 4.0.0 in the modpack index..............
            if (version.version == m_inst->getManagedPackVersionName())
                name = tr("%1 (Current)").arg(name);

            ui->versionsComboBox->addItem(name, QVariant(version.id));
        }

        suggestVersion();

        m_loaded = true;
    });
    QObject::connect(m_fetch_job.get(), &NetJob::failed, this, &ModrinthManagedPackPage::setFailState);
    QObject::connect(m_fetch_job.get(), &NetJob::aborted, this, &ModrinthManagedPackPage::setFailState);

    ui->changelogTextBrowser->setText(tr("Fetching changelogs..."));

    m_fetch_job->start();
}

QString ModrinthManagedPackPage::url() const
{
    return ModPlatform::getMetaURL(ModPlatform::ResourceProvider::MODRINTH, m_inst->getManagedPackID());
}

void ModrinthManagedPackPage::suggestVersion()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    ui->changelogTextBrowser->setHtml(markdownToHTML(version.changelog.toUtf8()));

    ManagedPackPage::suggestVersion();
}

void ModrinthManagedPackPage::update()
{
    auto index = ui->versionsComboBox->currentIndex();
    if (m_pack.versions.length() == 0) {
        setFailState();
        return;
    }
    auto version = m_pack.versions.at(index);

    QMap<QString, QString> extra_info;
    // NOTE: Don't use 'm_pack.id' here, since we didn't completely parse all the metadata for the pack, including this field.
    extra_info.insert("pack_id", m_inst->getManagedPackID());
    extra_info.insert("pack_version_id", version.id);
    extra_info.insert("original_instance_id", m_inst->id());

    auto extracted = new InstanceImportTask(version.download_url, this, std::move(extra_info));

    InstanceName inst_name(m_inst->getManagedPackName(), version.version);
    inst_name.setName(m_inst->name().replace(m_inst->getManagedPackVersionName(), version.version));
    extracted->setName(inst_name);

    extracted->setGroup(APPLICATION->instances()->getInstanceGroup(m_inst->id()));
    extracted->setIcon(m_inst->iconKey());
    extracted->setConfirmUpdate(false);

    auto did_succeed = runUpdateTask(extracted);

    if (m_instance_window && did_succeed)
        m_instance_window->close();
}

void ModrinthManagedPackPage::updateFromFile()
{
    auto output = QFileDialog::getOpenFileUrl(this, tr("Choose update file"), QDir::homePath(), "Modrinth pack (*.mrpack *.zip)");
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