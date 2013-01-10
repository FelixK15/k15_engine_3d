/**
 * @file K15_EventListener.h
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

#ifndef __K15_EVENTLISTENER__
#define __K15_EVENTLISTENER__

#include "K15_StdInclude.h"
#include "K15_Event.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API EventListener
	{
	public:
		virtual void HandleEvent(const Event &evGameEvent) = 0;
	};
}

#endif //__K15_EVENTLISTENER__