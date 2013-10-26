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

#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Rendering {
	class TextureImplBase
	{
	public:
		virtual void init(Enum p_TextureType, Enum p_TextureUsage, Enum p_PixelFormat, uint32 p_Width, uint32 p_Height, uint32 p_Depth, uint8 p_MipMapCount) = 0;
		virtual void shutdown() = 0;

		virtual void setTextureWrapMode(Enum p_TextureWrapMode) = 0;
		virtual void setTextureType(Enum p_TextureType) = 0;
		virtual void setTextureUsage(Enum p_TextureUsage) = 0;

		virtual void setMipMapCount(uint8 p_MipMapCount) = 0;

		virtual void setHeight(uint32 p_Height) = 0;
		virtual void setWidth(uint32 p_Width) = 0;
		virtual void setDepth(uint32 p_Depth) = 0;

		virtual void setPixelFormat(Enum p_PixelFormat) = 0;
		virtual bool hasAlpha() const = 0;
	protected:
		Texture *m_Texture;
	};// end of TexutreImpl class declaration
	/*********************************************************************************/
	class Texture
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
			TU_DYNAMIC = 0,
			TU_STATIC,

			TU_COUNT
		};//TextureUsage
		/*********************************************************************************/
		enum eTextureWrapMode
		{
			TWM_CLAMP = 0,
			TWM_MIRROR,
			TWM_REPEAT,

			TWM_COUNT
		};//TextureWrapMode
		/*********************************************************************************/
	public:
		Texture();
		~Texture();

		void setTextureWrapMode(Enum p_TextureWrapMode);
		inline Enum getTextureWrapMode() const;

		void setTextureType(Enum p_TextureType);
		inline Enum getTextureType() const;

		void setTextureUsage(Enum p_TextureUsage);
		inline Enum getTextureUsage() const;

		void setMipMapCount(uint8 p_MipMapCount);
		inline uint8 getMipMapCount() const;

		void setHeight(uint32 p_Height);
		inline uint32 getHeight() const;

		void setWidth(uint32 p_Width);
		inline uint32 getWidth() const;

		void setResolution(const Resolution& p_Resolution);
		inline Resolution getResolution() const;

		void setDepth(uint32 p_Depth);
		inline uint32 getDepth() const;

		void setPixelFormat(Enum p_PixelFormat);
		inline Enum getPixelFormat() const;

		bool hasAlpha() const;

	private:
		TextureImplBase* m_Impl;
		Enum m_TextureType;
		Enum m_TextureUsage;
		Enum m_PixelFormat;
		uint32 m_Height;
		uint32 m_Width;
		uint32 m_Depth;
		uint8 m_MipMapCount;
	};//end of Texture class declaration
#	include "K15_Texture.inl"
}}// end of K15_Engine::Rendering

#endif //_K15Engine_Renderer_Texture_h_