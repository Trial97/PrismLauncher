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

    // clang-format off
    // data version
    knownOptions["version"] = std::shared_ptr<GameOption>(new GameOption(0, "Data version of the client version this file was last saved in; used for upgrading default settings. (numeric)", true));

    // general options
    knownOptions["autoJump"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["autoSuggestions"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatColors"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatLinks"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatLinksPrompt"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["enableVsync"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["entityShadows"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["forceUnicodeFont"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["discrete_mouse_scroll"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["invertYMouse"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["realmsNotifications"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["reducedDebugInfo"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["showSubtitles"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["directionalAudio"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["touchscreen"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["fullscreen"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["bobView"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["toggleCrouch"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["toggleSprint"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["darkMojangStudiosBackground"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["hideLightningFlashes"] = std::shared_ptr<GameOption>(new GameOption(""));

    knownOptions["mouseSensitivity"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["fov"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["screenEffectScale"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["fovEffectScale"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["darknessEffectScale"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["gamma"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["renderDistance"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["simulationDistance"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["entityDistanceScaling"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["guiScale"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["particles"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["maxFps"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["difficulty‌"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["graphicsMode"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["ao"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["prioritizeChunkUpdates"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["biomeBlendRadius"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["renderClouds"] = std::shared_ptr<GameOption>(new GameOption(""));

    knownOptions["resourcePacks"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["incompatibleResourcePacks"] = std::shared_ptr<GameOption>(new GameOption(""));

    knownOptions["lastServer"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["lang"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["soundDevice"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatVisibility"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatOpacity"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatLineSpacing"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["textBackgroundOpacity"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["backgroundForChatOnly"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["hideServerAddress"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["advancedItemTooltips"] = std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["pauseOnLostFocus"] =  std::shared_ptr<GameOption>(new GameOption(false, 
            "Whether switching out of Minecraft without pressing Esc or opening an in-game interface automatically pauses the game; toggled by pressing F3 + P"));
    knownOptions["overrideWidth"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["overrideHeight"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["heldItemTooltips"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatHeightFocused"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatDelay"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatHeightUnfocused"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatScale"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["chatWidth"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["mipmapLevels"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["useNativeTransport"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["mainHand"] =  std::shared_ptr<GameOption>(new GameOption("right", "Whether the main hand appears as left or right", QStringList{ "left", "right" }));
    knownOptions["attackIndicator"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["narrator"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["tutorialStep"] =
         std::shared_ptr<GameOption>(new GameOption("movement", "Next stage of tutorial hints to display",
             QStringList{ "movement", "find_tree", "punch_tree", "open_inventory", "craft_planks", "none" } 
    ));
    knownOptions["mouseWheelSensitivity"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["rawMouseInput"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["glDebugVerbosity"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["skipMultiplayerWarning"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["skipRealms32bitWarning"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["hideMatchedNames"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["joinedFirstServer"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["hideBundleTutorial"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["syncChunkWrites"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["showAutosaveIndicator"] =  std::shared_ptr<GameOption>(new GameOption(""));
    knownOptions["allowServerListing"] =  std::shared_ptr<GameOption>(new GameOption(""));

    // Keys Binds
    /*knownOptions["key_key.attack"] = std::shared_ptr<GameOption>(new GameOption("Attack control", OptionType::KeyBind));
    knownOptions["key_key.use"] = std::shared_ptr<GameOption>(new GameOption("Use Item control", OptionType::KeyBind));
    knownOptions["key_key.forward"] = std::shared_ptr<GameOption>(new GameOption("Forward control ", OptionType::KeyBind));
    knownOptions["key_key.left"] = std::shared_ptr<GameOption>(new GameOption("Left control", OptionType::KeyBind));
    knownOptions["key_key.back"] = std::shared_ptr<GameOption>(new GameOption("Back control", OptionType::KeyBind));
    knownOptions["key_key.right"] = std::shared_ptr<GameOption>(new GameOption("Right control", OptionType::KeyBind));
    knownOptions["key_key.jump"] = std::shared_ptr<GameOption>(new GameOption("Jump control", OptionType::KeyBind));
    knownOptions["key_key.sneak"] = std::shared_ptr<GameOption>(new GameOption("Sneak control", OptionType::KeyBind));
    knownOptions["key_key.sprint"] = std::shared_ptr<GameOption>(new GameOption("Sprint control ", OptionType::KeyBind));
    knownOptions["key_key.drop"] = std::shared_ptr<GameOption>(new GameOption("Drop control ", OptionType::KeyBind));
    knownOptions["key_key.inventory"] = std::shared_ptr<GameOption>(new GameOption("Inventory control", OptionType::KeyBind));
    knownOptions["key_key.chat"] = std::shared_ptr<GameOption>(new GameOption("Chat control", OptionType::KeyBind));
    knownOptions["key_key.playerlist"] = std::shared_ptr<GameOption>(new GameOption("List Players control", OptionType::KeyBind));
    knownOptions["key_key.pickItem"] = std::shared_ptr<GameOption>(new GameOption("Pick Block control", OptionType::KeyBind));
    knownOptions["key_key.command"] = std::shared_ptr<GameOption>(new GameOption("Command control", OptionType::KeyBind));
    knownOptions["key_key.socialInteractions"] = std::shared_ptr<GameOption>(new GameOption("Social Interaction control", OptionType::KeyBind));
    knownOptions["key_key.screenshot"] = std::shared_ptr<GameOption>(new GameOption("Screenshot control", OptionType::KeyBind));
    knownOptions["key_key.togglePerspective"] = std::shared_ptr<GameOption>(new GameOption("Perspective control", OptionType::KeyBind));
    knownOptions["key_key.smoothCamera"] = std::shared_ptr<GameOption>(new GameOption("Mouse Smoothing control", OptionType::KeyBind));
    knownOptions["key_key.fullscreen"] = std::shared_ptr<GameOption>(new GameOption("Fullscreen control", OptionType::KeyBind));
    knownOptions["key_key.spectatorOutlines"] = std::shared_ptr<GameOption>(new GameOption("Visibility of player outlines in Spectator Mode control", OptionType::KeyBind));
    knownOptions["key_key.swapOffhand"] = std::shared_ptr<GameOption>(new GameOption("Swapping of items between both hands control", OptionType::KeyBind));
    knownOptions["key_key.saveToolbarActivator"] = std::shared_ptr<GameOption>(new GameOption("Save current toolbar to a slot (in Creative Mode)", OptionType::KeyBind));
    knownOptions["key_key.loadToolbarActivator"] = std::shared_ptr<GameOption>(new GameOption("Load toolbar from a slot (in Creative Mode)", OptionType::KeyBind));
    knownOptions["key_key.advancements"] = std::shared_ptr<GameOption>(new GameOption("Open the Advancements screen", OptionType::KeyBind));
    knownOptions["key_key.hotbar.1"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 1 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.2"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 2 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.3"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 3 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.4"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 4 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.5"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 5 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.6"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 6 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.7"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 7 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.8"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 8 control", OptionType::KeyBind));
    knownOptions["key_key.hotbar.9"] = std::shared_ptr<GameOption>(new GameOption("Hotbar Slot 9 control", OptionType::KeyBind));*/

    // Sound
    knownOptions["soundCategory_master"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of all sounds", Range{ 0, 1 }));
    knownOptions["soundCategory_music"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of gameplay music", Range{ 0, 1 }));
    knownOptions["soundCategory_record"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of music/sounds from Jukeboxes and Note Blocks", Range{ 0, 1 }));
    knownOptions["soundCategory_weather"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of rain and thunder", Range{ 0, 1 }));
    knownOptions["soundCategory_block"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of blocks", Range{ 0, 1 }));
    knownOptions["soundCategory_hostile"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of hostile and neutral mobs", Range{ 0, 1 }));
    knownOptions["soundCategory_neutral"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of passive mobs", Range{ 0, 1 }));
    knownOptions["soundCategory_player"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of players", Range{ 0, 1 }));
    knownOptions["soundCategory_ambient"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of cave sounds and fireworks", Range{ 0, 1 }));
    knownOptions["soundCategory_voice"] = std::shared_ptr<GameOption>(new GameOption(1.0f, "The volume of voices", Range{ 0, 1 }));

    // Model Parts
    knownOptions["modelPart_cape"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the cape is shown"));
    knownOptions["modelPart_jacket"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Jacket\" skin layer is shown"));
    knownOptions["modelPart_left_sleeve"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Left Sleeve\" skin layer is shown"));
    knownOptions["modelPart_right_sleeve"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Right Sleeve\" skin layer is shown"));
    knownOptions["modelPart_left_pants_leg"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Left Pants Leg\" skin layer is shown"));
    knownOptions["modelPart_right_pants_leg"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Right Pants Leg\" skin layer is shown"));
    knownOptions["modelPart_hat"] = std::shared_ptr<GameOption>(new GameOption(true, "Whether the \"Hat\" skin layer is shown"));

    // Misc
    knownOptions["fullscreenResolution"] = std::shared_ptr<GameOption>(new GameOption("", "Changes the resolution of the game when in fullscreen mode. The only values allowed are the values supported by the user's monitor, shown when changing the screen resolution in the operating system settings. Setting this option to a value not supported by the monitor resets the option to \"Current\". When set to \"Current\", this option is absent from options.txt. "));

    // Not vanilla - from mods or modloaders
    //knownOptions["quilt_available_resourcepacks"] = std::shared_ptr<GameOption>(new GameOption{ "", OptionType::String, false });

    // ##########################
    //  Keyboard Bindings
    // ##########################

    // Unboud
    keyboardButtons.append(new KeyBindData( "key.keyboard.unknown", 0, QObject::tr("Not bound"), Qt::Key_unknown ));  // 4th value is Qt key code

    // Mouse
    keyboardButtons.append(new KeyBindData( "key.mouse.left", 0, QObject::tr("Left Mouse Button"), Qt::MouseButton::LeftButton ));
    keyboardButtons.append(new KeyBindData( "key.mouse.right", 0, QObject::tr("Right Mouse Button"), Qt::MouseButton::RightButton ));
    keyboardButtons.append(new KeyBindData( "key.mouse.middle", 0, QObject::tr("Middle Mouse Button"), Qt::MouseButton::MiddleButton ));
    keyboardButtons.append(new KeyBindData( "key.mouse.4", 0, QObject::tr("Mouse Button 4"), Qt::MouseButton::BackButton ));
    keyboardButtons.append(new KeyBindData( "key.mouse.5", 0, QObject::tr("Mouse Button 5"), Qt::MouseButton::ExtraButton1 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.6", 0, QObject::tr("Mouse Button 6"), Qt::MouseButton::ExtraButton2 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.7", 0, QObject::tr("Mouse Button 7"), Qt::MouseButton::ExtraButton3 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.8", 0, QObject::tr("Mouse Button 8"), Qt::MouseButton::ExtraButton4 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.9", 0, QObject::tr("Mouse Button 9"), Qt::MouseButton::ExtraButton5 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.10", 0, QObject::tr("Mouse Button 10"), Qt::MouseButton::ExtraButton6 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.11", 0, QObject::tr("Mouse Button 11"), Qt::MouseButton::ExtraButton7 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.12", 0, QObject::tr("Mouse Button 12"), Qt::MouseButton::ExtraButton8 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.13", 0, QObject::tr("Mouse Button 13"), Qt::MouseButton::ExtraButton9 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.14", 0, QObject::tr("Mouse Button 14"), Qt::MouseButton::ExtraButton10 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.15", 0, QObject::tr("Mouse Button 15"), Qt::MouseButton::ExtraButton11 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.16", 0, QObject::tr("Mouse Button 16"), Qt::MouseButton::ExtraButton12 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.17", 0, QObject::tr("Mouse Button 17"), Qt::MouseButton::ExtraButton13 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.18", 0, QObject::tr("Mouse Button 18"), Qt::MouseButton::ExtraButton14 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.19", 0, QObject::tr("Mouse Button 19"), Qt::MouseButton::ExtraButton15 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.20", 0, QObject::tr("Mouse Button 20"), Qt::MouseButton::ExtraButton16 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.21", 0, QObject::tr("Mouse Button 21"), Qt::MouseButton::ExtraButton17 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.22", 0, QObject::tr("Mouse Button 22"), Qt::MouseButton::ExtraButton18 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.23", 0, QObject::tr("Mouse Button 23"), Qt::MouseButton::ExtraButton19 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.24", 0, QObject::tr("Mouse Button 24"), Qt::MouseButton::ExtraButton20 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.25", 0, QObject::tr("Mouse Button 25"), Qt::MouseButton::ExtraButton21 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.26", 0, QObject::tr("Mouse Button 26"), Qt::MouseButton::ExtraButton22 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.27", 0, QObject::tr("Mouse Button 27"), Qt::MouseButton::ExtraButton23 ));
    keyboardButtons.append(new KeyBindData( "key.mouse.28", 0, QObject::tr("Mouse Button 28"), Qt::MouseButton::ExtraButton24 ));
    // not sure what if it makes sense to go this far, but its how far Qt can count.
    
    // Number Row
    keyboardButtons.append(new KeyBindData( "key.keyboard.0", 0, QObject::tr("0"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.1", 0, QObject::tr("1"), Qt::Key::Key_1 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.2", 0, QObject::tr("2"), Qt::Key::Key_2 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.3", 0, QObject::tr("3"), Qt::Key::Key_3 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.4", 0, QObject::tr("4"), Qt::Key::Key_4 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.5", 0, QObject::tr("5"), Qt::Key::Key_5 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.6", 0, QObject::tr("6"), Qt::Key::Key_6 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.7", 0, QObject::tr("7"), Qt::Key::Key_7 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.8", 0, QObject::tr("8"), Qt::Key::Key_8 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.9", 0, QObject::tr("9"), Qt::Key::Key_9 ));

    // Letters
    keyboardButtons.append(new KeyBindData( "key.keyboard.a", 0, QObject::tr("a"), Qt::Key::Key_A ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.b", 0, QObject::tr("b"), Qt::Key::Key_B ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.c", 0, QObject::tr("c"), Qt::Key::Key_C ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.d", 0, QObject::tr("d"), Qt::Key::Key_D ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.e", 0, QObject::tr("e"), Qt::Key::Key_E ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f", 0, QObject::tr("f"), Qt::Key::Key_F ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.g", 0, QObject::tr("g"), Qt::Key::Key_G ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.h", 0, QObject::tr("h"), Qt::Key::Key_H ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.i", 0, QObject::tr("i"), Qt::Key::Key_I ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.j", 0, QObject::tr("j"), Qt::Key::Key_J ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.k", 0, QObject::tr("k"), Qt::Key::Key_K ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.l", 0, QObject::tr("l"), Qt::Key::Key_L ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.m", 0, QObject::tr("m"), Qt::Key::Key_M ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.n", 0, QObject::tr("n"), Qt::Key::Key_N ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.o", 0, QObject::tr("o"), Qt::Key::Key_O ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.p", 0, QObject::tr("p"), Qt::Key::Key_P ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.q", 0, QObject::tr("q"), Qt::Key::Key_Q ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.r", 0, QObject::tr("r"), Qt::Key::Key_R ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.s", 0, QObject::tr("s"), Qt::Key::Key_S ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.t", 0, QObject::tr("t"), Qt::Key::Key_T ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.u", 0, QObject::tr("u"), Qt::Key::Key_U ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.v", 0, QObject::tr("v"), Qt::Key::Key_V ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.w", 0, QObject::tr("w"), Qt::Key::Key_W ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.x", 0, QObject::tr("x"), Qt::Key::Key_X ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.y", 0, QObject::tr("y"), Qt::Key::Key_Y ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.z", 0, QObject::tr("z"), Qt::Key::Key_Z ));

    // F Keys
    keyboardButtons.append(new KeyBindData( "key.keyboard.f1", 0, QObject::tr("F1"), Qt::Key::Key_F1 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f2", 0, QObject::tr("F2"), Qt::Key::Key_F2 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f3", 0, QObject::tr("F3"), Qt::Key::Key_F3 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f4", 0, QObject::tr("F4"), Qt::Key::Key_F4 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f5", 0, QObject::tr("F5"), Qt::Key::Key_F5 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f6", 0, QObject::tr("F6"), Qt::Key::Key_F6 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f7", 0, QObject::tr("F7"), Qt::Key::Key_F7 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f8", 0, QObject::tr("F8"), Qt::Key::Key_F8 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f9", 0, QObject::tr("F9"), Qt::Key::Key_F9 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f10", 0, QObject::tr("F10"), Qt::Key::Key_F10 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f11", 0, QObject::tr("F11"), Qt::Key::Key_F11 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f12", 0, QObject::tr("F12"), Qt::Key::Key_F12 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f13", 0, QObject::tr("F13"), Qt::Key::Key_F13 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f14", 0, QObject::tr("F14"), Qt::Key::Key_F14 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f15", 0, QObject::tr("F15"), Qt::Key::Key_F15 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f16", 0, QObject::tr("F16"), Qt::Key::Key_F16 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f17", 0, QObject::tr("F17"), Qt::Key::Key_F17 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f18", 0, QObject::tr("F18"), Qt::Key::Key_F18 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f19", 0, QObject::tr("F19"), Qt::Key::Key_F19 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f20", 0, QObject::tr("F20"), Qt::Key::Key_F20 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f21", 0, QObject::tr("F21"), Qt::Key::Key_F21 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f22", 0, QObject::tr("F22"), Qt::Key::Key_F22 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f23", 0, QObject::tr("F23"), Qt::Key::Key_F23 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f24", 0, QObject::tr("F24"), Qt::Key::Key_F24 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.f25", 0, QObject::tr("F25"), Qt::Key::Key_F25 ));

    // Numblock
    keyboardButtons.append(new KeyBindData( "key.keyboard.num.lock", 0, QObject::tr("Num Lock"), Qt::Key::Key_NumLock ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.0", 0, QObject::tr("Keypad 0"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.1", 0, QObject::tr("Keypad 1"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.2", 0, QObject::tr("Keypad 2"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.3", 0, QObject::tr("Keypad 3"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.4", 0, QObject::tr("Keypad 4"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.5", 0, QObject::tr("Keypad 5"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.6", 0, QObject::tr("Keypad 6"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.7", 0, QObject::tr("Keypad 7"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.8", 0, QObject::tr("Keypad 8"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.9", 0, QObject::tr("Keypad 9"), Qt::Key::Key_0 ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.add", 0, QObject::tr("Keypad +"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.decimal", 0, QObject::tr("Keypad Decimal"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.enter", 0, QObject::tr("Keypad Enter"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.equal", 0, QObject::tr("Keypad ="), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.multiply", 0, QObject::tr("Keypad *"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.divide", 0, QObject::tr("Keypad /"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.keypad.subtract", 0, QObject::tr("Keypad -"), Qt::Key_unknown )); // fix maybe?

    // Arrow Keys
    keyboardButtons.append(new KeyBindData( "key.keyboard.down", 0, QObject::tr("Down Arrow"), Qt::Key::Key_Down ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left", 0, QObject::tr("Left Arrow"), Qt::Key::Key_Left ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.right", 0, QObject::tr("Right Arrow"), Qt::Key::Key_Right ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.up", 0, QObject::tr("Up Arrow"), Qt::Key::Key_Up ));

    // Other
    keyboardButtons.append(new KeyBindData( "key.keyboard.apostrophe", 0, QObject::tr("'"), Qt::Key::Key_Apostrophe ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.backslash", 0, QObject::tr("\\"), Qt::Key::Key_Backslash ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.comma", 0, QObject::tr(","), Qt::Key::Key_Comma ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.equal", 0, QObject::tr("="), Qt::Key::Key_Equal ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.grave.accent", 0, QObject::tr("`"), Qt::Key::Key_Dead_Grave ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left.bracket", 0, QObject::tr("["), Qt::Key::Key_BracketLeft ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.minus", 0, QObject::tr("-"), Qt::Key::Key_Minus ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.period", 0, QObject::tr("."), Qt::Key::Key_Period ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.right.bracket", 0, QObject::tr("]"), Qt::Key::Key_BracketRight ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.semicolon", 0, QObject::tr(";"), Qt::Key::Key_Semicolon ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.slash", 0, QObject::tr("/"), Qt::Key::Key_Slash ));

    keyboardButtons.append(new KeyBindData( "key.keyboard.space", 0, QObject::tr("Space"), Qt::Key::Key_Space ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.tab", 0, QObject::tr("Tab"), Qt::Key::Key_Tab ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left.alt", 0, QObject::tr("Left Alt"), Qt::Key::Key_Alt ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left.control", 0, QObject::tr("Left Control"), Qt::Key::Key_Control ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left.shift", 0, QObject::tr("Left Shift"), Qt::Key::Key_Shift ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.left.win", 0, QObject::tr("Left Win"), Qt::Key::Key_Meta ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.right.alt", 0, QObject::tr("Right Alt"), Qt::Key::Key_AltGr ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.right.control", 0, QObject::tr("Right Control"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.right.shift", 0, QObject::tr("Right Shift"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.right.win", 0, QObject::tr("Right Win"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.enter", 0, QObject::tr("Enter"), Qt::Key::Key_Enter ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.escape", 0, QObject::tr("Escape"), Qt::Key::Key_Escape ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.backspace", 0, QObject::tr("Backspace"), Qt::Key::Key_Backspace ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.delete", 0, QObject::tr("Delete"), Qt::Key::Key_Delete ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.end", 0, QObject::tr("End"), Qt::Key::Key_End ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.home", 0, QObject::tr("Home"), Qt::Key::Key_Home ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.insert", 0, QObject::tr("Insert"), Qt::Key::Key_Insert ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.page.down", 0, QObject::tr("Page Down"), Qt::Key::Key_PageDown ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.page.up", 0, QObject::tr("Page Up"), Qt::Key::Key_PageUp ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.caps.lock", 0, QObject::tr("Caps Lock"), Qt::Key::Key_CapsLock ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.pause", 0, QObject::tr("Pause"), Qt::Key::Key_Pause ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.scroll.lock", 0, QObject::tr("Scroll Lock"), Qt::Key::Key_ScrollLock ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.menu", 0, QObject::tr("Menu"), Qt::Key::Key_Menu ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.print.screen", 0, QObject::tr("Print Screen"), Qt::Key::Key_Print ));
    keyboardButtons.append(new KeyBindData( "key.keyboard.world.1", 0, QObject::tr("World 1"), Qt::Key_unknown )); // fix maybe?
    keyboardButtons.append(new KeyBindData( "key.keyboard.world.2", 0, QObject::tr("World 2"), Qt::Key_unknown )); // fix maybe?
    // keyboardButtons.append(new KeyBindData( "scancode.###", 0, QObject::tr("scancode.###") )); no description, no translation, no mapping?

    // clang-format on
};
