/**
 * @file K15_TextureImplOGL.cpp
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

#include "K15_TextureImplOGL.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	const GLuint TextureImplOGL::GLTextureTypeConverter[Texture::TT_COUNT] = {
		GL_TEXTURE_1D,			//TT_1D
		GL_TEXTURE_2D,			//TT_2D
		GL_TEXTURE_3D,			//TT_3D
		GL_TEXTURE_CUBE_MAP		//TT_CUBE_MAP
	};//GLTextureTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLuint TextureImplOGL::GLTextureUsageConverter[Texture::TU_COUNT] = {

	};//GLTextureUsageConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLuint TextureImplOGL::GLInternalFormatConverter[RendererBase::PF_COUNT] = {
		GL_RGB8I,	//PF_RGB_8_I
		GL_RGB8UI,	//PF_RGB_8_UI
		GL_RGB16I,	//PF_RGB_16_I
		GL_RGB16UI, //PF_RGB_16_U
		GL_RGB16F,	//PF_RGB_16_F
		GL_RGB32I,	//PF_RGB_32_I
		GL_RGB32UI, //PF_RGB_32_U
		GL_RGB32F,	//PF_RGB_32_F

		GL_RGBA8I,	//PF_RGBA_8_I
		GL_RGBA8UI, //PF_RGBA_8_U
		GL_RGBA16I,	//PF_RGBA_16_I
		GL_RGBA16UI,//PF_RGBA_16_U
		GL_RGBA16F, //PF_RGBA_16_F
		GL_RGBA32I, //PF_RGBA_32_I
		GL_RGBA32UI,//PF_RGBA_32_U
		GL_RGBA32F  //PF_RGBA_32_F
	};//GLInternalFormatConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum TextureImplOGL::GLPixelDataTypeConverter[RendererBase::PF_COUNT] = {
		GL_BYTE,			//PF_RGB_8_I
		GL_UNSIGNED_BYTE,	//PF_RGB_8_UI
		GL_SHORT,			//PF_RGB_16_I
		GL_UNSIGNED_SHORT,	//PF_RGB_16_U
		GL_HALF_FLOAT,		//PF_RGB_16_F
		GL_INT,				//PF_RGB_32_I
		GL_UNSIGNED_INT,	//PF_RGB_32_U
		GL_FLOAT,			//PF_RGB_32_F

		GL_BYTE,			//PF_RGBA_8_I
		GL_UNSIGNED_BYTE,	//PF_RGBA_8_UI
		GL_SHORT,			//PF_RGBA_16_I
		GL_UNSIGNED_SHORT,	//PF_RGBA_16_U
		GL_HALF_FLOAT,		//PF_RGBA_16_F
		GL_INT,				//PF_RGBA_32_I
		GL_UNSIGNED_INT,	//PF_RGBA_32_U
		GL_FLOAT,			//PF_RGBA_32_F
	};//GLPixelDataTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum TextureImplOGL::GLTextureWrapModeConverter[Texture::TWM_COUNT] = {
		GL_CLAMP,					//TWM_CLAMP
		GL_MIRROR_CLAMP_TO_EDGE,	//TWM_MIRROR
		GL_REPEAT					//TWM_REPEAT
	};//GLTextureWrapModeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	TextureImplOGL::TextureImplOGL()
		: m_TextureHandle(0)
	{

	}
	/*********************************************************************************/
	TextureImplOGL::~TextureImplOGL()
	{

	}
	/*********************************************************************************/
	void TextureImplOGL::init(Enum p_TextureType, Enum p_TextureUsage, Enum p_PixelFormat, uint32 p_Width, uint32 p_Height, uint32 p_Depth, uint8 p_MipMapCount)
	{
		if(!m_TextureHandle)
		{
			glGenTextures(1,&m_TextureHandle);
		}
		
		glBindTexture(GLTextureTypeConverter[p_TextureType],m_TextureHandle);

		if(p_TextureType == Texture::TT_1D)
		{
			glTexImage1D(GLTextureTypeConverter[p_TextureType],
				p_MipMapCount,
				GLInternalFormatConverter[p_PixelFormat],
				p_Width,0,
				GLInternalFormatConverter[p_PixelFormat],
				GLPixelDataTypeConverter[p_PixelFormat],
				0);
		}
		else if(p_TextureType == Texture::TT_2D)
		{
			glTexImage2D(GLTextureTypeConverter[p_TextureType],
				p_MipMapCount,
				GLInternalFormatConverter[p_PixelFormat],
				p_Width,p_Height,0,
				GLInternalFormatConverter[p_PixelFormat],
				GLPixelDataTypeConverter[p_PixelFormat],
				0);
		}
		else if(p_TextureType == Texture::TT_3D)
		{
			glTexImage3D(GLTextureTypeConverter[p_TextureType],
				p_MipMapCount,
				GLInternalFormatConverter[p_PixelFormat],
				p_Width,p_Height,p_Depth,0,
				GLInternalFormatConverter[p_PixelFormat],
				GLPixelDataTypeConverter[p_PixelFormat],
				0);
		}
	}
	/*********************************************************************************/
	void TextureImplOGL::shutdown()
	{
		glDeleteTextures(1,&m_TextureHandle);
		m_TextureHandle = 0;
	}
	/*********************************************************************************/
	void TextureImplOGL::setTextureWrapMode(Enum p_TextureType)
	{

	}
	/*********************************************************************************/
	void TextureImplOGL::setTextureType(Enum p_TextureType)
	{

	}
/*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace