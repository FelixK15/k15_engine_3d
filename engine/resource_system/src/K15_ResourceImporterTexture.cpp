/**
 * @file K15_ResourceImporterObj.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/26
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

#include "K15_Texture.h"
#include "K15_ResourceImporterTexture.h"
#include "K15_RawData.h"
#include "K15_RendererBase.h"

#include <K15_HeaderFormat.h>
#include <K15_TextureFormat.h>
#include <K15_DataAccessHelper.h>

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceImporterTexture,ResourceImporterBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceImporterTexture::ResourceImporterTexture()
		 : ResourceImporterBase("Texture")
	{

	}
	/*********************************************************************************/
	ResourceImporterTexture::~ResourceImporterTexture()
	{

	}	
	/*********************************************************************************/
	void ResourceImporterTexture::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".k15tex");
	}
	/*********************************************************************************/
	void ResourceImporterTexture::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{
		RawData magicNumber;

		magicNumber.data = (byte*)K15_RESOURCE_IDENTIFIER;
		magicNumber.size = K15_IDENTIFIER_LENGTH;

		p_MagicNumber.push_back(magicNumber);
	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterTexture::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		K15_DataAccessContext dataAccess;
		K15_TextureFormat textureFormat;

		K15_CreateDataAccessContextFromMemory(&dataAccess, p_ResourceData.data);

		if (K15_InternalLoadTextureFormat(&dataAccess, &textureFormat) != K15_SUCCESS)
		{

		}

		Texture::CreationOptions creationOptions;

		creationOptions.createMipMaps = false;
		creationOptions.height = textureFormat.height;
		creationOptions.width = textureFormat.width;
		creationOptions.useShadowCopy = false;
		creationOptions.pixelFormat = RendererBase::PF_RGBA_8_DXT5;

		Texture* texture = K15_NEW Texture(creationOptions);

		return 0;
	}
	/*********************************************************************************/

}} // end of K15_Engine::Core namespace