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
		/*********************************************************************************/
	public:
		TextureImplOGL();
		~TextureImplOGL();

		virtual uint32 write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ) OVERRIDE;
		virtual uint32 readData(uint32 p_Size,byte* p_Destination,uint32 p_Offset) OVERRIDE{return 0;}

		void createStorage();
		void deleteStorage();
	private:
		GLuint m_TextureHandle;
		uint32 m_TextureStorageSize;
	};// end of TextureImplOGL class declaration
}}}//end of K15_Engine::Rendering::OGL namespace

#endif //_K15Engine_RendererOGL_TextureImpl_h_