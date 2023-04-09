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
#include <memory>

QMap<QString, std::shared_ptr<GameOption>> GameOptionsSchema::knownOptions;
QList<std::shared_ptr<KeyBindData>> GameOptionsSchema::keyboardButtons;

QMap<QString, std::shared_ptr<GameOption>>* GameOptionsSchema::getKnownOptions()
{
    if (knownOptions.isEmpty()) {
        populateInternalOptionList();
    }

    return &knownOptions;
}

QList<std::shared_ptr<KeyBindData>>* GameOptionsSchema::getKeyBindOptions()
{
    if (keyboardButtons.isEmpty()) {
        populateInternalKeyBindList();
    }

    return &keyboardButtons;
}

/// @brief this data is mostly copied from https://minecraft.fandom.com/wiki/Options.txt
/// the order of options here should be the same order as in the file
void GameOptionsSchema::populateInternalOptionList()
{
    // clang-format off
    
    // data version
    knownOptions["version"]                 = std::make_shared<GameOption>(0, "Data version of the client version this file was last saved in; used for upgrading default settings. (numeric)", true);

    // general options
    knownOptions["autoJump"]                = std::make_shared<GameOption>(false, "Whether auto-jump is enabled");
    knownOptions["autoSuggestions"]         = std::make_shared<GameOption>(true,  "True if brigadier's command suggestion UI should always be shown, instead of just when pressing tab");
    knownOptions["chatColors"]              = std::make_shared<GameOption>(true,  "Whether colored chat is allowed");
    knownOptions["chatLinks"]               = std::make_shared<GameOption>(true,  "Whether links show as links or just text in the chat");
    knownOptions["chatLinksPrompt"]         = std::make_shared<GameOption>(true,  "Whether clicking on links in chat needs confirmation before opening them");
    knownOptions["enableVsync"]             = std::make_shared<GameOption>(true,  "Whether v-sync (vertical synchronization) is enabled");
    knownOptions["entityShadows"]           = std::make_shared<GameOption>(true,  "Whether to display entity shadows");
    knownOptions["forceUnicodeFont"]        = std::make_shared<GameOption>(false, "Whether Unicode font should be used");
    knownOptions["discrete_mouse_scroll"]   = std::make_shared<GameOption>(false, "Ignores scrolling set by operating system");
    knownOptions["invertYMouse"]            = std::make_shared<GameOption>(false, "Whether mouse is inverted or not");
    knownOptions["realmsNotifications"]     = std::make_shared<GameOption>(true,  "Whether Realms invites are alerted on the main menu");
    knownOptions["reducedDebugInfo"]        = std::make_shared<GameOption>(false, "Whether to show reduced information on the Debug screen");
    knownOptions["showSubtitles"]           = std::make_shared<GameOption>(false, "If subtitles are shown");
    knownOptions["directionalAudio"]        = std::make_shared<GameOption>(false, "If Directional Audio is enabled");
    knownOptions["touchscreen"]             = std::make_shared<GameOption>(false, "Whether touchscreen controls are used");
    knownOptions["fullscreen"]              = std::make_shared<GameOption>(false, "Whether the game attempts to go fullscreen at startup");
    knownOptions["bobView"]                 = std::make_shared<GameOption>(true,  "Whether or not the camera bobs up and down as the player walks");
    knownOptions["toggleCrouch"]            = std::make_shared<GameOption>(false, "Whether the sneak key must be pressed or held to activate sneaking");
    knownOptions["toggleSprint"]            = std::make_shared<GameOption>(false, "Whether the sprint key must be pressed or held to activate sprinting");
    knownOptions["darkMojangStudiosBackground"] = std::make_shared<GameOption>(false, "Whether the Mojang Studios loading screen will appear monochrome");
    knownOptions["hideLightningFlashes"]    = std::make_shared<GameOption>(false, "Hide lightning flashes (visual effect)");

    knownOptions["mouseSensitivity"]        = std::make_shared<GameOption>(0.5,   "How much a mouse movement changes the position of the camera", Range<float>{ 0.0, 1.0 });
    // FOV: The in-game value is counted in degrees, however, the options.txt isn't. The value is converted into degrees with the following formula: degrees = 40 * value + 70
    knownOptions["fov"]                     = std::make_shared<GameOption>(0.0,   "How large the field of view is", Range<float>{ -1.0, 1.0 });
    knownOptions["screenEffectScale"]       = std::make_shared<GameOption>(1.0,   "Distortion Effects (how intense the effects of Nausea and nether portals are)", Range<float>{ 0.0, 1.0 });
    knownOptions["fovEffectScale"]          = std::make_shared<GameOption>(1.0,   "FOV Effects (how much the field of view changes when sprinting, having Speed or Slowness etc.)", Range<float>{ 0.0, 1.0 });
    knownOptions["darknessEffectScale"]     = std::make_shared<GameOption>(1.0,   "Darkness Pulsing (how much the Darkness effect pulses)", Range<float>{ 0.0, 1.0 });
    knownOptions["gamma"]                   = std::make_shared<GameOption>(0.5,   "Brightness", Range<float>{ 0.0, 1.0 });
    knownOptions["renderDistance"]          = std::make_shared<GameOption>(12,    "The render distance in the chunk radius from the player - Note: The Maximum and default in vanilla is 32 and 12, 16 and 8 on 32bit machines", Range<int>{ 2, 64 });
    knownOptions["simulationDistance"]      = std::make_shared<GameOption>(12,    "The simulation distance in the chunk radius from the player - Note: The Maximum and default in vanilla is 32 and 12, 16 and 8 on 32bit machines", Range<int>{ 5, 32 });
    knownOptions["entityDistanceScaling"]   = std::make_shared<GameOption>(1.0,   "The maximum distance from the player that entities render", Range<float>{ 0.5, 5.0 });
    knownOptions["guiScale"]                = std::make_shared<GameOption>(0,     "Size of interfaces - Note: 0 (Auto) or 1+ for size. Upper limit based on window resolution");
    knownOptions["particles"]               = std::make_shared<GameOption>(0,     "Amount of particles (such as rain, potion effects, etc.)", Range<int>{ 0, 2 });
    knownOptions["maxFps"]                  = std::make_shared<GameOption>(120,   "The maximum framerate", Range<int>{ 10, 260 });
    knownOptions["difficulty‌"]              = std::make_shared<GameOption>(2,     "Has no effect after 1.7.2", Range<int>{ 0, 3 });
    knownOptions["graphicsMode"]            = std::make_shared<GameOption>(1,     "Whether Fast (less detailed), Fancy (more detailed), or Fabulous! (most detailed) graphics are turned on", Range<int>{ 0, 2 });
    knownOptions["ao"]                      = std::make_shared<GameOption>(true,  "Smooth lighting (Ambient Occlusion)");
    knownOptions["prioritizeChunkUpdates"]  = std::make_shared<GameOption>(0,     "Chunk section update strategy", Range<int>{ 0, 2 });
    knownOptions["biomeBlendRadius"]        = std::make_shared<GameOption>(2,     "Radius for which biome blending should happen", Range<int>{ 0, 7 });
    knownOptions["renderClouds"]            = std::make_shared<GameOption>("true", "Whether to display clouds", QStringList{ "true", "false", "fast" });

    knownOptions["resourcePacks"]           = std::make_shared<GameOption>("[]",  "A list of active resource packs", QStringList(), false);
    knownOptions["incompatibleResourcePacks"] = std::make_shared<GameOption>("[]", "A list of active resource packs that are marked as incompatible with the current Minecraft version.", QStringList(), false);

    knownOptions["lastServer"]              = std::make_shared<GameOption>("",    "Address of last server used with Direct Connection");
    knownOptions["lang"]                    = std::make_shared<GameOption>("en_us", "Language to be used");
    knownOptions["soundDevice"]             = std::make_shared<GameOption>("",    "Sound device to be used");
    knownOptions["chatVisibility"]          = std::make_shared<GameOption>(0,     "What is seen in chat", Range<int>{ 0, 2 });
    knownOptions["chatOpacity"]             = std::make_shared<GameOption>(1.0,   "Opacity of the chat", Range<float>{ 0, 1 });
    knownOptions["chatLineSpacing"]         = std::make_shared<GameOption>(0.0,   "Spacing between text in chat", Range<float>{ 0, 1 });
    knownOptions["textBackgroundOpacity"]   = std::make_shared<GameOption>(0.5,   "Opacity of text background", Range<float>{ 0, 1 });
    knownOptions["backgroundForChatOnly"]   = std::make_shared<GameOption>(true,  "Toggles if the background is only in chat or if it's everywhere");
    knownOptions["hideServerAddress"]       = std::make_shared<GameOption>(false, "Has no effect in modern versions");
    knownOptions["advancedItemTooltips"]    = std::make_shared<GameOption>(false, "Whether hovering over items in the inventory shows its ID and durability; toggled by pressing F3 + H");
    knownOptions["pauseOnLostFocus"]        = std::make_shared<GameOption>(true,  "Whether switching out of Minecraft without pressing Esc or opening an in-game interface automatically pauses the game; toggled by pressing F3 + P");
    knownOptions["overrideWidth"]           = std::make_shared<GameOption>(0,     "Width to open Minecraft with in pixels (0 means default to the Minecraft settings); no in-game control");
    knownOptions["overrideHeight"]          = std::make_shared<GameOption>(0,     "Height to open Minecraft with in pixels (0 means default to the Minecraft settings); no in-game control");
    knownOptions["heldItemTooltips"]        = std::make_shared<GameOption>(true,  "Whether switching between items shows the name of the item; no in-game control");
    knownOptions["chatHeightFocused"]       = std::make_shared<GameOption>(1.0,   "How tall the chat span is", Range<float>{ 0, 1 });
    knownOptions["chatDelay"]               = std::make_shared<GameOption>(0.0,   "How much delay there is between text", Range<float>{ 0, 6 });
    knownOptions["chatHeightUnfocused"]     = std::make_shared<GameOption>(0.4375, "How tall the maximum chat span is, when the chat button is not pressed", Range<float>{ 0, 1 });
    knownOptions["chatScale"]               = std::make_shared<GameOption>(1.0,   "The scale/size of the text in the chat", Range<float>{ 0, 1 });
    knownOptions["chatWidth"]               = std::make_shared<GameOption>(1.0,   "The span width of the chat", Range<float>{ 0, 1 });
    knownOptions["mipmapLevels"]            = std::make_shared<GameOption>(4,     "Amount by which distant textures are smoothed", Range<int>{0, 4});
    knownOptions["useNativeTransport"]      = std::make_shared<GameOption>(true,  "Whether to use a Netty EpollSocketChannel for connections to servers instead of a NioSocketChannel (only applies if EPOLL is available on the user's system)");
    knownOptions["mainHand"]                = std::make_shared<GameOption>("right", "Whether the main hand appears as left or right", QStringList{ "left", "right" });
    knownOptions["attackIndicator"]         = std::make_shared<GameOption>(1,     "When hitting, how the attack indicator is shown on screen", Range<int>{ 0, 2 });
    knownOptions["narrator"]                = std::make_shared<GameOption>(0,     "Setting of the Narrator", Range<int>{ 0, 3 });
    knownOptions["tutorialStep"]            = std::make_shared<GameOption>("movement", "Next stage of tutorial hints to display",
             QStringList{ "movement", "find_tree", "punch_tree", "open_inventory", "craft_planks", "none" });
    knownOptions["mouseWheelSensitivity"]   = std::make_shared<GameOption>(1.0f,  "Allows making the mouse wheel more sensitive (see MC-123773)", Range<float>{ 1.0f, 10.0f });
    knownOptions["rawMouseInput"]           = std::make_shared<GameOption>(true,  "Ignores acceleration set by the operating system");
    knownOptions["glDebugVerbosity"]        = std::make_shared<GameOption>(1,     "LWJGL log info level (only on some machines)", Range<int>{ 0, 4 });
    knownOptions["skipMultiplayerWarning"]  = std::make_shared<GameOption>(true,  "Whether to skip the legal disclaimer when entering the multiplayer screen");
    knownOptions["skipRealms32bitWarning"]  = std::make_shared<GameOption>(true,  "Whether to skip the 32-bit environment warning when entering the Realms screen");
    knownOptions["hideMatchedNames"]        = std::make_shared<GameOption>(true,  "Some servers send chat messages in non-standard formats. With this option on, the game will attempt to apply chat hiding anyway by matching the text in messages.");
    knownOptions["joinedFirstServer"]       = std::make_shared<GameOption>(true,  "Whether the player has joined a server before. If false, the Social Interactions tutorial hint will appear when joining a server.");
    knownOptions["hideBundleTutorial"]      = std::make_shared<GameOption>(true,  "Whether the player has seen the bundle tutorial hint when trying to use a bundle.");
    knownOptions["syncChunkWrites"]         = std::make_shared<GameOption>(true,  "Whether to open region files in synchronous mode");
    knownOptions["showAutosaveIndicator"]   = std::make_shared<GameOption>(true,  "Whether to show autosave indicator on the right-bottom of the screen");
    knownOptions["allowServerListing"]      = std::make_shared<GameOption>(true,  "Whether to allow player's ID to be shown in the player list shown on the multiplayer screen");

    // Keys Binds
    knownOptions["key_key.attack"]              = std::make_shared<GameOption>("key.mouse.left",      "Attack control");
    knownOptions["key_key.use"]                 = std::make_shared<GameOption>("key.mouse.right",     "Use Item control");
    knownOptions["key_key.forward"]             = std::make_shared<GameOption>("key.keyboard.w",      "Forward control ");
    knownOptions["key_key.left"]                = std::make_shared<GameOption>("key.keyboard.a",      "Left control");
    knownOptions["key_key.back"]                = std::make_shared<GameOption>("key.keyboard.s",      "Back control");
    knownOptions["key_key.right"]               = std::make_shared<GameOption>("key.keyboard.d",      "Right control");
    knownOptions["key_key.jump"]                = std::make_shared<GameOption>("key.keyboard.space",  "Jump control");
    knownOptions["key_key.sneak"]               = std::make_shared<GameOption>("key.keyboard.left.shift", "Sneak control");
    knownOptions["key_key.sprint"]              = std::make_shared<GameOption>("key.keyboard.left.control", "Sprint control ");
    knownOptions["key_key.drop"]                = std::make_shared<GameOption>("key.keyboard.q",      "Drop control ");
    knownOptions["key_key.inventory"]           = std::make_shared<GameOption>("key.keyboard.e",      "Inventory control");
    knownOptions["key_key.chat"]                = std::make_shared<GameOption>("key.keyboard.t",      "Chat control");
    knownOptions["key_key.playerlist"]          = std::make_shared<GameOption>("key.keyboard.tab",    "List Players control");
    knownOptions["key_key.pickItem"]            = std::make_shared<GameOption>("key.mouse.middle",    "Pick Block control");
    knownOptions["key_key.command"]             = std::make_shared<GameOption>("key.keyboard.slash",  "Command control");
    knownOptions["key_key.socialInteractions"]  = std::make_shared<GameOption>("key.keyboard.p",      "Social Interaction control");
    knownOptions["key_key.screenshot"]          = std::make_shared<GameOption>("key.keyboard.f2",     "Screenshot control");
    knownOptions["key_key.togglePerspective"]   = std::make_shared<GameOption>("key.keyboard.f5",     "Perspective control");
    knownOptions["key_key.smoothCamera"]        = std::make_shared<GameOption>("key.keyboard.unknown", "Mouse Smoothing control");
    knownOptions["key_key.fullscreen"]          = std::make_shared<GameOption>("key.keyboard.f11",    "Fullscreen control");
    knownOptions["key_key.spectatorOutlines"]   = std::make_shared<GameOption>("key.keyboard.unknown", "Visibility of player outlines in Spectator Mode control");
    knownOptions["key_key.swapOffhand"]         = std::make_shared<GameOption>("key.keyboard.f",      "Swapping of items between both hands control");
    knownOptions["key_key.saveToolbarActivator"] = std::make_shared<GameOption>("key.keyboard.c",     "Save current toolbar to a slot (in Creative Mode)");
    knownOptions["key_key.loadToolbarActivator"] = std::make_shared<GameOption>("key.keyboard.x",     "Load toolbar from a slot (in Creative Mode)");
    knownOptions["key_key.advancements"]        = std::make_shared<GameOption>("key.keyboard.l",      "Open the Advancements screen");
    knownOptions["key_key.hotbar.1"]            = std::make_shared<GameOption>("key.keyboard.1",      "Hotbar Slot 1 control");
    knownOptions["key_key.hotbar.2"]            = std::make_shared<GameOption>("key.keyboard.2",      "Hotbar Slot 2 control");
    knownOptions["key_key.hotbar.3"]            = std::make_shared<GameOption>("key.keyboard.3",      "Hotbar Slot 3 control");
    knownOptions["key_key.hotbar.4"]            = std::make_shared<GameOption>("key.keyboard.4",      "Hotbar Slot 4 control");
    knownOptions["key_key.hotbar.5"]            = std::make_shared<GameOption>("key.keyboard.5",      "Hotbar Slot 5 control");
    knownOptions["key_key.hotbar.6"]            = std::make_shared<GameOption>("key.keyboard.6",      "Hotbar Slot 6 control");
    knownOptions["key_key.hotbar.7"]            = std::make_shared<GameOption>("key.keyboard.7",      "Hotbar Slot 7 control");
    knownOptions["key_key.hotbar.8"]            = std::make_shared<GameOption>("key.keyboard.8",      "Hotbar Slot 8 control");
    knownOptions["key_key.hotbar.9"]            = std::make_shared<GameOption>("key.keyboard.9",      "Hotbar Slot 9 control");

    // Sound
    knownOptions["soundCategory_master"]    = std::make_shared<GameOption>(1.0f,  "The volume of all sounds", Range<float>{ 0, 1 });
    knownOptions["soundCategory_music"]     = std::make_shared<GameOption>(1.0f,  "The volume of gameplay music", Range<float>{ 0, 1 });
    knownOptions["soundCategory_record"]    = std::make_shared<GameOption>(1.0f,  "The volume of music/sounds from Jukeboxes and Note Blocks", Range<float>{ 0, 1 });
    knownOptions["soundCategory_weather"]   = std::make_shared<GameOption>(1.0f,  "The volume of rain and thunder", Range<float>{ 0, 1 });
    knownOptions["soundCategory_block"]     = std::make_shared<GameOption>(1.0f,  "The volume of blocks", Range<float>{ 0, 1 });
    knownOptions["soundCategory_hostile"]   = std::make_shared<GameOption>(1.0f,  "The volume of hostile and neutral mobs", Range<float>{ 0, 1 });
    knownOptions["soundCategory_neutral"]   = std::make_shared<GameOption>(1.0f,  "The volume of passive mobs", Range<float>{ 0, 1 });
    knownOptions["soundCategory_player"]    = std::make_shared<GameOption>(1.0f,  "The volume of players", Range<float>{ 0, 1 });
    knownOptions["soundCategory_ambient"]   = std::make_shared<GameOption>(1.0f,  "The volume of cave sounds and fireworks", Range<float>{ 0, 1 });
    knownOptions["soundCategory_voice"]     = std::make_shared<GameOption>(1.0f,  "The volume of voices", Range<float>{ 0, 1 });

    // Model Parts
    knownOptions["modelPart_cape"]              = std::make_shared<GameOption>(true,  "Whether the cape is shown");
    knownOptions["modelPart_jacket"]            = std::make_shared<GameOption>(true,  "Whether the \"Jacket\" skin layer is shown");
    knownOptions["modelPart_left_sleeve"]       = std::make_shared<GameOption>(true,  "Whether the \"Left Sleeve\" skin layer is shown");
    knownOptions["modelPart_right_sleeve"]      = std::make_shared<GameOption>(true,  "Whether the \"Right Sleeve\" skin layer is shown");
    knownOptions["modelPart_left_pants_leg"]    = std::make_shared<GameOption>(true,  "Whether the \"Left Pants Leg\" skin layer is shown");
    knownOptions["modelPart_right_pants_leg"]   = std::make_shared<GameOption>(true,  "Whether the \"Right Pants Leg\" skin layer is shown");
    knownOptions["modelPart_hat"]               = std::make_shared<GameOption>(true,  "Whether the \"Hat\" skin layer is shown");

    // Misc
    knownOptions["fullscreenResolution"]        = std::make_shared<GameOption>("", "Changes the resolution of the game when in fullscreen mode. The only values allowed are the values supported by the user's monitor, shown when changing the screen resolution in the operating system settings. Setting this option to a value not supported by the monitor resets the option to \"Current\". When set to \"Current\", this option is absent from options.txt. ");

    // Not vanilla - from mods or modloaders
    knownOptions["quilt_available_resourcepacks"] = std::make_shared<GameOption>("[]", "Quilt Internal Available Resource Packs", QStringList(), false);

}

/// @brief this data is mostly copied from https://minecraft.fandom.com/wiki/Key_codes
void GameOptionsSchema::populateInternalKeyBindList() {
    // TODO: add numeric (GLFW) key codes

    // Unboud
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.unknown", 0, QObject::tr("Not bound"), Qt::Key_unknown )));  // 4th value is Qt key code

    // Mouse - legacy keybindings only go up to 16 for mice, represented as: -100 + LWJGL Code
    addMouseBind(   "key.mouse.left",                   -100,   QObject::tr("Left Mouse Button"),   Qt::MouseButton::LeftButton );
    addMouseBind(   "key.mouse.right",                  -99,    QObject::tr("Right Mouse Button"),  Qt::MouseButton::RightButton );
    addMouseBind(   "key.mouse.middle",                 -98,    QObject::tr("Middle Mouse Button"), Qt::MouseButton::MiddleButton );
    addMouseBind(   "key.mouse.4",                      -97,    QObject::tr("Mouse Button 4"),      Qt::MouseButton::BackButton );
    addMouseBind(   "key.mouse.5",                      -96,    QObject::tr("Mouse Button 5"),      Qt::MouseButton::ExtraButton1 );
    addMouseBind(   "key.mouse.7",                      -95,    QObject::tr("Mouse Button 7"),      Qt::MouseButton::ExtraButton3 );
    addMouseBind(   "key.mouse.8",                      -94,    QObject::tr("Mouse Button 8"),      Qt::MouseButton::ExtraButton4 );
    addMouseBind(   "key.mouse.9",                      -93,    QObject::tr("Mouse Button 9"),      Qt::MouseButton::ExtraButton5 );
    addMouseBind(   "key.mouse.6",                      -92,    QObject::tr("Mouse Button 6"),      Qt::MouseButton::ExtraButton2 );
    addMouseBind(   "key.mouse.10",                     -91,    QObject::tr("Mouse Button 10"),     Qt::MouseButton::ExtraButton6 );
    addMouseBind(   "key.mouse.11",                     -90,    QObject::tr("Mouse Button 11"),     Qt::MouseButton::ExtraButton7 );
    addMouseBind(   "key.mouse.12",                     -89,    QObject::tr("Mouse Button 12"),     Qt::MouseButton::ExtraButton8 );
    addMouseBind(   "key.mouse.13",                     -88,    QObject::tr("Mouse Button 13"),     Qt::MouseButton::ExtraButton9 );
    addMouseBind(   "key.mouse.14",                     -87,    QObject::tr("Mouse Button 14"),     Qt::MouseButton::ExtraButton10 );
    addMouseBind(   "key.mouse.15",                     -86,    QObject::tr("Mouse Button 15"),     Qt::MouseButton::ExtraButton11 );
    addMouseBind(   "key.mouse.16",                     -85,    QObject::tr("Mouse Button 16"),     Qt::MouseButton::ExtraButton12 );
    addMouseBind(   "key.mouse.17",                     0,      QObject::tr("Mouse Button 17"),     Qt::MouseButton::ExtraButton13 );
    addMouseBind(   "key.mouse.18",                     0,      QObject::tr("Mouse Button 18"),     Qt::MouseButton::ExtraButton14 );
    addMouseBind(   "key.mouse.19",                     0,      QObject::tr("Mouse Button 19"),     Qt::MouseButton::ExtraButton15 );
    addMouseBind(   "key.mouse.20",                     0,      QObject::tr("Mouse Button 20"),     Qt::MouseButton::ExtraButton16 );
    addMouseBind(   "key.mouse.21",                     0,      QObject::tr("Mouse Button 21"),     Qt::MouseButton::ExtraButton17 );
    addMouseBind(   "key.mouse.22",                     0,      QObject::tr("Mouse Button 22"),     Qt::MouseButton::ExtraButton18 );
    addMouseBind(   "key.mouse.23",                     0,      QObject::tr("Mouse Button 23"),     Qt::MouseButton::ExtraButton19 );
    addMouseBind(   "key.mouse.24",                     0,      QObject::tr("Mouse Button 24"),     Qt::MouseButton::ExtraButton20 );
    addMouseBind(   "key.mouse.25",                     0,      QObject::tr("Mouse Button 25"),     Qt::MouseButton::ExtraButton21 );
    addMouseBind(   "key.mouse.26",                     0,      QObject::tr("Mouse Button 26"),     Qt::MouseButton::ExtraButton22 );
    addMouseBind(   "key.mouse.27",                     0,      QObject::tr("Mouse Button 27"),     Qt::MouseButton::ExtraButton23 );
    addMouseBind(   "key.mouse.28",                     0,      QObject::tr("Mouse Button 28"),     Qt::MouseButton::ExtraButton24 );
    // not sure if it makes sense to go this far, but it's how far Qt can count.
    
    // Number Row
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.0", 0, QObject::tr("0"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.1", 0, QObject::tr("1"), Qt::Key::Key_1 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.2", 0, QObject::tr("2"), Qt::Key::Key_2 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.3", 0, QObject::tr("3"), Qt::Key::Key_3 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.4", 0, QObject::tr("4"), Qt::Key::Key_4 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.5", 0, QObject::tr("5"), Qt::Key::Key_5 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.6", 0, QObject::tr("6"), Qt::Key::Key_6 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.7", 0, QObject::tr("7"), Qt::Key::Key_7 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.8", 0, QObject::tr("8"), Qt::Key::Key_8 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.9", 0, QObject::tr("9"), Qt::Key::Key_9 )));

    // Letters
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.a", 0, QObject::tr("a"), Qt::Key::Key_A )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.b", 0, QObject::tr("b"), Qt::Key::Key_B )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.c", 0, QObject::tr("c"), Qt::Key::Key_C )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.d", 0, QObject::tr("d"), Qt::Key::Key_D )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.e", 0, QObject::tr("e"), Qt::Key::Key_E )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f", 0, QObject::tr("f"), Qt::Key::Key_F )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.g", 0, QObject::tr("g"), Qt::Key::Key_G )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.h", 0, QObject::tr("h"), Qt::Key::Key_H )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.i", 0, QObject::tr("i"), Qt::Key::Key_I )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.j", 0, QObject::tr("j"), Qt::Key::Key_J )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.k", 0, QObject::tr("k"), Qt::Key::Key_K )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.l", 0, QObject::tr("l"), Qt::Key::Key_L )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.m", 0, QObject::tr("m"), Qt::Key::Key_M )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.n", 0, QObject::tr("n"), Qt::Key::Key_N )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.o", 0, QObject::tr("o"), Qt::Key::Key_O )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.p", 0, QObject::tr("p"), Qt::Key::Key_P )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.q", 0, QObject::tr("q"), Qt::Key::Key_Q )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.r", 0, QObject::tr("r"), Qt::Key::Key_R )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.s", 0, QObject::tr("s"), Qt::Key::Key_S )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.t", 0, QObject::tr("t"), Qt::Key::Key_T )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.u", 0, QObject::tr("u"), Qt::Key::Key_U )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.v", 0, QObject::tr("v"), Qt::Key::Key_V )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.w", 0, QObject::tr("w"), Qt::Key::Key_W )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.x", 0, QObject::tr("x"), Qt::Key::Key_X )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.y", 0, QObject::tr("y"), Qt::Key::Key_Y )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.z", 0, QObject::tr("z"), Qt::Key::Key_Z )));

    // F Keys
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f1", 0, QObject::tr("F1"), Qt::Key::Key_F1 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f2", 0, QObject::tr("F2"), Qt::Key::Key_F2 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f3", 0, QObject::tr("F3"), Qt::Key::Key_F3 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f4", 0, QObject::tr("F4"), Qt::Key::Key_F4 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f5", 0, QObject::tr("F5"), Qt::Key::Key_F5 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f6", 0, QObject::tr("F6"), Qt::Key::Key_F6 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f7", 0, QObject::tr("F7"), Qt::Key::Key_F7 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f8", 0, QObject::tr("F8"), Qt::Key::Key_F8 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f9", 0, QObject::tr("F9"), Qt::Key::Key_F9 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f10", 0, QObject::tr("F10"), Qt::Key::Key_F10 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f11", 0, QObject::tr("F11"), Qt::Key::Key_F11 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f12", 0, QObject::tr("F12"), Qt::Key::Key_F12 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f13", 0, QObject::tr("F13"), Qt::Key::Key_F13 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f14", 0, QObject::tr("F14"), Qt::Key::Key_F14 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f15", 0, QObject::tr("F15"), Qt::Key::Key_F15 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f16", 0, QObject::tr("F16"), Qt::Key::Key_F16 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f17", 0, QObject::tr("F17"), Qt::Key::Key_F17 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f18", 0, QObject::tr("F18"), Qt::Key::Key_F18 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f19", 0, QObject::tr("F19"), Qt::Key::Key_F19 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f20", 0, QObject::tr("F20"), Qt::Key::Key_F20 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f21", 0, QObject::tr("F21"), Qt::Key::Key_F21 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f22", 0, QObject::tr("F22"), Qt::Key::Key_F22 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f23", 0, QObject::tr("F23"), Qt::Key::Key_F23 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f24", 0, QObject::tr("F24"), Qt::Key::Key_F24 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.f25", 0, QObject::tr("F25"), Qt::Key::Key_F25 )));

    // Numblock
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.num.lock", 0, QObject::tr("Num Lock"), Qt::Key::Key_NumLock )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.0", 0, QObject::tr("Keypad 0"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.1", 0, QObject::tr("Keypad 1"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.2", 0, QObject::tr("Keypad 2"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.3", 0, QObject::tr("Keypad 3"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.4", 0, QObject::tr("Keypad 4"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.5", 0, QObject::tr("Keypad 5"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.6", 0, QObject::tr("Keypad 6"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.7", 0, QObject::tr("Keypad 7"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.8", 0, QObject::tr("Keypad 8"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.9", 0, QObject::tr("Keypad 9"), Qt::Key::Key_0 )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.add", 0, QObject::tr("Keypad +"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.decimal", 0, QObject::tr("Keypad Decimal"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.enter", 0, QObject::tr("Keypad Enter"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.equal", 0, QObject::tr("Keypad ="), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.multiply", 0, QObject::tr("Keypad *"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.divide", 0, QObject::tr("Keypad /"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.keypad.subtract", 0, QObject::tr("Keypad -"), Qt::Key_unknown ))); // fix maybe?

    // Arrow Keys
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.down", 0, QObject::tr("Down Arrow"), Qt::Key::Key_Down )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left", 0, QObject::tr("Left Arrow"), Qt::Key::Key_Left )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right", 0, QObject::tr("Right Arrow"), Qt::Key::Key_Right )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.up", 0, QObject::tr("Up Arrow"), Qt::Key::Key_Up )));

    // Other
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.apostrophe", 0, QObject::tr("'"), Qt::Key::Key_Apostrophe )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.backslash", 0, QObject::tr("\\"), Qt::Key::Key_Backslash )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.comma", 0, QObject::tr(","), Qt::Key::Key_Comma )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.equal", 0, QObject::tr("="), Qt::Key::Key_Equal )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.grave.accent", 0, QObject::tr("`"), Qt::Key::Key_Dead_Grave )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left.bracket", 0, QObject::tr("["), Qt::Key::Key_BracketLeft )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.minus", 0, QObject::tr("-"), Qt::Key::Key_Minus )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.period", 0, QObject::tr("."), Qt::Key::Key_Period )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right.bracket", 0, QObject::tr("]"), Qt::Key::Key_BracketRight )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.semicolon", 0, QObject::tr(";"), Qt::Key::Key_Semicolon )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.slash", 0, QObject::tr("/"), Qt::Key::Key_Slash )));

    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.space", 0, QObject::tr("Space"), Qt::Key::Key_Space )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.tab", 0, QObject::tr("Tab"), Qt::Key::Key_Tab )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left.alt", 0, QObject::tr("Left Alt"), Qt::Key::Key_Alt )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left.control", 0, QObject::tr("Left Control"), Qt::Key::Key_Control )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left.shift", 0, QObject::tr("Left Shift"), Qt::Key::Key_Shift )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.left.win", 0, QObject::tr("Left Win"), Qt::Key::Key_Meta )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right.alt", 0, QObject::tr("Right Alt"), Qt::Key::Key_AltGr )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right.control", 0, QObject::tr("Right Control"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right.shift", 0, QObject::tr("Right Shift"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.right.win", 0, QObject::tr("Right Win"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.enter", 0, QObject::tr("Enter"), Qt::Key::Key_Enter )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.escape", 0, QObject::tr("Escape"), Qt::Key::Key_Escape )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.backspace", 0, QObject::tr("Backspace"), Qt::Key::Key_Backspace )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.delete", 0, QObject::tr("Delete"), Qt::Key::Key_Delete )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.end", 0, QObject::tr("End"), Qt::Key::Key_End )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.home", 0, QObject::tr("Home"), Qt::Key::Key_Home )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.insert", 0, QObject::tr("Insert"), Qt::Key::Key_Insert )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.page.down", 0, QObject::tr("Page Down"), Qt::Key::Key_PageDown )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.page.up", 0, QObject::tr("Page Up"), Qt::Key::Key_PageUp )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.caps.lock", 0, QObject::tr("Caps Lock"), Qt::Key::Key_CapsLock )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.pause", 0, QObject::tr("Pause"), Qt::Key::Key_Pause )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.scroll.lock", 0, QObject::tr("Scroll Lock"), Qt::Key::Key_ScrollLock )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.menu", 0, QObject::tr("Menu"), Qt::Key::Key_Menu )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.print.screen", 0, QObject::tr("Print Screen"), Qt::Key::Key_Print )));
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.world.1", 0, QObject::tr("World 1"), Qt::Key_unknown ))); // fix maybe?
    keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "key.keyboard.world.2", 0, QObject::tr("World 2"), Qt::Key_unknown ))); // fix maybe?
    // keyboardButtons.append(std::shared_ptr<KeyBindData>(new KeyBindData( "scancode.###", 0, QObject::tr("scancode.###") ))); no description, no translation, no mapping?

    // clang-format on
};
QString GameOption::getDefaultString()
{ return defaultString; }
