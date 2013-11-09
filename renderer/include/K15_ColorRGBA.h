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
	struct ColorRGBA 
	{
		/*********************************************************************************/
		static const ColorRGBA Transparent;
		static const ColorRGBA White;
		static const ColorRGBA Black;
		static const ColorRGBA Red;
		static const ColorRGBA Blue;
		static const ColorRGBA Green;
		static const ColorRGBA Yellow;
		static const ColorRGBA Grey;
		static const ColorRGBA Brown;
		static const ColorRGBA Purple;
		static const ColorRGBA Pink;
		static const ColorRGBA DarkRed;
		static const ColorRGBA LightRed;
		static const ColorRGBA DarkBlue;
		static const ColorRGBA LightBlue;
		static const ColorRGBA DarkGreen;
		static const ColorRGBA LightGreen;
		static const ColorRGBA LightYellow;
		static const ColorRGBA DarkYellow;
		static const ColorRGBA DarkGrey;
		static const ColorRGBA LightGrey;
		static const ColorRGBA DarkBrown;
		static const ColorRGBA LightBrown;
		/*********************************************************************************/

		ColorRGBA(float p_Red = 0.0f,float p_Green = 0.0f,float p_Blue = 0.0f,float p_Alpha = 1.0f);
		ColorRGBA(const ColorRGBA& p_Color);

		void clamp();

		bool operator==(const ColorRGBA& p_Color);
		bool operator!=(const ColorRGBA& p_Color);

		bool operator<(const ColorRGBA& p_Color);
		bool operator>(const ColorRGBA& p_Color);

		const ColorRGBA& operator=(const ColorRGBA& p_Color);

		const ColorRGBA& operator+=(const ColorRGBA& p_Color);
		const ColorRGBA& operator+=(float p_Value);

		const ColorRGBA& operator-=(const ColorRGBA& p_Color);
		const ColorRGBA& operator-=(float p_Value);

		const ColorRGBA& operator*=(const ColorRGBA& p_Color);
		const ColorRGBA& operator*=(float p_Value);

		const ColorRGBA& operator/=(const ColorRGBA& p_Color);
		const ColorRGBA& operator/=(float p_Value);

		ColorRGBA operator+(const ColorRGBA& p_Color);
		ColorRGBA operator+(float p_Value);

		ColorRGBA operator-(const ColorRGBA& p_Color);
		ColorRGBA operator-(float p_Value);

		ColorRGBA operator*(const ColorRGBA& p_Color);
		ColorRGBA operator*(float p_Value);

		ColorRGBA operator/(const ColorRGBA& p_Color);
		ColorRGBA operator/(float p_Value);

		float RedComponent;
		float GreenComponent;
		float BlueComponent;
		float AlphaComponent;
	};// end of Color struct
}}//end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_Color_h_