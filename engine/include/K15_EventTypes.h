/**
 * @file K15_EventTypes.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_EVENTTYPES__
#define __K15_EVENTTYPES__

#include "K15_StdInclude.h"
#include "K15_HashedString.h"

namespace K15_EngineV2
{
	class K15_API_EXPORT EventTypes
	{
	public:
		static const HashedString KeyboardPressed;
		static const HashedString KeyboardReleased;
		static const HashedString MouseMoved;
		static const HashedString MousePressed;
		static const HashedString MouseReleased;

		static const HashedString FileCreated;
		static const HashedString FileChanged;
		static const HashedString FileDeleted;
	};
}

#endif //__K15_EVENTTYPES__