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

#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>

QWidget* GameOptionDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    std::shared_ptr<GameOption> knownOption = contents->at(index.column()).knownOption;

    switch (contents->at(index.column()).type) {
        case OptionType::String: {
            if (knownOption != nullptr && knownOption->validValues.isEmpty()) {
                QComboBox* comboBox = new QComboBox(parent);
                for (auto value : knownOption->validValues) {
                    comboBox->addItem(value);
                }
                comboBox->setGeometry(option.rect);
                return comboBox;
            } else {
                QLineEdit* textField = new QLineEdit(parent);
                textField->setGeometry(option.rect);
                return textField;
            }
        }
        case OptionType::Int: {
            if (knownOption != nullptr && (knownOption->getIntRange().max != 0 || knownOption->getIntRange().min != 0)) {
                QSlider* slider = new QSlider(parent);
                slider->setMinimum(knownOption->getIntRange().min);
                slider->setMaximum(knownOption->getIntRange().max);
                slider->setGeometry(option.rect);
                return slider;
            } else {
                QSpinBox* intInput = new QSpinBox(parent);
                intInput->setGeometry(option.rect);
                return intInput;
            }
        }
        case OptionType::Bool: {
            QCheckBox* checkBox = new QCheckBox(parent);
            checkBox->setGeometry(option.rect);
            return checkBox;
        }
        case OptionType::Float: {
            if (knownOption != nullptr && (knownOption->getFloatRange().max != 0 || knownOption->getFloatRange().min != 0)) {
                QSlider* slider = new QSlider(parent);
                slider->setMinimum(knownOption->getFloatRange().min);
                slider->setMaximum(knownOption->getFloatRange().max);
                slider->setGeometry(option.rect);
                return slider;
            } else {
                QDoubleSpinBox* floatInput = new QDoubleSpinBox(parent);
                floatInput->setGeometry(option.rect);
                return floatInput;
            }
        }
        case OptionType::KeyBind: {
            QPushButton* pushButton = new QPushButton(parent);
            //pushButton->addAction(QAction());
            pushButton->setGeometry(option.rect);
            return pushButton;
        }
        default:
            break;
    }
};
void GameOptionDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    std::shared_ptr<GameOption> knownOption = contents->at(index.column()).knownOption;

    switch (contents->at(index.column()).type) {
        case OptionType::String: {
            if (knownOption != nullptr && !knownOption->validValues.isEmpty()) {
                ((QComboBox*)editor)->setCurrentIndex(((QComboBox*)editor)->findText(contents->at(index.column()).value));
            } else {
                ((QLineEdit*)editor)->setText(contents->at(index.column()).value);
            }
            return;
        }
        case OptionType::Int: {
            if (knownOption != nullptr && (knownOption->getIntRange().max != 0 || knownOption->getIntRange().min != 0)) {
                ((QSlider*)editor)->setValue(contents->at(index.column()).value.toInt());
            } else {
                ((QSpinBox*)editor)->setValue(contents->at(index.column()).value.toInt());
            }
            return;
        }
        case OptionType::Bool: {
            ((QCheckBox*)editor)->setText(contents->at(index.column()).value);
            ((QCheckBox*)editor)->setChecked(contents->at(index.column()).value == "true" ? true : false);
            return;
        }
        case OptionType::Float: {
            if (knownOption != nullptr && (knownOption->getFloatRange().max != 0 || knownOption->getFloatRange().min != 0)) {
                ((QSlider*)editor)->setValue(contents->at(index.column()).value.toFloat());
            } else {
                ((QDoubleSpinBox*)editor)->setValue(contents->at(index.column()).value.toFloat());
            }
            return;
        }
        case OptionType::KeyBind: {
            ((QPushButton*)editor)->setText(contents->at(index.column()).value);
            return;
        }
        default:
            return;
    }
};
void GameOptionDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const { 
    editor->setGeometry(option.rect);
};
void GameOptionDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    std::shared_ptr<GameOption> knownOption = contents->at(index.column()).knownOption;

    switch (contents->at(index.column()).type) {
        case OptionType::String: {
            if (knownOption != nullptr && knownOption->validValues.isEmpty()) {
                contents->at(index.column()).value = ((QComboBox*)editor)->currentText();
            } else {
                contents->at(index.column()).value = ((QLineEdit*)editor)->text();
            }
            return;
        }
        case OptionType::Int: {
            if (knownOption != nullptr && (knownOption->getIntRange().max != 0 || knownOption->getIntRange().min != 0)) {
                contents->at(index.column()).intValue = ((QSlider*)editor)->value();
            } else {
                contents->at(index.column()).intValue = ((QSpinBox*)editor)->value();
            }
            return;
        }
        case OptionType::Bool: {
            ((QCheckBox*)editor)->setText(contents->at(index.column()).value);
            contents->at(index.column()).boolValue = ((QCheckBox*)editor)->isChecked();
            return;
        }
        case OptionType::Float: {
            if (knownOption != nullptr && (knownOption->getFloatRange().max != 0 || knownOption->getFloatRange().min != 0)) {
                contents->at(index.column()).floatValue = ((QSlider*)editor)->value();
            } else {
                contents->at(index.column()).floatValue = ((QDoubleSpinBox*)editor)->value();
            }
            return;
        }
        case OptionType::KeyBind: {
            contents->at(index.column()).value = ((QPushButton*)editor)->text();
            return;
        }
        default:
            return;
    }
};
