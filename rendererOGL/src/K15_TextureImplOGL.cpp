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
#include "K15_LogManager.h"

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
	TextureImplOGL::TextureImplOGL()
		: m_TextureHandle(0),
		  m_TextureStorageSize(0)
	{
		glGenTextures(1,&m_TextureHandle);
	}
	/*********************************************************************************/
	TextureImplOGL::~TextureImplOGL()
	{
		glDeleteTextures(1,&m_TextureHandle);
	}
	/*********************************************************************************/
	uint32 TextureImplOGL::writeData(uint32 p_Size,byte* p_Source,uint32 p_Offset)
	{
		bool recreate = false;
		GLenum textureType = GLTextureTypeConverter[m_Texture->getTextureType()];
		GLenum formatType = GLInternalFormatConverter[m_Texture->getPixelFormat()];

		uint32 width,height,depth;
		width = m_Texture->getWidth();
		height = m_Texture->getHeight();
		depth = m_Texture->getDepth();

		uint32 newSize = m_Texture->getSize();
		if(newSize != m_TextureStorageSize)
		{
			_LogWarning("Texture size changed from %u to %u. Recreating texture storage.",m_TextureStorageSize,newSize);
			m_TextureStorageSize = newSize;
			recreate = true;
		}

		if(recreate)
		{
			deleteStorage();
			createStorage();
		}

		glBindTexture(textureType,0);

		return p_Size;
	}
	/*********************************************************************************/
	void TextureImplOGL::createStorage()
	{
		GLenum textureType = GLTextureTypeConverter[m_Texture->getTextureType()];
		GLenum formatType = GLInternalFormatConverter[m_Texture->getPixelFormat()];

		uint32 width,height,depth;
		width = m_Texture->getWidth();
		height = m_Texture->getHeight();
		depth = m_Texture->getDepth();

		glBindTexture(textureType,m_TextureHandle);

		for(int i = 0;i < m_Texture->getMipMapCount();++i)
		{
			if(width == 1 || height == 1 || depth == 1)
			{
				_LogError("Can't create mip map level %i and following (up to level %i). Dimension is already 1.");
				break;
			}

			if(textureType == GL_TEXTURE_1D)
			{
				glTexStorage1D(textureType,i,formatType,width);
				width = max(1,(uint32)width * 0.5);
			}
			else if(textureType == GL_TEXTURE_2D)
			{
				glTexStorage2D(textureType,i,formatType,width,height);
				width = max(1,(uint32)width * 0.5);
				height = max(1,(uint32)height * 0.5);
			}
			else if(textureType == GL_TEXTURE_3D)
			{
				glTexStorage3D(textureType,i,formatType,width,height,depth);
				width = max(1,(uint32)width * 0.5);
				height = max(1,(uint32)height * 0.5);
				depth = max(1,(uint32)depth * 0.5);
			}
		}

		glBindTexture(textureType,0);
	}
	/*********************************************************************************/
	void TextureImplOGL::deleteStorage()
	{
		for(int i = 0;i < m_Texture->getMipMapCount();++i)
		{
			glInvalidateTexImage(m_TextureHandle,i);
		}
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace