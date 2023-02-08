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
#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <memory>

enum class OptionType { String, Int, Float, Bool, KeyBind };

template <class T> struct UniversalRange {
    typename T min, max;
};

union Range {
    UniversalRange<float> floatRange;
    UniversalRange<int> intRange;
};

union OptionValue {
    float floatValue;
    int intValue;
    bool boolValue;
    // QString stringValue;
};

class GameOption {
   public:

    /// @brief Bool variant
    /// @param description
    /// @param type
    /// @param readOnly
    /// @param defaultValue
    GameOption(bool defaultBool, QString description = "", bool readOnly = false)
        : description(description)
        , type(OptionType::Bool), readOnly(readOnly) {
        defaultValue.boolValue = defaultBool;
    };

    /// @brief String variant
    /// @param description 
    /// @param type 
    /// @param readOnly 
    /// @param defaultValue 
    /// @param validValues 
    GameOption(QString defaultValue = "", QString description = "", QList<QString> validValues = QList<QString>(), bool readOnly = false)
        : description(description), type(OptionType::String), readOnly(readOnly), defaultString(defaultValue), validValues(validValues){};

    /// @brief Float variant
    /// @param description 
    /// @param type 
    /// @param readOnly 
    /// @param range 
    /// @param defaultValue 
    GameOption(float defaultValue = 0.0f, QString description = "", Range range = Range{ 0.0f, 0.0f }, bool readOnly = false)
        : description(description), type(OptionType::Float), readOnly(readOnly), range(range), defaultValue{ defaultValue }{};

    /// @brief Int variant
    /// @param description 
    /// @param type 
    /// @param readOnly 
    /// @param range 
    /// @param defaultValue 
    GameOption(int defaultInt = 0, QString description = "", bool readOnly = false, Range range = Range{ 0, 0 })
        : description(description), type(OptionType::Int), readOnly(readOnly), range(range)
    {
        defaultValue.intValue = defaultInt;
    };

    QString description;
    OptionType type;
    bool readOnly;
    Range range = { 0.0f, 0.0f };
    OptionValue defaultValue = { 0.0f };
    QString defaultString;
    QList<QString> validValues;  // if empty, treat as text input
    //int introducedVersion;
    //int removedVersion;
};

union a {
    QList<QString> enumValues;
    struct BoolMarker {
    } boolean;
};

union mouseOrKeyboardButton {
    Qt::Key keyboardKey;
    Qt::MouseButton mouseButton;
};

class KeyBindData {
   public:
    KeyBindData(QString minecraftKeyCode, int glfwCode, QString displayName, Qt::MouseButton mouseButton) : 
        minecraftKeyCode(minecraftKeyCode), glfwCode(glfwCode), displayName(displayName)
    {
        qtKeyCode.mouseButton = mouseButton;
    }

    KeyBindData(QString minecraftKeyCode, int glfwCode, QString displayName, Qt::Key keyboardKey)
        : minecraftKeyCode(minecraftKeyCode), glfwCode(glfwCode), displayName(displayName)
    {
        qtKeyCode.keyboardKey = keyboardKey;
    }


    QString minecraftKeyCode;
    int glfwCode;
    QString displayName;
    mouseOrKeyboardButton qtKeyCode;
};

/// @brief defines constraints, default values and descriptions for known game settings 
class GameOptionsSchema {
   public:
    static QMap<QString, std::shared_ptr<GameOption>>* getKnownOptions();

   private:
    static QMap<QString, std::shared_ptr<GameOption>> knownOptions;
    static QList<KeyBindData*> keyboardButtons;

    static void populateInternalLists();

};
