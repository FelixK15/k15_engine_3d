/**
 * @file K15_ColorRGB.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#include "K15_ColorRGB.h"
using namespace K15_Renderer;

ColorRGB::ColorRGB()
{
	R = G = B = 0.f;
}

ColorRGB::ColorRGB(float R,float G,float B)
{
	this->R = R;
	this->G = G;
	this->B = B;
}

ColorRGB::ColorRGB(const ColorRGB &clr)
{
	this->R = clr.R;
	this->G = clr.G;
	this->B = clr.B;
}

const ColorRGB &ColorRGB::operator=(const ColorRGB &clr)
{
	this->R = clr.R;
	this->G = clr.G;
	this->B = clr.B;
	return *this;
}

const ColorRGB &ColorRGB::operator+(const ColorRGB &clr)
{
	R += clr.R;
	G += clr.B;
	B += clr.G;

	_ClampValues();
	return *this;
}

const ColorRGB &ColorRGB::operator-(const ColorRGB &clr)
{
	R -= clr.R;
	G -= clr.B;
	B -= clr.G;

	_ClampValues();
	return *this;
}

const ColorRGB &ColorRGB::operator*(const ColorRGB &clr)
{
	R *= clr.R;
	G *= clr.B;
	B *= clr.G;

	_ClampValues();
	return *this;
}

const ColorRGB &ColorRGB::operator/(const ColorRGB &clr)
{
	R /= clr.R;
	G /= clr.B;
	B /= clr.G;

	_ClampValues();
	return *this;
}

const ColorRGB &ColorRGB::operator*(float fScalar)
{
	R *= fScalar;
	G *= fScalar;
	B *= fScalar;

	_ClampValues();
	return *this;
}

const ColorRGB &ColorRGB::operator/(float fScalar)
{
	R /= fScalar;
	G /= fScalar;
	B /= fScalar;

	_ClampValues();
	return *this;
}

void ColorRGB::operator+=(const ColorRGB &clr)
{
	operator+(clr);
}

void ColorRGB::operator-=(const ColorRGB &clr)
{
	operator-(clr);
}

void ColorRGB::operator*=(const ColorRGB &clr)
{
	operator*(clr);
}

void ColorRGB::operator/=(const ColorRGB &clr)
{
	operator/(clr);
}

void ColorRGB::operator*=(float fScalar)
{
	operator*(fScalar);
}

void ColorRGB::operator/=(float fScalar)
{
	operator/(fScalar);
}

bool ColorRGB::operator==(const ColorRGB &clr)
{
	return R == clr.R && G == clr.G && B == clr.B; 
}

bool ColorRGB::operator!=(const ColorRGB &clr)
{
	return !operator==(clr);
}

void ColorRGB::_ClampValues()
{
	for(int i = 0;i < 3;++i){
		if(C[i] > 1.f){
			C[i] = 1.f;
		}else if(C[i] < 0.f){
			C[i] = 0.f;
		}
	}
}