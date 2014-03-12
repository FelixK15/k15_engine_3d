/**
 * @file K15_TextureImplOGL.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#ifndef _K15Engine_RendererWGL_TextureImpl_h_
#define _K15Engine_RendererWGL_TextureImpl_h_

#ifndef K15_RENDERER_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_RendererBase.h"
#include "K15_Texture.h"

namespace K15_Engine { namespace Rendering { namespace WGL {
	class TextureImpl : public TextureImplBase
	{
	public:
		/*********************************************************************************/
		static const GLuint GLTextureTypeConverter[Texture::TT_COUNT];
		static const GLuint GLInternalFormatConverter[RendererBase::PF_COUNT];
		static const GLenum GLPixelDataTypeConverter[RendererBase::PF_COUNT];
		/*********************************************************************************/
	public:
		TextureImpl();
		~TextureImpl();

		virtual bool loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, uint32 p_Depth, bool p_CreateMipMaps) OVERRIDE;

		virtual bool write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) OVERRIDE;
		virtual bool writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) OVERRIDE;

// 		virtual bool read(byte** p_Destination, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) OVERRIDE;
// 		virtual bool readMipmap(byte** p_Destination, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) OVERRIDE;

		virtual bool resize(uint32 p_Width, uint32 p_Height, uint32 p_Depth) OVERRIDE;

		INLINE GLuint getTextureHandle() const;
	private:
		GLuint m_TextureHandle;
		uint32 m_TextureStorageSize;
	};// end of TextureImplOGL class declaration
#	include "K15_WGL_TextureImpl.inl"
}}}//end of K15_Engine::Rendering::WGL namespace

#endif //_K15Engine_RendererWGL_TextureImpl_h_
