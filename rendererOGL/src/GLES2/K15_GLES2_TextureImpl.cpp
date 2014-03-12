/**
 * @file K15_TextureImpl.cpp
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
 */

#include "K15_GLES2_TextureImpl.h"
#include "K15_LogManager.h"

#include "K15_RenderTask.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
	/*********************************************************************************/
	const GLuint TextureImpl::GLTextureTypeConverter[Texture::TT_COUNT] = {
		GL_NONE,			//TT_1D			//not for GLES 2.0
		GL_TEXTURE_2D,		//TT_2D
		GL_NONE,			//TT_3D			//not for GLES 2.0
		GL_TEXTURE_CUBE_MAP	//TT_CUBE_MAP
	};//GLTextureTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLuint TextureImpl::GLInternalFormatConverter[RendererBase::PF_COUNT] = {
		GL_RGB,	//PF_RGB_8_I
		GL_RGB,	//PF_RGB_8_UI
		GL_RGB,	//PF_RGB_16_I
		GL_RGB,	//PF_RGB_16_U
		GL_RGB,		//PF_RGB_16_F   
		GL_RGB,		//PF_RGB_32_I
		GL_RGB,		//PF_RGB_32_U
		GL_RGB,		//PF_RGB_32_F
		GL_RGB, //PF_RGB_4_DXT1

		GL_RGBA,	//PF_RGBA_8_I
		GL_RGBA, //PF_RGBA_8_U
		GL_RGBA,	//PF_RGBA_16_I
		GL_RGBA,//PF_RGBA_16_U
		GL_RGBA, //PF_RGBA_16_F
		GL_RGBA, //PF_RGBA_32_I
		GL_RGBA,//PF_RGBA_32_U
		GL_RGBA,  //PF_RGBA_32_F,

		GL_RGBA, //PF_RGBA_4_DXT1
		GL_RGBA, //PF_RGBA_8_DXT3
		GL_RGBA  //PF_RGBA_8_DXT5
	};//GLInternalFormatConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum TextureImpl::GLPixelDataTypeConverter[RendererBase::PF_COUNT] = {
		GL_BYTE,			//PF_RGB_8_I
		GL_UNSIGNED_BYTE,	//PF_RGB_8_UI
		GL_SHORT,			//PF_RGB_16_I
		GL_UNSIGNED_SHORT,	//PF_RGB_16_U
		GL_HALF_FLOAT_OES,	//PF_RGB_16_F
		GL_INT,				//PF_RGB_32_I
		GL_UNSIGNED_INT,	//PF_RGB_32_U
		GL_FLOAT,			//PF_RGB_32_F
		0,					//PF_RGB_4_DXT1

		GL_BYTE,			//PF_RGBA_8_I
		GL_UNSIGNED_BYTE,	//PF_RGBA_8_UI
		GL_SHORT,			//PF_RGBA_16_I
		GL_UNSIGNED_SHORT,	//PF_RGBA_16_U
		GL_HALF_FLOAT_OES,	//PF_RGBA_16_F
		GL_INT,				//PF_RGBA_32_I
		GL_UNSIGNED_INT,	//PF_RGBA_32_U
		GL_FLOAT,			//PF_RGBA_32_F
		0,					//PF_RGBA_4_DXT1
		0,					//PF_RGBA_8_DXT3
		0					//PF_RGBA_8_DXT5
	};//GLPixelDataTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	TextureImpl::TextureImpl()
		: m_TextureHandle(0),
		  m_TextureStorageSize(0)
	{
		glGenTextures(1,&m_TextureHandle);
	}
	/*********************************************************************************/
	TextureImpl::~TextureImpl()
	{
		glDeleteTextures(1,&m_TextureHandle);
	}
	/*********************************************************************************/
	bool TextureImpl::loadRawData(byte* p_Data, Enum p_Format, uint32 p_Width, uint32 p_Height, uint32 p_Depth, bool p_CreateMipMaps)
	{
		GLenum target = GLTextureTypeConverter[m_Texture->getTextureType()];
		GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];
		GLenum type = GLTextureTypeConverter[m_Texture->getPixelFormat()];
		
		if(m_Texture->getTextureType() == Texture::TT_1D)
		{
			p_Height = 1;
			target = GL_TEXTURE_2D;
		}

		if(target == GL_TEXTURE_2D)
		{
			glBindTexture(target,m_TextureHandle);
			glTexImage2D(target,0,format,p_Width,p_Height,0,format,type,0);
			glTexSubImage2D(target,0,0,0,p_Width,p_Height,format,type,p_Data);

			if(p_CreateMipMaps)
			{
				glGenerateMipmap(target);
			}

			glBindTexture(target,0);
		}

		return true;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::GLES2 namespace