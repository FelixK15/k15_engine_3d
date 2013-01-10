/**
 * @file K15_ColorRGBA.cpp
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

#include "K15_ColorRGBA.h"

using namespace K15_Renderer;

ColorRGBA::ColorRGBA()
{
	R = G = B = A = 0.f;
}

ColorRGBA::ColorRGBA(float R,float G,float B,float A)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}

ColorRGBA::ColorRGBA(const ColorRGBA &clr)
{
	this->R = clr.R;
	this->G = clr.G;
	this->B = clr.B;
	this->A = clr.A;
}

const ColorRGBA &ColorRGBA::operator=(const ColorRGBA &clr)
{
	this->R = clr.R;
	this->G = clr.G;
	this->B = clr.B;
	this->A = clr.A;
	return *this;
}

const ColorRGBA &ColorRGBA::operator+(const ColorRGBA &clr)
{
	R += clr.R;
	G += clr.B;
	B += clr.G;
	A += clr.A;

	_ClampValues();
	return *this;
}

const ColorRGBA &ColorRGBA::operator-(const ColorRGBA &clr)
{
	R -= clr.R;
	G -= clr.B;
	B -= clr.G;
	A -= clr.A;

	_ClampValues();
	return *this;
}

const ColorRGBA &ColorRGBA::operator*(const ColorRGBA &clr)
{
	R *= clr.R;
	G *= clr.B;
	B *= clr.G;
	A *= clr.A;

	_ClampValues();
	return *this;
}

const ColorRGBA &ColorRGBA::operator/(const ColorRGBA &clr)
{
	R /= clr.R;
	G /= clr.B;
	B /= clr.G;
	A /= clr.A;

	_ClampValues();
	return *this;
}

const ColorRGBA &ColorRGBA::operator*(float fScalar)
{
	R *= fScalar;
	G *= fScalar;
	B *= fScalar;
	A *= fScalar;

	_ClampValues();
	return *this;
}

const ColorRGBA &ColorRGBA::operator/(float fScalar)
{
	R /= fScalar;
	G /= fScalar;
	B /= fScalar;
	A /= fScalar;

	_ClampValues();
	return *this;
}

void ColorRGBA::operator+=(const ColorRGBA &clr)
{
	operator+(clr);
}

void ColorRGBA::operator-=(const ColorRGBA &clr)
{
	operator-(clr);
}

void ColorRGBA::operator*=(const ColorRGBA &clr)
{
	operator*(clr);
}

void ColorRGBA::operator/=(const ColorRGBA &clr)
{
	operator/(clr);
}

void ColorRGBA::operator*=(float fScalar)
{
	operator*(fScalar);
}

void ColorRGBA::operator/=(float fScalar)
{
	operator/(fScalar);
}

bool ColorRGBA::operator==(const ColorRGBA &clr)
{
	return R == clr.R && G == clr.G && B == clr.B && A == clr.A; 
}

bool ColorRGBA::operator!=(const ColorRGBA &clr)
{
	return !operator==(clr);
}

void ColorRGBA::_ClampValues()
{
	for(int i = 0;i < 4;++i){
		if(C[i] > 1.f){
			C[i] = 1.f;
		}else if(C[i] < 0.f){
			C[i] = 0.f;
		}
	}
}