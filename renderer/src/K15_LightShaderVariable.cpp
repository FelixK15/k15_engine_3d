/**
 * @file K15_LightShaderVariable.cpp
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

#include "K15_LightShaderVariable.h"
#include "K15_Renderer.h"
#include "K15_Light.h"

using namespace K15_Renderer;

void LightShaderVariable::Update(Geometry *pGeometry)
{
	DynamicArray<Light*> &arrLights = g_pRenderer->GetLights();
	Vector3F vecPosCamera(0.0,0.0,-10.f);
	const U32 iNumBuffer = sizeof(Light) * 8 + sizeof(Material) + sizeof(Vector3F) + sizeof(U32);
	byte buffer[iNumBuffer] = {0};
	U32 iOffset = 0;
	U32 iNumLights = 0;
	
	for(iNumLights = 0;iNumLights < arrLights.Size();++iNumLights){
		if(iNumLights == 8){
			break;
		}

		Memory::MemCopy(&buffer[iOffset],arrLights[iNumLights],sizeof(Light));
		iOffset+= sizeof(Light);
	}

	if(iNumLights < 8){
		U32 iMissingLights = 8 - iNumLights;
		Memory::MemSet(&buffer[iOffset],0,sizeof(Light) * iMissingLights);
		iOffset += sizeof(Light) * iMissingLights;
	}

	Memory::MemCopy(&buffer[iOffset],&pGeometry->Material,sizeof(Material));
	iOffset += sizeof(Material);

	Memory::MemCopy(&buffer[iOffset],&vecPosCamera,sizeof(Vector3F));
	iOffset += sizeof(Vector3F);

	Memory::MemCopy(&buffer[iOffset],&iNumLights,sizeof(U32));

	SetData((float*)&buffer);
}