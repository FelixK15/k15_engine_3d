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
  bool TextureImplOGL::write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
  {
    GLenum target = GLTextureTypeConverter[m_Texture->getType()];
    GLenum type = GLPixelDataTypeConverter[m_Texture->getPixelFormat()];
    GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];

    if(target == GL_TEXTURE_1D)
    {
      glTextureSubImage1DEXT(m_TextureHandle,target,0,p_OffsetX,p_Width,format,type,p_Pixels);
    }
    else if(target == GL_TEXTURE_2D)
    {
      glTextureSubImage2DEXT(m_TextureHandle,target,0,p_OffsetX,p_OffsetY,p_Width,p_Height,format,type,p_Pixels);
    }
    else if(target == GL_TEXTURE_3D)
    {
      glTextureSubImage3DEXT(m_TextureHandle,target,0,p_OffsetX,p_OffsetY,p_OffsetZ,p_Width,p_Height,p_Depth,format,type,p_Pixels);
    }

    glGenerateTextureMipmapEXT(m_TextureHandle,target);

    return true;
  }
  /*********************************************************************************/
  bool TextureImplOGL::writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
  {
    GLenum target = GLTextureTypeConverter[m_Texture->getType()];
    GLenum type = GLPixelDataTypeConverter[m_Texture->getPixelFormat()];
    GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];

    if(target == GL_TEXTURE_1D)
    {
      glTextureSubImage1DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetZ,p_Width,format,type,p_Pixels);
    }
    else if(target == GL_TEXTURE_2D)
    {
      glTextureSubImage2DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_Width,p_Height,format,type,p_Pixels);
    }
    else if(target == GL_TEXTURE_3D)
    {
      glTextureSubImage3DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_OffsetZ,p_Width,p_Height,p_Depth,format,type,p_Pixels);
    }

    return true;
  }
  /*********************************************************************************/
  bool TextureImplOGL::read(byte** p_Destination, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
  {
    return false;
  }
  /*********************************************************************************/
  bool TextureImplOGL::readMipmap(byte** p_Destination, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
  {
    return false;
  }
  /*********************************************************************************/
  bool TextureImplOGL::resize(uint32 p_Width, uint32 p_Height, uint32 p_Depth)
  {
    GLenum target = GLTextureTypeConverter[m_Texture->getType()];
    GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];

    if(target == GL_TEXTURE_1D)
    {
      glTextureStorage1DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width);
    }
    else if(target == GL_TEXTURE_2D)
    {
      glTextureStorage2DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width,p_Height);
    }
    else if(target == GL_TEXTURE_3D)
    {
      glTextureStorage3DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width,p_Height,p_Depth);
    }

    return true;
  }
  /*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace