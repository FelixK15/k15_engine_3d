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

#include "K15_PrecompiledHeader.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const ColorRGBA ColorRGBA::Transparent = ColorRGBA(255,255,255,0);
	const ColorRGBA ColorRGBA::White = ColorRGBA(255,255,255);
	const ColorRGBA ColorRGBA::Black = ColorRGBA(0,0,0);
	const ColorRGBA ColorRGBA::Red = ColorRGBA(255,0,0);
	const ColorRGBA ColorRGBA::Green = ColorRGBA(0,255,0);
	const ColorRGBA ColorRGBA::Blue = ColorRGBA(255,0,255);
	const ColorRGBA ColorRGBA::Yellow = ColorRGBA(255,255,0);
	const ColorRGBA ColorRGBA::Grey = ColorRGBA(127,127,127);
	const ColorRGBA ColorRGBA::Brown = ColorRGBA(140,75,0);
	const ColorRGBA ColorRGBA::Purple = ColorRGBA(127,0,115);
	const ColorRGBA ColorRGBA::Pink = ColorRGBA(255,127,135);
	const ColorRGBA ColorRGBA::DarkRed = ColorRGBA(127,0,0);
	const ColorRGBA ColorRGBA::DarkBlue = ColorRGBA(0,0,127);
	const ColorRGBA ColorRGBA::DarkGreen = ColorRGBA(0,127,0);
	const ColorRGBA ColorRGBA::DarkYellow = ColorRGBA(127,127,0);
	const ColorRGBA ColorRGBA::DarkGrey = ColorRGBA(64,64,64);
	const ColorRGBA ColorRGBA::DarkBrown = ColorRGBA(54,40,0);
	/*********************************************************************************/

	/*********************************************************************************/
	ColorRGBA::ColorRGBA(byte p_Red, byte p_Green, byte p_Blue,byte p_Alpha)
		: R(p_Red),
		  G(p_Green),
		  B(p_Blue),
		  A(p_Alpha)
	{

	}
	/*********************************************************************************/
	bool ColorRGBA::operator==(const ColorRGBA& p_Color) const
	{
		return R == p_Color.R && G == p_Color.G && 
			B == p_Color.B && A == p_Color.A;
	}
	/*********************************************************************************/
	bool ColorRGBA::operator!=(const ColorRGBA& p_Color) const
	{
		return !this->operator==(p_Color);
	}
	/*********************************************************************************/
	bool ColorRGBA::operator<(const ColorRGBA& p_Color) const
	{
		return R < p_Color.R && G < p_Color.G && 
			B < p_Color.B && A < p_Color.A;
	}
	/*********************************************************************************/
	bool ColorRGBA::operator>(const ColorRGBA& p_Color) const
	{
		return !this->operator<(p_Color);
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator+=(const ColorRGBA& p_Color)
	{
		R = p_Color.R + R > 255 ? 255 : p_Color.R + R;
		R = p_Color.G + G > 255 ? 255 : p_Color.G + G;
		R = p_Color.B + B > 255 ? 255 : p_Color.B + B;
		R = p_Color.A + A > 255 ? 255 : p_Color.A + A;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator+=(byte p_Value)
	{
		R = p_Value + R > 255 ? 255 : R + p_Value;
		G = p_Value + G > 255 ? 255 : G + p_Value;
		B = p_Value + B > 255 ? 255 : B + p_Value;
		A = p_Value + A > 255 ? 255 : A + p_Value;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator-=(const ColorRGBA& p_Color)
	{
		R = R - p_Color.R < 0 ? 0 : R - p_Color.R;
		R = G - p_Color.G < 0 ? 0 : G - p_Color.G;
		R = B - p_Color.B < 0 ? 0 : B - p_Color.B;
		R = A - p_Color.A < 0 ? 0 : A - p_Color.A;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator-=(byte p_Value)
	{
		R = R - p_Value < 0 ? 0 : R - p_Value;
		R = G - p_Value < 0 ? 0 : G - p_Value;
		R = B - p_Value < 0 ? 0 : B - p_Value;
		R = A - p_Value < 0 ? 0 : A - p_Value;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator*=(const ColorRGBA& p_Color)
	{
		R = R * p_Color.R > 255 ? 255 : R * p_Color.R;
		G = G * p_Color.G > 255 ? 255 : G * p_Color.G;
		B = B * p_Color.B > 255 ? 255 : B * p_Color.B;
		A = A * p_Color.A > 255 ? 255 : A * p_Color.A;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator*=(byte p_Value)
	{
		R = R * p_Value > 255 ? 255 : R * p_Value;
		G = G * p_Value > 255 ? 255 : G * p_Value;
		B = B * p_Value > 255 ? 255 : B * p_Value;
		A = A * p_Value > 255 ? 255 : A * p_Value;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator/=(const ColorRGBA& p_Color)
	{
		R /= p_Color.R;
		G /= p_Color.G;
		B /= p_Color.B;
		A /= p_Color.A;

		return *this;
	}
	/*********************************************************************************/
	const ColorRGBA& ColorRGBA::operator/=(byte p_Value)
	{
		R /= p_Value;
		G /= p_Value;
		B /= p_Value;
		A /= p_Value;

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
	ColorRGBA ColorRGBA::operator+(byte p_Value)
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
	ColorRGBA ColorRGBA::operator-(byte p_Value)
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
	ColorRGBA ColorRGBA::operator*(byte p_Value)
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
	ColorRGBA ColorRGBA::operator/(byte p_Value)
	{
		ColorRGBA color(*this);

		color /= p_Value;

		return color;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace