/**
 * @file K15_Texture.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_Texture_h_
#define _K15Engine_Renderer_Texture_h_

#ifndef K15_RENDERER_USE_PRECOMPILED_HEADER
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_USE_PRECOMPILED_HEADER

#include "K15_ResourceBase.h"
#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	struct TextureCreationOptions
	{
		uint32 width;
		uint32 height;
		uint32 depth;

		RawData pixels;
		
		RendererBase::ePixelFormat pixelFormat;
		Texture::eTextureUsage usage;
		
		bool createMipMaps;
		bool useShadowCopy;

		uint8 mipMapLevels;
	}; // end of TextureCreationOptions struct
	/*********************************************************************************/
	class TextureImplBase
	{
	public:
		TextureImplBase();
		virtual ~TextureImplBase();

		virtual uint32 write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) = 0;
		virtual uint32 read(const Math::Vector3& p_Offset, const Math::Vector3& p_Dimension, byte** p_Destination) = 0;
		virtual bool resize(const Math::Vector3& p_Dimension) = 0;

		void setTexture(Texture* p_Texture);
		Texture* getTexture() const;
	protected:
		Texture *m_Texture;
	};// end of TexutreImpl class declaration
	/*********************************************************************************/
	class Texture : public ResourceBase
	{
	public:
		/*********************************************************************************/
		enum eTextureType
		{
			TT_1D = 0,
			TT_2D,
			TT_3D,
			TT_CUBEMAP,

			TT_COUNT
		};//TextureType
		/*********************************************************************************/
		enum eTextureUsage
		{
			TU_MUTABLE = 0,
			TU_IMMUTABLE,

			TU_COUNT
		};//TextureUsage
		/*********************************************************************************/
	public:
		Texture();
		Texture(const TextureCreationOptions& p_Options);
		~Texture();

		void getType(Enum p_TextureType);
		inline Enum getType() const;

		void setUsage(Enum p_TextureUsage);
		inline Enum getUsage() const;

		void setMipMapCount(uint8 p_MipMapCount);
		inline uint8 getMipMapCount() const;

		void setHeight(uint32 p_Height);
		inline uint32 getHeight() const;

		void setWidth(uint32 p_Width);
		inline uint32 getWidth() const;

		void setDepth(uint32 p_Depth);
		inline uint32 getDepth() const;

		inline const Math::Vector3& getDimension() const;

		void setPixelFormat(Enum p_PixelFormat);
		inline Enum getPixelFormat() const;

		uint32 getSize() const;
		uint32 getMipMapSize(uint8 p_Index) const;

		uint32 write(byte* p_Pixels, uint32 p_Width, int32 p_OffsetX = 0, uint8 p_MipMapLevel = 0);
		uint32 write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, int32 p_OffsetX = 0, int32 p_OffsetY = 0, uint8 p_MipMapLevel = 0);
		uint32 write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX = 0, int32 p_OffsetY = 0, int32 p_OffsetZ = 0, uint8 p_MipMapLevel = 0);

		uint32 read(byte** p_Destination, uint32 p_Width, int32 p_OffsetX = 0);
		uint32 read(byte** p_Destination, uint32 p_Width, uint32 p_Height, int32 p_OffsetX = 0, int32 p_OffsetY = 0);
		uint32 read(byte** p_Destination, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX = 0, int32 p_OffsetY = 0, int32 p_OffsetZ = 0);

		bool resize(uint32 p_Width,uint32 p_Height,uint32 p_Depth = 0,byte* p_PixelData = 0,uint32 p_PixelDataSize = 0);
		
		inline bool hasAlpha() const;
		inline bool hasShadowCopy() const;

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

	private:
		uint8 calculateMipMapLevels() const;

	private:
		TextureImplBase* m_Impl;
		byte* m_ShadowCopy;
		Enum m_Type;
		Enum m_Usage;
		Enum m_PixelFormat;
		uint32 m_Height;
		uint32 m_Width;
		uint32 m_Depth;
		uint32 m_Size;
		uint8 m_MipMapCount;
		bool m_HasAlpha;
		bool m_HasShadowCopy;
	};//end of Texture class declaration
#	include "K15_Texture.inl"
}}// end of K15_Engine::Rendering

#endif //_K15Engine_Renderer_Texture_h_