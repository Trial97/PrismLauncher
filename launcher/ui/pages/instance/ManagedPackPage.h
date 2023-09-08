// SPDX-FileCopyrightText: 2022 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "BaseInstance.h"

#include "ui/pages/BasePage.h"

#include <QWidget>

namespace Ui {
class ManagedPackPage;
}

class InstanceTask;
class InstanceWindow;

class ManagedPackPage : public QWidget, public BasePage {
    Q_OBJECT

   public:
    inline static ManagedPackPage* createPage(BaseInstance* inst, QWidget* parent = nullptr)
    {
        return ManagedPackPage::createPage(inst, inst->getManagedPackType(), parent);
    }

    static ManagedPackPage* createPage(BaseInstance* inst, QString type, QWidget* parent = nullptr);
    ~ManagedPackPage() override;

    [[nodiscard]] QString displayName() const override;
    [[nodiscard]] QIcon icon() const override;
    [[nodiscard]] QString helpPage() const override;
    [[nodiscard]] QString id() const override { return "managed_pack"; }
    [[nodiscard]] bool shouldDisplay() const override;

    void openedImpl() override;

    bool apply() override { return true; }
    void retranslate() override;

    /** Gets the necessary information about the managed pack, such as
     *  available versions*/
    virtual void parseManagedPack(){};

    /** URL of the managed pack.
     *  Not the version-specific one.
     */
    [[nodiscard]] virtual QString url() const { return {}; };

    void setInstanceWindow(InstanceWindow* window) { m_instance_window = window; }

   public slots:
    /** Gets the current version selection and update the UI, including the update button and the changelog.
     */
    virtual void suggestVersion();

    virtual void update(){};
    virtual void updateFromFile(){};

   protected slots:
    /** Does the necessary UI changes for when something failed.
     *
     *  This includes:
     *  - Setting an appropriate text on the version selector to indicate a fail;
     *  - Setting an appropriate text on the changelog text browser to indicate a fail;
     *  - Disable the update button.
     */
    void setFailState();

   protected:
    ManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent = nullptr);

    /** Run the InstanceTask, with a progress dialog and all.
     *  Similar to MainWindow::instanceFromInstanceTask
     *
     *  Returns whether the task was successful.
     */
    bool runUpdateTask(InstanceTask*);

   protected:
    InstanceWindow* m_instance_window = nullptr;

    Ui::ManagedPackPage* ui;
    BaseInstance* m_inst;

    bool m_loaded = false;
};

/** Simple page for when we aren't a managed pack. */
class GenericManagedPackPage final : public ManagedPackPage {
    Q_OBJECT

   public:
    GenericManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent = nullptr)
        : ManagedPackPage(inst, instance_window, parent)
    {}
    ~GenericManagedPackPage() override = default;

    // TODO: We may want to show this page with some useful info at some point.
    [[nodiscard]] bool shouldDisplay() const override { return false; };
};
