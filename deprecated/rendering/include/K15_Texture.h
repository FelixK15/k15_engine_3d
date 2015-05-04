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

#include "K15_Prerequisites.h"
#include "K15_ResourceBase.h"
#include "K15_RenderWindow.h"
#include "K15_GpuResource.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	class K15_CORE_API TextureImplBase : public RenderingAllocatedObject
	{
	public:
		TextureImplBase();
		virtual ~TextureImplBase();

		virtual bool loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, uint32 p_Depth, bool p_CreateMipMaps) = 0;

		void setTexture(Texture* p_Texture);
		Texture* getTexture() const;

	protected:
		Texture *m_Texture;
	};// end of TexutreImpl class declaration
	/*********************************************************************************/
	class K15_CORE_API Texture : public GpuResource, 
								 public ResourceBase, 
								 public RenderingAllocatedObject
	{
		K15_DECLARE_RTTI;

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
		enum eTextureSlot
		{
			TS_SLOT1 = 0,
			TS_SLOT2,
			TS_SLOT3,
			TS_SLOT4,
			TS_SLOT5,
			TS_SLOT6,
			TS_SLOT7,
			TS_SLOT8,

			TS_COUNT,

			TS_NO_SLOT
		}; //TextureSlot
		/*********************************************************************************/
	public:
		/*********************************************************************************/
		struct K15_CORE_API CreationOptions
		{
			uint32 width;
			uint32 height;
			uint32 depth;

			RawData pixels;

			Enum pixelFormat;

			bool createMipMaps;
			bool useShadowCopy;

			uint32 mipmapLevels;

			CreationOptions();
		}; // end of Texture::CreationOptions struct
		/*********************************************************************************/
	public:
		Texture();
		Texture(const CreationOptions& p_CreationOptions);
		virtual ~Texture();

		INLINE Enum getTextureType() const;

		void setUsage(Enum p_TextureUsage);
		INLINE Enum getUsage() const;

		INLINE uint32 getMipmapLevels() const;
		INLINE uint32 getHeight() const;
		INLINE uint32 getWidth() const;
		INLINE uint32 getDepth() const;

		void setPixelFormat(Enum p_PixelFormat);
		INLINE Enum getPixelFormat() const;

		INLINE void setTextureSamplerSlot(Enum p_Slot);
		INLINE void setSlot(Enum p_Slot);
		INLINE Enum getSlot() const;
		INLINE Enum getTextureSamplerSlot() const;

		void getDimension(uint32* p_Width, uint32* p_Height, uint32* p_Depth = 0);
		void getMipmapDimension(uint32 p_MipmapLevel, uint32* p_Width, uint32* p_Height, uint32* p_Depth = 0);

		uint32 getSize() const;
		uint32 getMipmapSize(uint32 p_Index) const;

		bool loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, bool p_CreateMipMap = true);
		bool loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, bool p_CreateMipMap = true);
		bool loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, uint32 p_Depth, bool p_CreateMipMap = true);

		bool create(const CreationOptions& p_Options);

		INLINE bool hasAlpha() const;

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

		INLINE TextureImplBase* getImpl() const;

	private:
		uint32 calculateMipmapLevels() const;

	private:
		TextureImplBase* m_Impl;
		byte* m_ShadowCopy;
		Enum m_Type;
		Enum m_Usage;
		Enum m_PixelFormat;
		Enum m_Slot;
		Enum m_SamplerSlot;
		uint32 m_Height;
		uint32 m_Width;
		uint32 m_Depth;
		uint32 m_Size;
		uint32 m_MipmapLevels;
		bool m_HasAlpha;
		bool m_HasShadowCopy;
	};//end of Texture class declaration
	/*********************************************************************************/
	
	/*********************************************************************************/
#	include "K15_Texture.inl"
}}// end of K15_Engine::Rendering

#endif //_K15Engine_Renderer_Texture_h_