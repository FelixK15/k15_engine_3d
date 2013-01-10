/**
 * @file K15_Light.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#pragma once
#ifndef __K15_LIGHT__
#define __K15_LIGHT__

#include "K15_Object.h"
#include "K15_ColorRGBA.h"
#include "K15_Vector3.h"

#include "K15_ShaderVariable.h"
#include "K15_Material.h"
#include "K15_Geometry.h"

using namespace K15_EngineV2;
using namespace K15_Math;

namespace K15_Renderer
{
	enum LightType{
		LT_DIRECT = 0,
		LT_POINT,
		LT_SPOT
	};

	class Light
	{
	public:
		Light(LightType ltType);
		Light(const Light &lLight);

		~Light();

		const Light &operator=(const Light &lLight);

	public:
		ColorRGBA Diffuse; 
		ColorRGBA Specular;
		ColorRGBA Ambient; 

		Vector3F Position; 
		float Range; 

		Vector3F Direction; 

		LightType Type; 
		float Pi; 

		float Attenuation0; 
		float Attenuation1; 
		float Attenuation2; 
	};
}

#endif //__K15_LIGHT__