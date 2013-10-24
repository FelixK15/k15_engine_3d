/**
 * @file K15_Color.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#ifndef _K15Engine_Renderer_Color_h_
#define _K15Engine_Renderer_Color_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

namespace K15_Engine { namespace Rendering {
	struct Color 
	{
		/*********************************************************************************/
		static Color Red;
		static Color Blue;
		static Color Green;
		static Color Yellow;
		static Color Grey;
		static Color Brown;
		static Color Purple;
		static Color Pink;
		static Color DarkRed;
		static Color LightRed;
		static Color DarkBlue;
		static Color LightBlue;
		static Color DarkGreen;
		static Color LightGreen;
		static Color LightYellow;
		static Color DarkYellow;
		static Color DarkGrey;
		static Color LightGrey;
		static Color DarkBrown;
		static Color LightBrown;
		/*********************************************************************************/
	};// end of Color struct
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_Color_h_