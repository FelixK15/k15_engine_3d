/**
 * @file K15_ColorRGBA.cpp
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
 */

#include "K15_RendererPrecompiledHeader.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const ColorRGBA ColorRGBA::Transparent = ColorRGBA(1.0f,1.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::White = ColorRGBA(1.0f,1.0f,1.0f);
	const ColorRGBA ColorRGBA::Black = ColorRGBA(0.0f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::Red = ColorRGBA(1.0f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::Green = ColorRGBA(0.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::Blue = ColorRGBA(1.0f,0.0f,1.0f);
	const ColorRGBA ColorRGBA::Yellow = ColorRGBA(1.0f,1.0f,0.0f);
	const ColorRGBA ColorRGBA::Grey = ColorRGBA(0.5f,0.5f,0.5f);
	const ColorRGBA ColorRGBA::Brown = ColorRGBA(0.65f,0.27f,0.0f);
	const ColorRGBA ColorRGBA::Purple = ColorRGBA(0.5f,0.0f,0.45f);
	const ColorRGBA ColorRGBA::Pink = ColorRGBA(1.0f,0.5f,0.65f);
	const ColorRGBA ColorRGBA::DarkRed = ColorRGBA(0.5f,0.0f,0.0f);
	const ColorRGBA ColorRGBA::DarkBlue = ColorRGBA(0.0f,0.0f,0.5f);
	const ColorRGBA ColorRGBA::DarkGreen = ColorRGBA(0.0f,0.5f,0.0f);
	const ColorRGBA ColorRGBA::DarkYellow = ColorRGBA(0.5f,0.5f,0.0f);
	const ColorRGBA ColorRGBA::DarkGrey = ColorRGBA(0.25f,0.25f,0.25f);
	const ColorRGBA ColorRGBA::DarkBrown = ColorRGBA(0.43f,0.15f,0.0f);
	/*********************************************************************************/

	/*********************************************************************************/
	ColorRGBA::ColorRGBA(float p_Red, float p_Green, float p_Blue,float p_Alpha)
		: RedComponent(p_Red),
		  GreenComponent(p_Green),
		  BlueComponent(p_Blue),
		  AlphaComponent(p_Alpha)
	{
		clamp();
	}
	/*********************************************************************************/
	ColorRGBA::ColorRGBA(const ColorRGBA& p_Color)
		: RedComponent(p_Color.RedComponent),
		  GreenComponent(p_Color.GreenComponent),
		  BlueComponent(p_Color.BlueComponent),
		  AlphaComponent(p_Color.AlphaComponent)
	{
		clamp();
	}
	/*********************************************************************************/
	void ColorRGBA::clamp()
	{
		if(RedComponent > 1.0f) RedComponent = 1.0f;
		if(GreenComponent > 1.0f) GreenComponent = 1.0f;
		if(BlueComponent > 1.0f) BlueComponent = 1.0f;
		if(AlphaComponent > 1.0f) AlphaComponent = 1.0f;

		if(RedComponent < 0.0f) RedComponent = 0.0f;
		if(GreenComponent < 0.0f) GreenComponent = 0.0f;
		if(BlueComponent < 0.0f) BlueComponent = 0.0f;
		if(AlphaComponent < 0.0f) AlphaComponent = 0.0f;
	}
	/*********************************************************************************/
	bool ColorRGBA::operator==(const ColorRGBA& p_Color)
	{
		return RedComponent == p_Color.RedComponent && GreenComponent == p_Color.GreenComponent && 
			BlueComponent == p_Color.BlueComponent && AlphaComponent == p_Color.AlphaComponent;
	}
	/*********************************************************************************/
	bool ColorRGBA::operator!=(const ColorRGBA& p_Color)
	{
		return !this->operator==(p_Color);
	}
	/*********************************************************************************/
	bool ColorRGBA::operator<(const ColorRGBA& p_Color)
	{
		return RedComponent < p_Color.RedComponent && GreenComponent < p_Color.GreenComponent && 
			BlueComponent < p_Color.BlueComponent && AlphaComponent < p_Color.AlphaComponent;
	}
	/*********************************************************************************/
	bool ColorRGBA::operator>(const ColorRGBA& p_Color)
	{
		return !this->operator<(p_Color);
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator=(const ColorRGBA& p_Color)
	{
		RedComponent = p_Color.RedComponent;
		GreenComponent = p_Color.GreenComponent;
		BlueComponent = p_Color.BlueComponent;
		AlphaComponent = p_Color.AlphaComponent;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator+=(const ColorRGBA& p_Color)
	{
		RedComponent += p_Color.RedComponent;
		GreenComponent += p_Color.GreenComponent;
		BlueComponent += p_Color.BlueComponent;
		AlphaComponent += p_Color.AlphaComponent;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator+=(float p_Value)
	{
		RedComponent += p_Value;
		GreenComponent += p_Value;
		BlueComponent += p_Value;
		AlphaComponent += p_Value;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator-=(const ColorRGBA& p_Color)
	{
		RedComponent -= p_Color.RedComponent;
		GreenComponent -= p_Color.GreenComponent;
		BlueComponent -= p_Color.BlueComponent;
		AlphaComponent -= p_Color.AlphaComponent;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator-=(float p_Value)
	{
		RedComponent -= p_Value;
		GreenComponent -= p_Value;
		BlueComponent -= p_Value;
		AlphaComponent -= p_Value;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator*=(const ColorRGBA& p_Color)
	{
		RedComponent *= p_Color.RedComponent;
		GreenComponent *= p_Color.GreenComponent;
		BlueComponent *= p_Color.BlueComponent;
		AlphaComponent *= p_Color.AlphaComponent;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator*=(float p_Value)
	{
		RedComponent *= p_Value;
		GreenComponent *= p_Value;
		BlueComponent *= p_Value;
		AlphaComponent *= p_Value;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator/=(const ColorRGBA& p_Color)
	{
		RedComponent /= p_Color.RedComponent;
		GreenComponent /= p_Color.GreenComponent;
		BlueComponent /= p_Color.BlueComponent;
		AlphaComponent /= p_Color.AlphaComponent;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator/=(float p_Value)
	{
		RedComponent /= p_Value;
		GreenComponent /= p_Value;
		BlueComponent /= p_Value;
		AlphaComponent /= p_Value;

		clamp();

		return *this;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator+(const ColorRGBA& p_Color)
	{
		ColorRGBA color(*this);

		color += p_Color;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator+(float p_Value)
	{
		ColorRGBA color(*this);

		color += p_Value;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator-(const ColorRGBA& p_Color)
	{
		ColorRGBA color(*this);

		color -= p_Color;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator-(float p_Value)
	{
		ColorRGBA color(*this);

		color -= p_Value;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator*(const ColorRGBA& p_Color)
	{
		ColorRGBA color(*this);

		color *= p_Color;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator*(float p_Value)
	{
		ColorRGBA color(*this);

		color *= p_Value;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator/(const ColorRGBA& p_Color)
	{
		ColorRGBA color(*this);

		color /= p_Color;

		return color;
	}
	/*********************************************************************************/
	ColorRGBA ColorRGBA::operator/(float p_Value)
	{
		ColorRGBA color(*this);

		color /= p_Value;

		return color;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace