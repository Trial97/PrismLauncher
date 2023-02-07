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
#include "GameOptionsSchema.h"
#include <QObject>

QMap<QString, std::shared_ptr<GameOption>> GameOptionsSchema::knownOptions;
QList<KeyBindData*> GameOptionsSchema::keyboardButtons;

QMap<QString, std::shared_ptr<GameOption>>* GameOptionsSchema::getKnownOptions()
{
    if (knownOptions.isEmpty()) {
        populateInternalLists();
    }

    return &knownOptions;
}

/// @brief this data is mostly copied from https://minecraft.fandom.com/wiki/Options.txt
/// the order of options here should be the same order as in the file
void GameOptionsSchema::populateInternalLists()
{
    // data version
    knownOptions["version"] = std::shared_ptr<GameOptionInt>(new GameOptionInt{ 
        "Data version of the client version this file was last saved in; used for upgrading default settings. (numeric)",
        OptionType::Int,
        true
    });

    // general options
    knownOptions["autoJump"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["autoSuggestions"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["chatColors"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["chatLinks"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["chatLinksPrompt"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["enableVsync"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["entityShadows"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["forceUnicodeFont"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["discrete_mouse_scroll"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["invertYMouse"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["realmsNotifications"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["reducedDebugInfo"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["showSubtitles"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["directionalAudio"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["touchscreen"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["fullscreen"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["bobView"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["toggleCrouch"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["toggleSprint"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["darkMojangStudiosBackground"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["hideLightningFlashes"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });

    knownOptions["mouseSensitivity"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["fov"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["screenEffectScale"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["fovEffectScale"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["darknessEffectScale"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["gamma"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["renderDistance"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["simulationDistance"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["entityDistanceScaling"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["guiScale"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["particles"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["maxFps"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["difficulty‌"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["graphicsMode"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["ao"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["prioritizeChunkUpdates"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["biomeBlendRadius"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["renderClouds"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });

    knownOptions["resourcePacks"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });
    knownOptions["incompatibleResourcePacks"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });

    knownOptions["lastServer"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });
    knownOptions["lang"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });
    knownOptions["soundDevice"] = std::shared_ptr<GameOptionString>(new GameOptionString{ "" });
    knownOptions["chatVisibility"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["chatOpacity"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["chatLineSpacing"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["textBackgroundOpacity"] = std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["backgroundForChatOnly"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["hideServerAddress"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["advancedItemTooltips"] = std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["pauseOnLostFocus"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{
        "Whether switching out of Minecraft without pressing Esc or opening an in-game interface automatically pauses the game; toggled by "
        "pressing F3 + P"
    });
    knownOptions["overrideWidth"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["overrideHeight"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["heldItemTooltips"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["chatHeightFocused"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["chatDelay"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["chatHeightUnfocused"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["chatScale"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["chatWidth"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["mipmapLevels"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["useNativeTransport"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["mainHand"] =  std::shared_ptr<GameOptionString>(new GameOptionString{
        "Whether the main hand appears as left or right", OptionType::String, false, 0, 0, QStringList{ "left", "right" }, "right"
    });
    knownOptions["attackIndicator"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["narrator"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["tutorialStep"] =
         std::shared_ptr<GameOptionString>(new GameOptionString{ "Next stage of tutorial hints to display",
                              OptionType::String,
                              false,
                              0,
                              0,
                              QStringList{ "movement", "find_tree", "punch_tree", "open_inventory", "craft_planks", "none" },
                              "movement" });
    knownOptions["mouseWheelSensitivity"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["rawMouseInput"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["glDebugVerbosity"] =  std::shared_ptr<GameOptionInt>(new GameOptionInt{ "" });
    knownOptions["skipMultiplayerWarning"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["skipRealms32bitWarning"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["hideMatchedNames"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["joinedFirstServer"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["hideBundleTutorial"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["syncChunkWrites"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["showAutosaveIndicator"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["allowServerListing"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });

    // Keys Binds
    knownOptions["key_key.attack"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.use"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.forward"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.left"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.back"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.right"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.jump"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.sneak"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.sprint"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.drop"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.inventory"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.chat"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.playerlist"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.pickItem"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.command"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.socialInteractions"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.screenshot"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.togglePerspective"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.smoothCamera"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.fullscreen"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.spectatorOutlines"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.swapOffhand"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.saveToolbarActivator"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.loadToolbarActivator"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.advancements"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.1"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.2"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.3"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.4"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.5"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.6"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.7"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.8"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });
    knownOptions["key_key.hotbar.9"] =  std::shared_ptr<GameOptionKeyBind>(new GameOptionKeyBind{ "" });

    // Sound
    knownOptions["soundCategory_master"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_music"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_record"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_weather"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_block"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_hostile"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_neutral"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_player"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_ambient"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });
    knownOptions["soundCategory_voice"] =  std::shared_ptr<GameOptionFloat>(new GameOptionFloat{ "" });

    // Model Parts
    knownOptions["modelPart_cape"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_jacket"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_left_sleeve"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_right_sleeve"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_left_pants_leg"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_right_pants_leg"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });
    knownOptions["modelPart_hat"] =  std::shared_ptr<GameOptionBool>(new GameOptionBool{ "" });

    // Misc
    knownOptions["fullscreenResolution"] =  std::shared_ptr<GameOption>(new GameOption{ "" });

    // Not vanilla - from mods or modloaders
    knownOptions["quilt_available_resourcepacks"] =  std::shared_ptr<GameOption>(new GameOption{ "", OptionType::String, false });

    // ##########################
    //  Keyboard Bindings
    // ##########################

    // Unboud
    keyboardButtons.append(new KeyBindData{ "key.keyboard.unknown", 0, QObject::tr("Not bound") });  // 4th value is Qt key code

    // Mouse
    keyboardButtons.append(new KeyBindData{ "key.mouse.left", 0, QObject::tr("Left Mouse Button") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.right", 0, QObject::tr("Right Mouse Button") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.middle", 0, QObject::tr("Middle Mouse Button") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.4", 0, QObject::tr("Mouse Button 4") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.5", 0, QObject::tr("Mouse Button 5") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.6", 0, QObject::tr("Mouse Button 6") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.7", 0, QObject::tr("Mouse Button 7") });
    keyboardButtons.append(new KeyBindData{ "key.mouse.8", 0, QObject::tr("Mouse Button 8") });
    // keyboardButtons.append(new KeyBindData{ "key.mouse.#", 0, QObject::tr("Mouse Button #") }); // not sure what this means, we might need to just continue this up to 30 or smth to include MMO mice.
    
    // Number Row
    keyboardButtons.append(new KeyBindData{ "key.keyboard.0", 0, QObject::tr("0") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.1", 0, QObject::tr("1") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.2", 0, QObject::tr("2") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.3", 0, QObject::tr("3") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.4", 0, QObject::tr("4") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.5", 0, QObject::tr("5") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.6", 0, QObject::tr("6") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.7", 0, QObject::tr("7") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.8", 0, QObject::tr("8") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.9", 0, QObject::tr("9") });

    // Letters
    keyboardButtons.append(new KeyBindData{ "key.keyboard.a", 0, QObject::tr("a") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.b", 0, QObject::tr("b") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.c", 0, QObject::tr("c") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.d", 0, QObject::tr("d") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.e", 0, QObject::tr("e") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f", 0, QObject::tr("f") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.g", 0, QObject::tr("g") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.h", 0, QObject::tr("h") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.i", 0, QObject::tr("i") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.j", 0, QObject::tr("j") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.k", 0, QObject::tr("k") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.l", 0, QObject::tr("l") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.m", 0, QObject::tr("m") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.n", 0, QObject::tr("n") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.o", 0, QObject::tr("o") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.p", 0, QObject::tr("p") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.q", 0, QObject::tr("q") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.r", 0, QObject::tr("r") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.s", 0, QObject::tr("s") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.t", 0, QObject::tr("t") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.u", 0, QObject::tr("u") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.v", 0, QObject::tr("v") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.w", 0, QObject::tr("w") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.x", 0, QObject::tr("x") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.y", 0, QObject::tr("y") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.z", 0, QObject::tr("z") });

    // F Keys
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f1", 0, QObject::tr("F1") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f2", 0, QObject::tr("F2") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f3", 0, QObject::tr("F3") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f4", 0, QObject::tr("F4") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f5", 0, QObject::tr("F5") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f6", 0, QObject::tr("F6") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f7", 0, QObject::tr("F7") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f8", 0, QObject::tr("F8") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f9", 0, QObject::tr("F9") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f10", 0, QObject::tr("F10") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f11", 0, QObject::tr("F11") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f12", 0, QObject::tr("F12") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f13", 0, QObject::tr("F13") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f14", 0, QObject::tr("F14") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f15", 0, QObject::tr("F15") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f16", 0, QObject::tr("F16") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f17", 0, QObject::tr("F17") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f18", 0, QObject::tr("F18") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f19", 0, QObject::tr("F19") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f20", 0, QObject::tr("F20") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f21", 0, QObject::tr("F21") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f22", 0, QObject::tr("F22") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f23", 0, QObject::tr("F23") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f24", 0, QObject::tr("F24") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.f25", 0, QObject::tr("F25") });

    // Numblock
    keyboardButtons.append(new KeyBindData{ "key.keyboard.num.lock", 0, QObject::tr("Num Lock") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.0", 0, QObject::tr("Keypad 0") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.1", 0, QObject::tr("Keypad 1") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.2", 0, QObject::tr("Keypad 2") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.3", 0, QObject::tr("Keypad 3") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.4", 0, QObject::tr("Keypad 4") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.5", 0, QObject::tr("Keypad 5") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.6", 0, QObject::tr("Keypad 6") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.7", 0, QObject::tr("Keypad 7") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.8", 0, QObject::tr("Keypad 8") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.9", 0, QObject::tr("Keypad 9") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.add", 0, QObject::tr("Keypad +") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.decimal", 0, QObject::tr("Keypad Decimal") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.enter", 0, QObject::tr("Keypad Enter") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.equal", 0, QObject::tr("Keypad =") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.multiply", 0, QObject::tr("Keypad *") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.divide", 0, QObject::tr("Keypad /") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.keypad.subtract", 0, QObject::tr("Keypad -") });

    // Arrow Keys
    keyboardButtons.append(new KeyBindData{ "key.keyboard.down", 0, QObject::tr("Down Arrow") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left", 0, QObject::tr("Left Arrow") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right", 0, QObject::tr("Right Arrow") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.up", 0, QObject::tr("Up Arrow") });

    // Other
    keyboardButtons.append(new KeyBindData{ "key.keyboard.apostrophe", 0, QObject::tr("'") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.backslash", 0, QObject::tr("\\") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.comma", 0, QObject::tr(",") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.equal", 0, QObject::tr("=") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.grave.accent", 0, QObject::tr("`") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left.bracket", 0, QObject::tr("[") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.minus", 0, QObject::tr("-") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.period", 0, QObject::tr(".") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right.bracket", 0, QObject::tr("]") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.semicolon", 0, QObject::tr(";") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.slash", 0, QObject::tr("/") });

    keyboardButtons.append(new KeyBindData{ "key.keyboard.space", 0, QObject::tr("Space") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.tab", 0, QObject::tr("Tab") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left.alt", 0, QObject::tr("Left Alt") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left.control", 0, QObject::tr("Left Control") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left.shift", 0, QObject::tr("Left Shift") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.left.win", 0, QObject::tr("Left Win") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right.alt", 0, QObject::tr("Right Alt") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right.control", 0, QObject::tr("Right Control") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right.shift", 0, QObject::tr("Right Shift") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.right.win", 0, QObject::tr("Right Win") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.enter", 0, QObject::tr("Enter") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.escape", 0, QObject::tr("Escape") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.backspace", 0, QObject::tr("Backspace") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.delete", 0, QObject::tr("Delete") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.end", 0, QObject::tr("End") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.home", 0, QObject::tr("Home") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.insert", 0, QObject::tr("Insert") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.page.down", 0, QObject::tr("Page Down") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.page.up", 0, QObject::tr("Page Up") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.caps.lock", 0, QObject::tr("Caps Lock") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.pause", 0, QObject::tr("Pause") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.scroll.lock", 0, QObject::tr("Scroll Lock") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.menu", 0, QObject::tr("Menu") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.print.screen", 0, QObject::tr("Print Screen") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.world.1", 0, QObject::tr("World 1") });
    keyboardButtons.append(new KeyBindData{ "key.keyboard.world.2", 0, QObject::tr("World 2") });
    // keyboardButtons.append(new KeyBindData{ "scancode.###", 0, QObject::tr("scancode.###") }); no description, no translation, no mapping?
};
