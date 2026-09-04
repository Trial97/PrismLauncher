#include "ChooseProviderDialog.h"
#include "ui_ChooseProviderDialog.h"

#include <QPushButton>
#include <QRadioButton>

#include <array>

#include "modplatform/ModIndex.h"

ChooseProviderDialog::ChooseProviderDialog(QWidget* parent, bool single_choice, bool allow_skipping)
    : QDialog(parent), ui(new Ui::ChooseProviderDialog)
{
    ui->setupUi(this);

    addProviders();
    m_providers.button(0)->click();

    connect(ui->skipOneButton, &QPushButton::clicked, this, &ChooseProviderDialog::skipOne);
    connect(ui->skipAllButton, &QPushButton::clicked, this, &ChooseProviderDialog::skipAll);

    connect(ui->confirmOneButton, &QPushButton::clicked, this, &ChooseProviderDialog::confirmOne);
    connect(ui->confirmAllButton, &QPushButton::clicked, this, &ChooseProviderDialog::confirmAll);

    if (single_choice) {
        ui->providersLayout->removeWidget(ui->skipAllButton);
        ui->providersLayout->removeWidget(ui->confirmAllButton);
    }

    if (!allow_skipping) {
        ui->providersLayout->removeWidget(ui->skipOneButton);
        ui->providersLayout->removeWidget(ui->skipAllButton);
    }
}

ChooseProviderDialog::~ChooseProviderDialog()
{
    delete ui;
}

void ChooseProviderDialog::setDescription(QString desc)
{
    ui->explanationLabel->setText(desc);
}

void ChooseProviderDialog::skipOne()
{
    reject();
}
void ChooseProviderDialog::skipAll()
{
    m_response.skip_all = true;
    reject();
}

void ChooseProviderDialog::confirmOne()
{
    m_response.chosen = getSelectedProvider();
    m_response.try_others = ui->tryOthersCheckbox->isChecked();
    accept();
}
void ChooseProviderDialog::confirmAll()
{
    m_response.chosen = getSelectedProvider();
    m_response.confirm_all = true;
    m_response.try_others = ui->tryOthersCheckbox->isChecked();
    accept();
}

namespace {
constexpr std::array<Resources::Platform, 2> s_providerChoices{ Resources::Platform::Modrinth, Resources::Platform::Curseforge };
}

auto ChooseProviderDialog::getSelectedProvider() const -> Resources::Platform
{
    return s_providerChoices.at(static_cast<std::size_t>(m_providers.checkedId()));
}

void ChooseProviderDialog::addProviders()
{
    int btn_index = 0;
    QRadioButton* btn;

    for (auto& provider : s_providerChoices) {
        btn = new QRadioButton(provider.readableName(), this);
        m_providers.addButton(btn, btn_index++);
        ui->providersLayout->addWidget(btn);
    }
}

void ChooseProviderDialog::disableInput()
{
    for (auto& btn : m_providers.buttons())
        btn->setEnabled(false);

    ui->skipOneButton->setEnabled(false);
    ui->skipAllButton->setEnabled(false);
    ui->confirmOneButton->setEnabled(false);
    ui->confirmAllButton->setEnabled(false);
}
