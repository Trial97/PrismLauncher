// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2023 Tayou <tayou@gmx.net>
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
#include "GameOptionDelegate.h"
#include "ui/widgets/GameOptionWidget.h"
#include "ui/widgets/GameOptionWidgetCheckBox.h"
#include "ui/widgets/GameOptionWidgetComboBox.h"
#include "ui/widgets/GameOptionWidgetSlider.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QKeySequenceEdit>
#include <QLineEdit>

QWidget* GameOptionDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    std::shared_ptr<GameOption> knownOption = contents->at(index.row()).knownOption;

    switch (contents->at(index.row()).type) {
        case OptionType::String: {
            if (knownOption != nullptr && !knownOption->validValues.isEmpty()) {
                return new GameOptionWidgetComboBox(parent, knownOption);
            } else {
                QLineEdit* textField = new QLineEdit(parent);
                return textField;
            }
        }
        case OptionType::Int: {
            if (knownOption != nullptr && (knownOption->getIntRange().max != 0 || knownOption->getIntRange().min != 0)) {
                return new GameOptionWidgetSlider(parent, knownOption);
            } else {
                QSpinBox* intInput = new QSpinBox(parent);
                return intInput;
            }
        }
        case OptionType::Bool: {
            return new GameOptionWidgetCheckBox(parent, knownOption);
        }
        case OptionType::Float: {
            if (knownOption != nullptr && (knownOption->getFloatRange().max != 0 || knownOption->getFloatRange().min != 0)) {
                return new GameOptionWidgetSlider(parent, knownOption);
            } else {
                QDoubleSpinBox* floatInput = new QDoubleSpinBox(parent);
                return floatInput;
            }
        }
        case OptionType::KeyBind: {
            QKeySequenceEdit* keySequenceEdit = new QKeySequenceEdit(parent);
            return keySequenceEdit;
        }
        default:
            break;
    }
};

void GameOptionDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    GameOptionWidget* widget = dynamic_cast<GameOptionWidget*>(editor);

    GameOptionItem optionItem = contents->at(index.row());

    if (widget != nullptr) {
        widget->setEditorData(optionItem);
    } else {
        qDebug() << "[GameOptions] Setting widget data failed because widget was still null";
    }

    /*switch (contents->at(index.row()).type) {
        case OptionType::String: {
            QComboBox* comboBox;
            if ((comboBox = dynamic_cast<QComboBox*>(editor)) != nullptr) {
                comboBox->setCurrentIndex(comboBox->findText(contents->at(index.row()).value));
            } else {
                ((QLineEdit*)editor)->setText(contents->at(index.row()).value);
            }
            return;
        }
        case OptionType::Int: {
            QSlider* slider;
            if ((slider = dynamic_cast<QSlider*>(editor)) != nullptr) {
                slider->setValue(contents->at(index.row()).intValue);
            } else {
                ((QSpinBox*)editor)->setValue(contents->at(index.row()).intValue);
            }
            return;
        }
        case OptionType::Bool: {
            ((QCheckBox*)editor)->setText(contents->at(index.row()).value);
            ((QCheckBox*)editor)->setChecked(contents->at(index.row()).boolValue);
            return;
        }
        case OptionType::Float: {
            QSlider* slider;
            if ((slider = dynamic_cast<QSlider*>(editor)) != nullptr) {
                slider->setValue(contents->at(index.row()).floatValue * 100);
            } else {
                ((QDoubleSpinBox*)editor)->setValue(contents->at(index.row()).floatValue);
            }
            return;
        }
        case OptionType::KeyBind: {
            // TODO: fall back to QLineEdit if keybind can't be represented, like some mods do (by using another key input API)
            // TODO: mouse binding? If not possible, fall back as well maybe?
            Qt::Key key;
            for (auto& keyBinding : *keybindingOptions) {
                // this could become a std::find_if eventually, if someone wants to bother making it that.
                if (keyBinding->minecraftKeyCode == contents->at(index.row()).value) {
                    key = keyBinding->qtKeyCode.keyboardKey;
                }
            }
            ((QKeySequenceEdit*)editor)->setKeySequence(QKeySequence(key));
            return;
        }
        default:
            return;
    }*/
};

void GameOptionDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
};

void GameOptionDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    std::shared_ptr<GameOption> knownOption = contents->at(index.row()).knownOption;
    /*
    switch (contents->at(index.row()).type) {
        case OptionType::String: {
            QComboBox* comboBox;
            if ((comboBox = dynamic_cast<QComboBox*>(editor)) != nullptr) {
                contents->at(index.row()).value = comboBox->currentText();
            } else {
                contents->at(index.row()).value = ((QLineEdit*)editor)->text();
            }
            return;
        }
        case OptionType::Int: {
            QSlider* slider;
            if ((slider = dynamic_cast<QSlider*>(editor)) != nullptr) {
                contents->at(index.row()).intValue = slider->value();
            } else {
                contents->at(index.row()).intValue = ((QSpinBox*)editor)->value();
            }
            return;
        }
        case OptionType::Bool: {
            ((QCheckBox*)editor)->setText(contents->at(index.row()).value);
            contents->at(index.row()).boolValue = ((QCheckBox*)editor)->isChecked();
            return;
        }
        case OptionType::Float: {
            QSlider* slider;
            if ((slider = dynamic_cast<QSlider*>(editor)) != nullptr) {
                contents->at(index.row()).floatValue = slider->value() / 100.0f;
            } else {
                contents->at(index.row()).floatValue = ((QDoubleSpinBox*)editor)->value();
            }
            return;
        }
        case OptionType::KeyBind: {
            QKeySequenceEdit* keySequenceEdit = (QKeySequenceEdit*)editor;

            QString minecraftKeyCode;
            for (auto& keyBinding : *keybindingOptions) {
                // this could become a std::find_if eventually, if someone wants to bother making it that.
                if (keyBinding->qtKeyCode.keyboardKey == keySequenceEdit->keySequence()[0].key() ||
                    keyBinding->qtKeyCode.mouseButton == keySequenceEdit->keySequence()[0].key()) {
                    minecraftKeyCode = keyBinding->minecraftKeyCode;
                }
            }
            contents->at(index.row()).value = minecraftKeyCode;
            return;
        }
        default:
            return;
    }*/
}