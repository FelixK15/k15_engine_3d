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

#include "K15_PrecompiledHeader.h"
#include "K15_Texture.h"
#include "K15_RendererBase.h"


namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Rendering,Texture,ResourceBase);
	/*********************************************************************************/

	/*********************************************************************************/
	Texture::CreationOptions::CreationOptions()
		: width(0),
		height(0),
		depth(0),
		useShadowCopy(false),
		createMipMaps(false),
		pixelFormat(0),
		mipmapLevels(0)
	{

	}
	/*********************************************************************************/

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
		m_MipmapLevels(0),
		m_PixelFormat(0),
		m_Type(0),
		m_Usage(0),
		m_Width(0),
		m_Height(0),
		m_Depth(0),
		m_HasAlpha(true),
		m_HasShadowCopy(false),
		m_ShadowCopy(0),
		m_Slot(TS_NO_SLOT),
		m_SamplerSlot(TS_SLOT1)
	{
 		m_Impl = g_Application->getRenderer()->createTextureImpl();
		m_Impl->setTexture(this);
	}
	/*********************************************************************************/
	Texture::Texture(const CreationOptions& p_CreationOptions)
		: m_Impl(0),
		m_MipmapLevels(0),
		m_PixelFormat(0),
		m_Type(0),
		m_Usage(0),
		m_Width(0),
		m_Height(0),
		m_Depth(0),
		m_HasAlpha(true),
		m_HasShadowCopy(false),
		m_ShadowCopy(0),
		m_Slot(TS_NO_SLOT),
		m_SamplerSlot(TS_SLOT1)
	{
		m_Impl = g_Application->getRenderer()->createTextureImpl();
		m_Impl->setTexture(this);

		create(p_CreationOptions);
	}
	/*********************************************************************************/
	Texture::~Texture()
	{
		if(isBound())
		{
			getRenderer()->bindTexture(0, m_Type, m_Slot);
		}

		K15_DELETE m_Impl;
	}
	/*********************************************************************************/
	void Texture::setUsage(Enum p_TextureUsage)
	{
		m_Usage = p_TextureUsage;
	}
	/*********************************************************************************/
	void Texture::setPixelFormat(Enum p_PixelFormat)
	{
		m_PixelFormat = p_PixelFormat;

		if(m_PixelFormat > RendererBase::PF_RGB_32_F)
		{
			m_HasAlpha = true;
		}
	}
	/*********************************************************************************/
	bool Texture::hasAlpha() const
	{
		return m_HasAlpha;
	}
	/*********************************************************************************/
	void Texture::loadDebug(RawData& p_Data)
	{
		
	}
	/*********************************************************************************/
	bool Texture::internalLoad(const RawData& p_Data)
	{
		// K15 Texture are loaded as DDS 
		DDS_HEADER *header;
		byte* data = p_Data.data;
		uint32 width, height, depth;
		Enum type = 0;
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
		header = (DDS_HEADER*)data;
		
		//set width
		if(header->dwFlags & DDSF_WIDTH)
		{
			width = header->dwWidth;
			type = TT_1D;
		}
		
		//set height
		if(header->dwFlags & DDSF_HEIGHT)
		{
			height = header->dwHeight;
			type = TT_2D;
		}

		//set depth
		if(header->dwFlags & DDSF_DEPTH)
		{
			depth = header->dwDepth;
			type = TT_3D;
		}
		
		//set mip map count
		if(header->dwFlags & DDSF_MIPMAPCOUNT)
		{
			mipmaps = header->dwMipMapCount;
		}
		//else default mip map count
		
		// check for alpha support
		if(header->ddspf.dwFlags & DDSF_ALPHAPIXELS)
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
	uint32 Texture::getSize() const
	{
		uint32 size = 0;
		for(uint32 i = 0;i < m_MipmapLevels;++i)
		{
			size += getMipmapSize(i);
		}

		return size;
	}
	/*********************************************************************************/
	uint32 Texture::getMipmapSize(uint32 p_Index) const
	{
		uint32 width = m_Width;
		uint32 height = m_Height > 0 ? m_Height : 1;
		uint32 depth = m_Depth > 0 ? m_Depth : 1;
		uint32 bitsPerPixel = RendererBase::PixelFormatSize[m_PixelFormat];

		if(p_Index == 0)
		{
			width = m_Width;
			height = m_Height;
			depth = m_Depth;
		}
		else
		{
			width *= (uint32)(0.5 / p_Index);	
			height *= (uint32)(0.5 / p_Index);
			depth *= (uint32)(0.5 / p_Index);
		}

		if(m_Type == TT_1D)
		{
			return width * bitsPerPixel;
		}
		else if(m_Type == TT_2D)
		{
			return (width * height) * bitsPerPixel;
		}

		return (width * height * depth) * bitsPerPixel;;
	}
	/*********************************************************************************/
	uint32 Texture::calculateMipmapLevels() const
	{
		uint32 i = 0;
		uint32 dimension = std::max<uint32>(m_Width,m_Height);

		while(dimension > 1)
		{
			dimension =  (uint32)(dimension * 0.5);
			++i;
		}
		return i;
	}
	/*********************************************************************************/
	bool Texture::create(const CreationOptions& p_Options)
	{
		if(p_Options.width > 0 && p_Options.height > 0 && p_Options.depth > 0)
		{
			m_Type = Texture::TT_3D;
			m_Width = p_Options.width;
			m_Height = p_Options.height;
			m_Depth = p_Options.depth;
		}
		else if(p_Options.width > 0 && p_Options.height > 0)
		{
			m_Type = Texture::TT_2D;
			m_Width = p_Options.width;
			m_Height = p_Options.height;
		}
		else if(p_Options.width > 0)
		{
			m_Type = Texture::TT_1D;
			m_Width = p_Options.width;
		}

		m_PixelFormat = p_Options.pixelFormat;

		if(m_PixelFormat > RendererBase::PF_RGB_32_F)
		{
			m_HasAlpha = true;
		}

		m_HasShadowCopy = p_Options.useShadowCopy;

		int i = 0;
		if(p_Options.mipmapLevels > 0)
		{
			i = p_Options.mipmapLevels;
		}
		else if(p_Options.createMipMaps)
		{
			i = calculateMipmapLevels();
		}

		m_MipmapLevels = i-1;

		return loadRawData(p_Options.pixels.data,m_PixelFormat,m_Width,m_Height,m_Depth,p_Options.createMipMaps);
	}
	/*********************************************************************************/
	void Texture::getMipmapDimension(uint32 p_MipmapLevel, uint32* p_Width, uint32* p_Height, uint32* p_Depth)
	{
		if(p_MipmapLevel > m_MipmapLevels)
		{
			K15_LOG_WARNING("Trying to get the dimension of mipmap %u from texture %s that only has %u mipmaps. Clamping value to %u",p_MipmapLevel,m_Name.c_str(),m_MipmapLevels,m_MipmapLevels);
			p_MipmapLevel = m_MipmapLevels;
		}

		if(p_Width)
		{
			if(p_MipmapLevel == 0)
			{
				*p_Width = m_Width;
			}
			else
			{
				*p_Width = m_Width * (uint32)(0.5 / p_MipmapLevel);	
			}
		}

		if(p_Height)
		{
			if(p_MipmapLevel == 0)
			{
				*p_Height = m_Height;
			}
			else
			{
				*p_Height = m_Height * (uint32)(0.5 / p_MipmapLevel);	
			}
		}

		if(p_Depth)
		{
			if(p_MipmapLevel == 0)
			{
				*p_Depth = m_Depth;
			}
			else
			{
				*p_Depth = m_Depth * (uint32)(0.5 / p_MipmapLevel);
			}
		}
	}
	/*********************************************************************************/
	void Texture::getDimension(uint32* p_Width, uint32* p_Height, uint32* p_Depth)
	{
		getMipmapDimension(0,p_Width,p_Height,p_Depth);
	}
	/*********************************************************************************/
	bool Texture::loadRawData( byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, uint32 p_Depth, bool p_CreateMipMap /*= true*/ )
	{
		if(m_Impl)
		{
			m_Impl->loadRawData(p_Data,p_Format,p_Width,p_Height,p_Depth,p_CreateMipMap);
		}

		return true;
	}
	/*********************************************************************************/
	bool Texture::loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, bool p_CreateMipMap)
	{
		return loadRawData(p_Data,p_Format,p_Width,p_Height,0,p_CreateMipMap);
	}
	/*********************************************************************************/
	bool Texture::loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, bool p_CreateMipMap)
	{
		return loadRawData(p_Data,p_Format,p_Width,0,0,p_CreateMipMap);
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace