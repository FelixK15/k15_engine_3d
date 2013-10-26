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

#ifndef _K15Engine_RendererOGL_TextureImpl_h_
#define _K15Engine_RendererOGL_TextureImpl_h_

#include "K15_RendererOGLPrerequisites.h"
#include "K15_RendererBase.h"
#include "K15_Texture.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	class TextureImplOGL : public TextureImplBase
	{
	public:
		/*********************************************************************************/
		static const GLuint GLTextureTypeConverter[Texture::TT_COUNT];
		static const GLuint GLTextureUsageConverter[Texture::TU_COUNT];
		static const GLuint GLInternalFormatConverter[RendererBase::PF_COUNT];
		static const GLenum GLPixelDataTypeConverter[RendererBase::PF_COUNT];
		static const GLenum GLTextureWrapModeConverter[Texture::TWM_COUNT];
		/*********************************************************************************/
	public:
		TextureImplOGL();
		~TextureImplOGL();

		virtual void init(Enum p_TextureType, Enum p_TextureUsage, Enum p_PixelFormat, uint32 p_Width, uint32 p_Height, uint32 p_Depth, uint8 p_MipMapCount) OVERRIDE;
		virtual void shutdown() OVERRIDE;

		virtual void setTextureWrapMode(Enum p_TextureType) OVERRIDE;
		virtual void setTextureType(Enum p_TextureType) OVERRIDE;
		virtual void setTextureUsage(Enum p_TextureUsage) OVERRIDE;

		virtual void setMipMapCount(uint8 p_MipMapCount) OVERRIDE;

		virtual void setHeight(uint32 p_Height) OVERRIDE;
		virtual void setWidth(uint32 p_Width) OVERRIDE;
		virtual void setDepth(uint32 p_Depth) OVERRIDE;

		virtual void setPixelFormat(Enum p_PixelFormat) OVERRIDE;
		virtual bool hasAlpha() const OVERRIDE;
	private:
		GLuint m_TextureHandle;
	};// end of TextureImplOGL class declaration
}}}//end of K15_Engine::Rendering::OGL namespace

#endif //_K15Engine_RendererOGL_TextureImpl_h_