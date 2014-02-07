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
#include "K15_RenderTask.h"
#include "K15_RendererBase.h"
#include "K15_LogManager.h"

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
		m_MipmapLevels(0),
		m_PixelFormat(0),
		m_Type(0),
		m_Usage(0),
		m_Width(0),
		m_Height(0),
		m_Depth(0),
		m_HasAlpha(true),
		m_HasShadowCopy(true),
		m_ShadowCopy(0)
	{
 		m_Impl = g_Application->getRenderTask()->getRenderer()->createTextureImpl();
		m_Impl->setTexture(this);
	}
	/*********************************************************************************/
	Texture::~Texture()
	{
		K15_DELETE m_Impl;
	}
	/*********************************************************************************/
	void Texture::setUsage(Enum p_TextureUsage)
	{
		m_Usage = p_TextureUsage;
	}
	/*********************************************************************************/
	void Texture::setMipmapLevels(uint32 p_MipMapCount)
	{
		m_MipmapLevels = p_MipMapCount;

		resize(m_Width,m_Height,m_Depth);
	}
	/*********************************************************************************/
	void Texture::setHeight(uint32 p_Height)
	{
		m_Height = p_Height;

		resize(m_Width,m_Height,m_Depth);
	}
	/*********************************************************************************/
	void Texture::setWidth(uint32 p_Width)
	{
		m_Width = p_Width;

		resize(m_Width,m_Height,m_Depth);
	}
	/*********************************************************************************/
	void Texture::setDepth(uint32 p_Depth)
	{
		m_Depth = p_Depth;

		resize(m_Width,m_Height,m_Depth);
	}
	/*********************************************************************************/
	void Texture::setPixelFormat(Enum p_PixelFormat)
	{
		m_PixelFormat = p_PixelFormat;

		if(m_PixelFormat > RendererBase::PF_RGB_32_F)
		{
			m_HasAlpha = true;
		}

		//we need to "resize" the texture in order to let the graphics API recreate the texture storage
		resize(m_Width,m_Height,m_Depth);
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
	uint32 Texture::write(byte* p_Pixels, uint32 p_Width, int32 p_OffsetX)
	{
		if(p_Width + p_OffsetX > m_Width)
		{
			_LogWarning("Trying to write %u pixels to texture %s (starting from pixel %i) - This would write out of bounds.\n Clamping width to %u.",
				         p_Width + p_OffsetX,m_Name.c_str(),p_OffsetX,m_Width - p_OffsetX);

			p_Width = m_Width - p_OffsetX;
		}
	
		if(m_Impl->write(p_Pixels,p_Width,0,0,p_OffsetX,0,0))
		{
		  if(m_ShadowCopy)
		  {
			//todo
		  }

		  return p_Width;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 Texture::write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, int32 p_OffsetX, int32 p_OffsetY)
	{
		if(m_Type == TT_1D)
		{
			_LogError("Trying to write 2 dimensional pixel data to 1D texture %s.",m_Name.c_str());
			return 0;
		}

		if(p_Width + p_OffsetX > m_Width || p_Height + p_OffsetY > m_Height)
		{
			_LogWarning("Trying to write %ux%u pixels to texture %s (starting from pixel %ix%i) - This would write out of bounds.\n Clamping width to %ux%u.",
				p_Width + p_OffsetX,p_Height + p_OffsetY,m_Name.c_str(),p_OffsetX,p_OffsetY,m_Width - p_OffsetX,m_Height - p_OffsetY);

			p_Width = m_Width - p_OffsetX;
			p_Height = m_Height - p_OffsetY;
		}

		if(m_Impl->write(p_Pixels,p_Width,p_Height,0,p_OffsetX,p_OffsetY,0))
		{
		  if(m_ShadowCopy)
		  {
			//todo
		  }

		  return p_Width * p_Height;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 Texture::write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
	{
		if(m_Type == TT_1D || m_Type == TT_2D)
		{
			_LogError("Trying to write 3 dimensional pixel data to %iD texture %s.",m_Type == TT_1D ? 1 : 2,m_Name.c_str());
			return 0;
		}

		if(p_Width + p_OffsetX > m_Width || p_Height + p_OffsetY > m_Height || p_Depth + p_OffsetZ > m_Depth)
		{
			_LogWarning("Trying to write %ux%ux%u pixels to texture %s (starting from pixel %ix%ix%i) - This would write out of bounds.\n Clamping width to %ux%ux%u.",
				p_Width + p_OffsetX,p_Height + p_OffsetY,p_Depth + p_OffsetZ,m_Name.c_str(),
				p_OffsetX,p_OffsetY,p_OffsetZ,m_Width - p_OffsetX,m_Height - p_OffsetY,m_Depth - p_OffsetZ);

			p_Width = m_Width - p_OffsetX;
			p_Height = m_Height - p_OffsetY;
			p_Depth = m_Depth - p_Depth;
		}

		if(m_Impl->write(p_Pixels,p_Width,p_Height,p_Depth,p_OffsetX,p_OffsetY,p_OffsetZ))
		{
		  if(m_ShadowCopy)
		  {
			//todo
		  }

		  return p_Width * p_Height * p_Depth;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 Texture::writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, int32 p_OffsetX)
	{
		if(p_MipmapLevel > m_MipmapLevels)
		{
			_LogWarning("Trying to write to mipmap %u of texture %s which only has %u mipmaps.Clamping value to %u",p_MipmapLevel,m_Name.c_str(),m_MipmapLevels,m_MipmapLevels);
			p_MipmapLevel = m_MipmapLevels;
		}

		uint32 width = 0;
		getMipmapDimension(p_MipmapLevel,&width,0);

		if(p_Width + p_OffsetX > width)
		{
			_LogWarning("Trying to write %u pixels to texture %s (starting from pixel %i) - This would write out of bounds.\n Clamping width to %u.",
			p_Width + p_OffsetX,m_Name.c_str(),p_OffsetX,width - p_OffsetX);

			p_Width = width - p_OffsetX;
		}

		if(m_Impl->write(p_Pixels,p_Width,0,0,p_OffsetX,0,0))
		{
			if(m_ShadowCopy)
			{
			//todo
			}

			return p_Width;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 Texture::writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, int32 p_OffsetX, int32 p_OffsetY)
	{
		if(p_MipmapLevel > m_MipmapLevels)
		{
			_LogWarning("Trying to write to mipmap %u of texture %s which only has %u mipmaps.Clamping value to %u",p_MipmapLevel,m_Name.c_str(),m_MipmapLevels,m_MipmapLevels);

			p_MipmapLevel = m_MipmapLevels;
		}

		uint32 width = 0;
		uint32 height = 0;
		getMipmapDimension(p_MipmapLevel,&width,&height);

		if(p_Width + p_OffsetX > width || p_Height + p_OffsetY > height)
		{
			_LogWarning("Trying to write %ux%u pixels to texture %s (starting from pixel %ix%i) - This would write out of bounds.\n Clamping width to %ux%u.",
			p_Width + p_OffsetX,p_Height + p_OffsetY,m_Name.c_str(),p_OffsetX,p_OffsetY,width - p_OffsetX,height - p_OffsetY);

			p_Width = width - p_OffsetX;
			p_Height = height - p_OffsetY;
		}

		if(m_Impl->write(p_Pixels,p_Width,p_Height,0,p_OffsetX,p_OffsetY,0))
		{
			if(m_ShadowCopy)
			{
			//todo
			}

			return p_Width*p_Height;
		}

		return 0;
	}
	/*********************************************************************************/
	uint32 Texture::writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
	{
		if(p_MipmapLevel > m_MipmapLevels)
		{
			_LogWarning("Trying to write to mipmap %u of texture %s which only has %u mipmaps.Clamping value to %u",p_MipmapLevel,m_Name.c_str(),m_MipmapLevels,m_MipmapLevels);

			p_MipmapLevel = m_MipmapLevels;
		}

		uint32 width = 0;
		uint32 height = 0;
		uint32 depth = 0;
		getMipmapDimension(p_MipmapLevel,&width,&height,&depth);

		if(p_Width + p_OffsetX > width || p_Height + p_OffsetY > height || p_Depth + p_OffsetZ > depth)
		{
			_LogWarning("Trying to write %ux%ux%u pixels to texture %s (starting from pixel %ix%ix%i) - This would write out of bounds.\n Clamping width to %ux%ux%u.",
			p_Width + p_OffsetX,p_Height + p_OffsetY,p_Depth + p_OffsetZ,m_Name.c_str(),
			p_OffsetX,p_OffsetY,p_OffsetZ,width - p_OffsetX,height - p_OffsetY,depth - p_OffsetZ);

			p_Width = width - p_OffsetX;
			p_Height = height - p_OffsetY;
			p_Depth = depth - p_Depth;
		}

		if(m_Impl->write(p_Pixels,p_Width,p_Height,p_Depth,p_OffsetX,p_OffsetY,p_OffsetZ))
		{
			if(m_ShadowCopy)
			{
			//todo
			}

			return p_Width*p_Height*p_Depth;
		}

		return 0;
	}
	/*********************************************************************************/
	bool Texture::create(const TextureCreationOptions& p_Options)
	{
		if(p_Options.width > 0 && p_Options.height > 0 && p_Options.depth > 0)
		{
			m_Type = Texture::TT_3D;
			m_Width = p_Options.width;
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
			m_Height = p_Options.height;
			m_Depth = p_Options.depth;
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

		if(resize(p_Options.width,p_Options.height,p_Options.depth))
		{
			if(p_Options.pixels.size > 0)
			{
				return write(p_Options.pixels.data,m_Width,m_Height) > 0;
			}

			return true;
		}
		
		return false;
	}
	/*********************************************************************************/
	bool Texture::resize(uint32 p_Width,uint32 p_Height,uint32 p_Depth)
	{
		Enum type = TT_1D;

		if(p_Height > 0)
		{
			type = TT_2D;
		}
		
		if(p_Depth > 0)
		{
			type = TT_3D;
		}

		m_Type = type;

		if(m_Impl->resize(p_Width,p_Height,p_Depth))
		{
		  m_Width = p_Width;
		  m_Height = p_Height;
		  m_Depth = p_Depth;

		  //create shadow copy buffer
		  if(m_HasShadowCopy)
		  {
			if(m_ShadowCopy)
			{
			  delete[] m_ShadowCopy;
			}
			uint32 size = getSize();

			m_ShadowCopy = new byte[size];
			memset(m_ShadowCopy,0,size);
		  }
		  return true;
		}

		return false;
	}
  /*********************************************************************************/
  void Texture::getMipmapDimension(uint32 p_MipmapLevel, uint32* p_Width, uint32* p_Height, uint32* p_Depth)
  {
    if(p_MipmapLevel > m_MipmapLevels)
    {
      _LogWarning("Trying to get the dimension of mipmap %u from texture %s that only has %u mipmaps. Clamping value to %u",p_MipmapLevel,m_Name.c_str(),m_MipmapLevels,m_MipmapLevels);
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
}}//end of K15_Engine::Rendering namespace