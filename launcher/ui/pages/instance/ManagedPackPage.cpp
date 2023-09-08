// SPDX-FileCopyrightText: 2022 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "ManagedPackPage.h"
#include "ui_ManagedPackPage.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QListView>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QUrl>
#include <QUrlQuery>

#include "Application.h"
#include "InstanceList.h"
#include "InstanceTask.h"

#include "ui/dialogs/CustomMessageBox.h"
#include "ui/dialogs/ProgressDialog.h"

#include "ui/pages/modplatform/flame/FlameManagedPackPage.h"
#include "ui/pages/modplatform/modrinth/ModrinthManagedPackPage.h"

/** This is just to override the combo box popup behavior so that the combo box doesn't take the whole screen.
 *  ... thanks Qt.
 */
class NoBigComboBoxStyle : public QProxyStyle {
    Q_OBJECT

   public:
    // clang-format off
    int styleHint(QStyle::StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override
    {
        if (hint == QStyle::SH_ComboBox_Popup)
            return false;

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
    // clang-format on

    /**
     * Something about QProxyStyle and QStyle objects means they can't be free'd just
     * because all the widgets using them are gone.
     * They seems to be tied to the QApplicaiton lifecycle.
     * So make singletons tied to the lifetime of the application to clean them up and ensure they aren't
     * being remade over and over again, thus leaking memory.
     */
   public:
    static NoBigComboBoxStyle* getInstance(QStyle* style)
    {
        static QHash<QStyle*, NoBigComboBoxStyle*> s_singleton_instances_ = {};
        static std::mutex s_singleton_instances_mutex_;

        std::lock_guard<std::mutex> lock(s_singleton_instances_mutex_);
        auto inst_iter = s_singleton_instances_.constFind(style);
        NoBigComboBoxStyle* inst = nullptr;
        if (inst_iter == s_singleton_instances_.constEnd() || *inst_iter == nullptr) {
            inst = new NoBigComboBoxStyle(style);
            inst->setParent(APPLICATION);
            s_singleton_instances_.insert(style, inst);
            qDebug() << "QProxyStyle NoBigComboBox created for" << style->objectName() << style;
        } else {
            inst = *inst_iter;
        }
        return inst;
    }

   private:
    NoBigComboBoxStyle(QStyle* style) : QProxyStyle(style) {}
};

ManagedPackPage* ManagedPackPage::createPage(BaseInstance* inst, QString type, QWidget* parent)
{
    if (type == "modrinth")
        return new ModrinthManagedPackPage(inst, nullptr, parent);
    if (type == "flame" && (APPLICATION->capabilities() & Application::SupportsFlame))
        return new FlameManagedPackPage(inst, nullptr, parent);

    return new GenericManagedPackPage(inst, nullptr, parent);
}

ManagedPackPage::ManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent)
    : QWidget(parent), m_instance_window(instance_window), ui(new Ui::ManagedPackPage), m_inst(inst)
{
    Q_ASSERT(inst);

    ui->setupUi(this);

    // NOTE: GTK2 themes crash with the proxy style.
    // This seems like an upstream bug, so there's not much else that can be done.
    if (!QStyleFactory::keys().contains("gtk2")) {
        auto comboStyle = NoBigComboBoxStyle::getInstance(ui->versionsComboBox->style());
        ui->versionsComboBox->setStyle(comboStyle);
    }

    ui->reloadButton->setVisible(false);
    connect(ui->reloadButton, &QPushButton::clicked, this, [this](bool) {
        ui->reloadButton->setVisible(false);

        m_loaded = false;
        // Pretend we're opening the page again
        openedImpl();
    });

    connect(ui->changelogTextBrowser, &QTextBrowser::anchorClicked, this, [](const QUrl url) {
        if (url.scheme().isEmpty()) {
            auto querry =
                QUrlQuery(url.query()).queryItemValue("remoteUrl", QUrl::FullyDecoded);  // curseforge workaround for linkout?remoteUrl=
            auto decoded = QUrl::fromPercentEncoding(querry.toUtf8());
            auto newUrl = QUrl(decoded);
            if (newUrl.isValid() && (newUrl.scheme() == "http" || newUrl.scheme() == "https"))
                QDesktopServices ::openUrl(newUrl);
            return;
        }
        QDesktopServices::openUrl(url);
    });
}

ManagedPackPage::~ManagedPackPage()
{
    delete ui;
}

void ManagedPackPage::openedImpl()
{
    ui->packName->setText(m_inst->getManagedPackName());
    ui->packVersion->setText(m_inst->getManagedPackVersionName());
    ui->packOrigin->setText(tr("Website: <a href=%1>%2</a>    |    Pack ID: %3    |    Version ID: %4")
                                .arg(url(), displayName(), m_inst->getManagedPackID(), m_inst->getManagedPackVersionID()));

    parseManagedPack();
}

QString ManagedPackPage::displayName() const
{
    auto type = m_inst->getManagedPackType();
    if (type.isEmpty())
        return {};
    if (type == "flame")
        type = "CurseForge";
    return type.replace(0, 1, type[0].toUpper());
}

QIcon ManagedPackPage::icon() const
{
    return APPLICATION->getThemedIcon(m_inst->getManagedPackType());
}

QString ManagedPackPage::helpPage() const
{
    return {};
}

void ManagedPackPage::retranslate()
{
    ui->retranslateUi(this);
}

bool ManagedPackPage::shouldDisplay() const
{
    return m_inst->isManagedPack();
}

bool ManagedPackPage::runUpdateTask(InstanceTask* task)
{
    Q_ASSERT(task);

    unique_qobject_ptr<Task> wrapped_task(APPLICATION->instances()->wrapInstanceTask(task));

    connect(task, &Task::failed,
            [this](QString reason) { CustomMessageBox::selectable(this, tr("Error"), reason, QMessageBox::Critical)->show(); });
    connect(task, &Task::succeeded, [this, task]() {
        QStringList warnings = task->warnings();
        if (warnings.count())
            CustomMessageBox::selectable(this, tr("Warnings"), warnings.join('\n'), QMessageBox::Warning)->show();
    });
    connect(task, &Task::aborted, [this] {
        CustomMessageBox::selectable(this, tr("Task aborted"), tr("The task has been aborted by the user."), QMessageBox::Information)
            ->show();
    });

    ProgressDialog loadDialog(this);
    loadDialog.setSkipButton(true, tr("Abort"));
    loadDialog.execWithTask(task);

    return task->wasSuccessful();
}

void ManagedPackPage::suggestVersion()
{
    ui->updateButton->setText(tr("Update pack"));
    ui->updateButton->setDisabled(false);
}

void ManagedPackPage::setFailState()
{
    qDebug() << "Setting fail state!";

    // We block signals here so that suggestVersion() doesn't get called, causing an assertion fail.
    ui->versionsComboBox->blockSignals(true);
    ui->versionsComboBox->clear();
    ui->versionsComboBox->addItem(tr("Failed to search for available versions."), {});
    ui->versionsComboBox->blockSignals(false);

    ui->changelogTextBrowser->setText(tr("Failed to request changelog data for this modpack."));

    ui->updateButton->setText(tr("Cannot update!"));
    ui->updateButton->setDisabled(true);

    ui->reloadButton->setVisible(true);
}

#include "ManagedPackPage.moc"
