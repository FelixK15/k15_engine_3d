/**
 * @file K15_EventTypes.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/11
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_EventTypes.h"

using namespace K15_EngineV2;

const HashedString EventTypes::KeyboardPressed = HashedString("keyboard_pressed");
const HashedString EventTypes::KeyboardReleased = HashedString("keyboard_released");
const HashedString EventTypes::MouseMoved = HashedString("mouse_moved");
const HashedString EventTypes::MousePressed = HashedString("mouse_pressed");
const HashedString EventTypes::MouseReleased = HashedString("mouse_released");

//////////////////////////////////////////////////////////////////////////
const HashedString EventTypes::FileCreated = HashedString("file_created");
const HashedString EventTypes::FileChanged = HashedString("file_changed");
const HashedString EventTypes::FileDeleted = HashedString("file_deleted");