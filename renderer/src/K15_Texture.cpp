/**
 * @file K15_Texture.cpp
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
#include "K15_Texture.h"
#include "K15_RendererBase.h"

#include "..\..\dependencies\nv_dds\include\nv_dds.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	TextureImplBase::TextureImplBase()
		: m_Texture(0)
	{

	}
	/*********************************************************************************/
	TextureImplBase::~TextureImplBase()
	{

	}
	/*********************************************************************************/
	void TextureImplBase::setTexture(Texture* p_Texture)
	{
		m_Texture = p_Texture;
	}
	/*********************************************************************************/
	Texture* TextureImplBase::getTexture() const
	{
		return m_Texture;
	}
	/*********************************************************************************/

	/*********************************************************************************/
	Texture::Texture()
		: m_Impl(0),
		  m_MipMapCount(0),
		  m_PixelFormat(0),
		  m_TextureType(0),
		  m_TextureUsage(0),
		  m_Width(0),
		  m_Height(0),
		  m_Depth(0)
	{
// 		m_Impl = g_Renderer->getTextureImpl();
	}
	/*********************************************************************************/
	Texture::~Texture()
	{
//		K15_DELETE m_Impl;
	}
	/*********************************************************************************/
	void Texture::setTextureType(Enum p_TextureType)
	{
		m_TextureType = p_TextureType;
	}
	/*********************************************************************************/
	void Texture::setTextureUsage(Enum p_TextureUsage)
	{
		m_TextureUsage = p_TextureUsage;
	}
	/*********************************************************************************/
	void Texture::setMipMapCount(uint8 p_MipMapCount)
	{
		m_MipMapCount = p_MipMapCount;
	}
	/*********************************************************************************/
	void Texture::setHeight(uint32 p_Height)
	{
		m_Height = p_Height;
	}
	/*********************************************************************************/
	void Texture::setWidth(uint32 p_Width)
	{
		m_Width = p_Width;
	}
	/*********************************************************************************/
	void Texture::setResolution(const Resolution& p_Resolution)
	{
		m_Width = p_Resolution.width;
		m_Height = p_Resolution.height;
	}
	/*********************************************************************************/
	void Texture::setDepth(uint32 p_Depth)
	{
		m_Depth = p_Depth;
	}
	/*********************************************************************************/
	void Texture::setPixelFormat(Enum p_PixelFormat)
	{
		m_PixelFormat = p_PixelFormat;
	}
	/*********************************************************************************/
	bool Texture::hasAlpha() const
	{
		return m_HasAlpha;
	}
	/*********************************************************************************/
	void Texture::loadDebug(ResourceData& p_Data)
	{
		
	}
	/*********************************************************************************/
	bool Texture::internalLoad(const ResourceData& p_Data)
	{
		// K15 Texture are loaded as DDS 
		nv_dds::DDS_HEADER *header;
		byte* data = p_Data.Data;
		uint32 width, height, depth;
		uint8 bpp, mipmaps;
		width = height = depth = bpp = mipmaps = 0;

		//Validate dds file
		if(data[0] != 'D' && data[1] != 'D' && data[2] != 'S')
		{
			//_LogError("No DDS file!");
			return false;
		}

		data += 4; //increment dds offset

		//get header data
		header = (nv_dds::DDS_HEADER*)data;
		
		//set width
		if(header->dwFlags & nv_dds::DDSF_WIDTH)
		{
			width = header->dwWidth;
		}
		
		//set height
		if(header->dwFlags & nv_dds::DDSF_HEIGHT)
		{
			height = header->dwHeight;
		}

		//set depth
		if(header->dwFlags & nv_dds::DDSF_DEPTH)
		{
			depth = header->dwDepth;
		}
		
		//set mip map count
		if(header->dwFlags & nv_dds::DDSF_MIPMAPCOUNT)
		{
			mipmaps = header->dwMipMapCount;
		}
		//else default mip map count
		
		// check for alpha support
		if(header->ddspf.dwFlags & nv_dds::DDSF_ALPHAPIXELS)
		{
			m_HasAlpha = true;
		}

		bpp = header->ddspf.dwRGBBitCount;

// 		m_Impl->init()
// 
// 		data += sizeof(nv_dds::DDS_HEADER);

		//create normal map etc.

		return true;
	}
	/*********************************************************************************/
	uint32 Texture::getTextureSize() const
	{
		uint32 size = 0;
		for(int i = 0;i < m_MipMapCount;++i)
		{
			size += getMipMapSize(i);
		}

		return size;
	}
	/*********************************************************************************/
	uint32 Texture::getMipMapSize(uint8 p_Index) const
	{
		uint32 width = m_Width;
		uint32 height = m_Height > 0 ? m_Height : 1;
		uint32 depth = m_Depth > 0 ? m_Depth : 1;

		uint32 bitsPerPixel = RendererBase::PixelFormatSize[m_PixelFormat];

		width *= (uint32)(0.5 / p_Index+1);	
		height *= (uint32)(0.5 / p_Index+1);
		depth *= (uint32)(0.5 / p_Index+1);

		return width * height * depth * bitsPerPixel;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace