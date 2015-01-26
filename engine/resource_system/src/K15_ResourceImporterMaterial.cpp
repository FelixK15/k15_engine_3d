/**
 * @file K15_ResourceImporterMaterial.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/05
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

#include "K15_ResourceManager.h"
#include "K15_ResourceImporterMaterial.h"
#include "K15_RendererBase.h"
#include "K15_ResourceManager.h"
#include "K15_Material.h"
#include "K15_GpuProgramBatch.h"
#include "K15_GpuProgramCatalog.h"
#include "K15_RawData.h"
#include "K15_IOUtil.h"

#include <K15_MaterialFormat.h>


#include "json/reader.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	ResourceImporterMaterial::ResourceImporterMaterial()
		: ResourceImporterBase("Material")
	{

	}
	/*********************************************************************************/
	ResourceImporterMaterial::~ResourceImporterMaterial()
	{

	}
	/*********************************************************************************/
	void ResourceImporterMaterial::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".k15material");
	}
	/*********************************************************************************/
	void ResourceImporterMaterial::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{

	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterMaterial::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		K15_MaterialFormat materialFormat;
		
		if(K15_LoadMaterialFormatFromMemory(&materialFormat, p_ResourceData.data) != K15_SUCCESS)
		{
			return 0;
		}
		
		uint32 submaterialCount = materialFormat.submaterialCount;
		
		Material* material = K15_NEW Material();
		material->setName(materialFormat.materialName);
		
		for(uint32 submaterialIndex = 0;
			submaterialIndex < materialFormat.submaterialCount;
			++submaterialIndex)
		{
			K15_SubMaterialFormat* currentSubmaterial = &materialFormat.subMaterials[submaterialIndex];
			MaterialPass* currentMaterialPass = material->getPass(submaterialIndex, true);

			ColorRGBA diffuseColor;
			ColorRGBA specularColor;
			ColorRGBA ambientColor;

			diffuseColor.R = (byte)(currentSubmaterial->diffuseColor[0] * 255.f);
			diffuseColor.G = (byte)(currentSubmaterial->diffuseColor[1] * 255.f);
			diffuseColor.B = (byte)(currentSubmaterial->diffuseColor[2] * 255.f);

			specularColor.R = (byte)(currentSubmaterial->specularColor[0] * 255.f);
			specularColor.G = (byte)(currentSubmaterial->specularColor[1] * 255.f);
			specularColor.B = (byte)(currentSubmaterial->specularColor[2] * 255.f);

			ambientColor.R = (byte)(currentSubmaterial->ambientColor[0] * 255.f);
			ambientColor.G = (byte)(currentSubmaterial->ambientColor[1] * 255.f);
			ambientColor.B = (byte)(currentSubmaterial->ambientColor[2] * 255.f);

			currentMaterialPass->setShininess(currentSubmaterial->shininess);
			currentMaterialPass->setDiffuse(diffuseColor);
			currentMaterialPass->setSpecular(specularColor);
			currentMaterialPass->setAmbient(ambientColor);
			
			for(uint32 diffuseTextureIndex = 0;
				diffuseTextureIndex < currentSubmaterial->diffuseTextureCount;
				++diffuseTextureIndex)
			{
				K15_SubMaterialTextureFormat* currentTexture = &currentSubmaterial->diffuseTextureFormat[diffuseTextureIndex];
				Texture* diffuseTexture = g_ResourceManager->getResource<Texture>(currentTexture->textureName);

				currentMaterialPass->setDiffuseMap(diffuseTexture);
			}

			currentMaterialPass->setProgram(GpuProgramCatalog::getGpuProgram("default.vert", GpuProgram::PS_VERTEX), GpuProgram::PS_VERTEX);
			currentMaterialPass->setProgram(GpuProgramCatalog::getGpuProgram("default.frag", GpuProgram::PS_FRAGMENT), GpuProgram::PS_FRAGMENT);
		}

		return material;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
