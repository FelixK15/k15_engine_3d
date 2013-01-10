/**
 * @file K15_Light.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/23
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

#include "K15_Light.h"

using namespace K15_Renderer;

Light::Light(LightType ltType)
	: Type(ltType)
{

}

Light::Light(const Light &lLight)
	: Ambient(lLight.Ambient),
	  Diffuse(lLight.Diffuse),
	  Specular(lLight.Specular),
	  Range(lLight.Range),
	  Direction(lLight.Direction),
	  Position(lLight.Position),
	  Pi(lLight.Pi),
	  Type(lLight.Type),
	  Attenuation0(lLight.Attenuation0),
	  Attenuation1(lLight.Attenuation1),
	  Attenuation2(lLight.Attenuation2)

{

}

Light::~Light()
{

}

const Light &Light::operator=(const Light &lLight)
{
	return *this;
}
