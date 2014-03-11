/**
 * @file K15_GameEventArguments.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifndef _K15Engine_Core_Class_h_
#define _K15Engine_Core_Class_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Core { namespace GameEventArguments {
	/*********************************************************************************/
	struct K15_CORE_API MouseActionArguments
	{
		Enum button;

		uint32 xPx;	//Pixel coordinates
		uint32 yPx; //Pixel coordinates

		float xNDC; //Normal Device Coordinates ([-1|+1])
		float yNDC; //Normal Device Coordinates ([-1|+1])
	};
	/*********************************************************************************/
}}}// end of K15_Engine::Core::GameEventArguments namespace

#endif //_K15Engine_Core_Class_h_