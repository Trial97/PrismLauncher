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

struct GameOption {
    QString description;
    OptionType type;
    bool readOnly;
    //a extraData;
    int introducedVersion;
    int removedVersion;
};
struct GameOptionFloat : GameOption {
    std::pair<float, float> range;
    float defaultValue;
    OptionType type = OptionType::Float;
};
struct GameOptionInt : GameOption {
    std::pair<int, int> range;
    int defaultValue;
    OptionType type = OptionType::Int;
};
struct GameOptionBool : GameOption {
    bool defaultValue;
    OptionType type = OptionType::Bool;
};
struct GameOptionString : GameOption {
    QList<QString> validValues;  // if empty, treat as text input
    QString defaultValue;
    OptionType type = OptionType::String;
};
struct GameOptionKeyBind : GameOption {
    QString defaultValue;
    OptionType type = OptionType::KeyBind;
};

template <class T> struct Range {
    typename T min, max;
};

union a {
    Range<float> floatRange;
    Range<int> intRange;
    QList<QString> enumValues;
    struct BoolMarker {
    } boolean;
};

struct KeyBindData {
    QString minecraftKeyCode;
    int glfwCode;
    QString displayName;
    QKeyCombination qtKeyCode;
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
