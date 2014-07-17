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

#include "K15_OpenGL_TextureImpl.h"
#include "K15_LogManager.h"

#include "K15_RenderTask.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	const GLuint TextureImpl::GLTextureTypeConverter[Texture::TT_COUNT] = {
		GL_TEXTURE_1D,			//TT_1D
		GL_TEXTURE_2D,			//TT_2D
		GL_TEXTURE_3D,			//TT_3D
		GL_TEXTURE_CUBE_MAP		//TT_CUBE_MAP
	};//GLTextureTypeConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLuint TextureImpl::GLFormatConverter[RendererBase::PF_COUNT] = {
		GL_RGB8I,	//PF_RGB_8_I
		GL_RGB8UI,	//PF_RGB_8_UI
		GL_RGB16I,	//PF_RGB_16_I
		GL_RGB16UI, //PF_RGB_16_U
		GL_RGB16F,	//PF_RGB_16_F
		GL_RGB32I,	//PF_RGB_32_I
		GL_RGB32UI, //PF_RGB_32_U
		GL_RGB32F,	//PF_RGB_32_F
		GL_COMPRESSED_RGB_S3TC_DXT1_EXT, //PF_RGB_4_DXT1

		GL_RGBA8I,	//PF_RGBA_8_I
		GL_RGBA8UI, //PF_RGBA_8_U
		GL_RGBA16I,	//PF_RGBA_16_I
		GL_RGBA16UI,//PF_RGBA_16_U
		GL_RGBA16F, //PF_RGBA_16_F
		GL_RGBA32I, //PF_RGBA_32_I
		GL_RGBA32UI,//PF_RGBA_32_U
		GL_RGBA32F,  //PF_RGBA_32_F,

		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, //PF_RGBA_4_DXT1
		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, //PF_RGBA_8_DXT3
		GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  //PF_RGBA_8_DXT5
	};//GLFormatConverter
	/*********************************************************************************/

	/*********************************************************************************/
	const GLenum TextureImpl::GLInternalFormatConverter[RendererBase::PF_COUNT] = {
		GL_RGB,	//PF_RGB_8_I
		GL_RGB,	//PF_RGB_8_UI
		GL_RGB,	//PF_RGB_16_I
		GL_RGB, //PF_RGB_16_U
		GL_RGB,	//PF_RGB_16_F
		GL_RGB,	//PF_RGB_32_I
		GL_RGB, //PF_RGB_32_U
		GL_RGB,	//PF_RGB_32_F
		GL_RGB, //PF_RGB_4_DXT1

		GL_RGBA, //PF_RGBA_8_I
		GL_RGBA, //PF_RGBA_8_U
		GL_RGBA, //PF_RGBA_16_I
		GL_RGBA, //PF_RGBA_16_U
		GL_RGBA, //PF_RGBA_16_F
		GL_RGBA, //PF_RGBA_32_I
		GL_RGBA, //PF_RGBA_32_U
		GL_RGBA, //PF_RGBA_32_F,

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
		GL_HALF_FLOAT,		//PF_RGB_16_F
		GL_INT,				//PF_RGB_32_I
		GL_UNSIGNED_INT,	//PF_RGB_32_U
		GL_FLOAT,			//PF_RGB_32_F
		0,					//PF_RGB_4_DXT1

		GL_BYTE,			//PF_RGBA_8_I
		GL_UNSIGNED_BYTE,	//PF_RGBA_8_UI
		GL_SHORT,			//PF_RGBA_16_I
		GL_UNSIGNED_SHORT,	//PF_RGBA_16_U
		GL_HALF_FLOAT,		//PF_RGBA_16_F
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
		GLenum internalFormat = GLInternalFormatConverter[m_Texture->getPixelFormat()];
		GLenum format = internalFormat;
		GLenum type = GLPixelDataTypeConverter[m_Texture->getPixelFormat()];

		if(target == GL_TEXTURE_1D)
		{
			kglTextureImage1DEXT(m_TextureHandle,target,0,internalFormat,p_Width,0,format,type,0);
			if(p_Data)
			{
				kglTextureSubImage1DEXT(m_TextureHandle,target,0,0,p_Width,format,type,p_Data);
			}
		}
		else if(target == GL_TEXTURE_2D)
		{
			kglTextureImage2DEXT(m_TextureHandle,target,0,internalFormat,p_Width,p_Height,0,format,type,0);
 			if(p_Data)
 			{
				kglTextureSubImage2DEXT(m_TextureHandle,target,0,0,0,p_Width,p_Height,format,type,p_Data);	
 			}
		}
		else if(target == GL_TEXTURE_3D)
		{
			glTextureImage3DEXT(m_TextureHandle,target,0,internalFormat,p_Width,p_Height,p_Depth,0,format,type,0);
			if(p_Data)
			{
				glTextureSubImage3DEXT(m_TextureHandle,target,0,0,0,0,p_Width,p_Height,p_Depth,format,type,p_Data);
			}
		}

		if(p_CreateMipMaps)
		{
			glBindTexture(target,m_TextureHandle);
			glGenerateMipmap(target);
			glBindTexture(target,0);
		}

		return true;
	}
	/*********************************************************************************/
// 	bool TextureImpl::write(byte* p_Pixels, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
// 	{
// 		GLenum target = GLTextureTypeConverter[m_Texture->getTextureType()];
// 		GLenum type = GLPixelDataTypeConverter[m_Texture->getPixelFormat()];
// 		GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];
// 
// 		if(target == GL_TEXTURE_1D)
// 		{
// 			glTextureSubImage1DEXT(m_TextureHandle,target,0,p_OffsetX,p_Width,format,type,p_Pixels);
// 		}
// 		else if(target == GL_TEXTURE_2D)
// 		{
// 			glTextureSubImage2DEXT(m_TextureHandle,target,0,p_OffsetX,p_OffsetY,p_Width,p_Height,format,type,p_Pixels);
// 		}
// 		else if(target == GL_TEXTURE_3D)
// 		{
// 			glTextureSubImage3DEXT(m_TextureHandle,target,0,p_OffsetX,p_OffsetY,p_OffsetZ,p_Width,p_Height,p_Depth,format,type,p_Pixels);
// 		}
// 
// 		return g_Application->getRenderer()->getLastError().empty();
// 	}
// 	/*********************************************************************************/
// 	bool TextureImpl::writeMipmap(byte* p_Pixels, uint32 p_MipmapLevel, uint32 p_Width, uint32 p_Height, uint32 p_Depth, int32 p_OffsetX, int32 p_OffsetY, int32 p_OffsetZ)
// 	{
// 		GLenum target = GLTextureTypeConverter[m_Texture->getTextureType()];
// 		GLenum type = GLPixelDataTypeConverter[m_Texture->getPixelFormat()];
// 		GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];
// 
// 		if(target == GL_TEXTURE_1D)
// 		{
// 			if(type == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || 
// 				type == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
// 			{
// 				uint32 blocksize = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
// 				GLsizei size = ((p_Width + 3) / 4) * blocksize;
// 
// 				glCompressedTextureSubImage1DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_Width,format,size,p_Pixels);
// 			}
// 			else
// 			{
// 				glTextureSubImage1DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_Width,format,type,p_Pixels);
// 			}
// 		}
// 		else if(target == GL_TEXTURE_2D)
// 		{
// 			if(type == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || 
// 				type == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
// 			{
// 				uint32 blocksize = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
// 				GLsizei size = ((p_Width + 3) / 4) * ((p_Height + 3) / 4) * blocksize;
// 
// 				glCompressedTextureSubImage2DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_Width,p_Height,format,size,p_Pixels);
// 			}
// 			else
// 			{
// 				glTextureSubImage2DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_Width,p_Height,format,type,p_Pixels);
// 			}
// 		}
// 		else if(target == GL_TEXTURE_3D)
// 		{
// 			if(type == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || 
// 				type == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || type == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
// 			{
// 				uint32 blocksize = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
// 				GLsizei size = ((p_Width + 3) / 4) * ((p_Height + 3) / 4) * ((p_Depth + 3) / 4) * blocksize;
// 
// 				glCompressedTextureSubImage3DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_OffsetZ,p_Width,p_Height,p_Depth,format,size,p_Pixels);
// 			}
// 			else
// 			{
// 				glTextureSubImage3DEXT(m_TextureHandle,target,p_MipmapLevel,p_OffsetX,p_OffsetY,p_OffsetZ,p_Width,p_Height,p_Depth,format,type,p_Pixels);
// 			}
// 		}
// 
// 		if(glGetError() != GL_NO_ERROR)
// 		{
// 			_LogError("Could not upload texture data to texture %s. %s",m_Texture->getName().c_str(),
// 				g_Application->getRenderTask()->getRenderer()->getLastError().c_str());
// 
// 			return false;
// 		}
// 
// 		return true;
// 	}
// 	/*********************************************************************************/
// 	bool TextureImpl::resize(uint32 p_Width, uint32 p_Height, uint32 p_Depth)
// 	{
// 		GLenum target = GLTextureTypeConverter[m_Texture->getTextureType()];
// 		GLenum format = GLInternalFormatConverter[m_Texture->getPixelFormat()];
// 		
// 		if(target == GL_TEXTURE_1D)
// 		{
// 			glTextureStorage1DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width);
// 		}
// 		else if(target == GL_TEXTURE_2D)
// 		{
// 			glTextureStorage2DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width,p_Height);
// 		}
// 		else if(target == GL_TEXTURE_3D)
// 		{
// 			glTextureStorage3DEXT(m_TextureHandle,target,m_Texture->getMipmapLevels(),format,p_Width,p_Height,p_Depth);
// 		}
// 
// 		if(glGetError() != GL_NO_ERROR)
// 		{
// 			_LogError("Could not resize texture %s. %s",m_Texture->getName().c_str(),
// 				g_Application->getRenderTask()->getRenderer()->getLastError().c_str());
// 
// 			return false;
// 		}
// 
// 		return true;
// 	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::WGL namespace