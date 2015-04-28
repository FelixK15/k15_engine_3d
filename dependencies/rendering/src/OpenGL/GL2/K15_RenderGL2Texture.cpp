/*********************************************************************************/
intern inline uint8 K15_InternalIsCompressedRenderFormat(K15_RenderFormat p_RenderFormat)
{
	uint8 result = FALSE;

	switch (p_RenderFormat)
	{
	case K15_RENDER_FORMAT_RGB_DXT_1:
	case K15_RENDER_FORMAT_RGBA_DXT_1:
	case K15_RENDER_FORMAT_RGBA_DXT_3:
	case K15_RENDER_FORMAT_RGBA_DXT_5:
		{
			result = TRUE;
			break;
		}
	}

	return result;
}
/*********************************************************************************/
intern inline void K15_InternalCreateManualMipmapTexture(K15_GLTexture* p_GLTexture, K15_RenderFormat p_RenderFormat, GLuint p_GLTextureHandle, uint32 p_MipmapCount, byte** p_CompressedMipmapData, uint32* p_CompressedMipmapDataSize)
{
	uint32 width = p_GLTexture->width;
	uint32 height = p_GLTexture->height;
	uint32 depth = p_GLTexture->depth;

	GLenum glTextureType = p_GLTexture->glTextureTarget;
	GLenum glFormat = p_GLTexture->glFormat;
	GLenum glInternalFormat = p_GLTexture->glInternalFormat;
	GLenum glType = p_GLTexture->glFormatType;;

	for (uint32 mipmapIndex = 0;
		mipmapIndex < p_MipmapCount;
		++mipmapIndex)
	{
		if (glTextureType == GL_TEXTURE_1D)
		{
			if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
			{
				K15_OPENGL_CALL(kglCompressedTextureImage1DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glInternalFormat, width, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				K15_OPENGL_CALL(kglTextureImage1DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glFormat, width, 0, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
			}

			width = (uint32)((float)width * 0.5f);

			if (width == 0)
			{
				break;
			}
		}
		else if (glTextureType == GL_TEXTURE_2D)
		{
			if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
			{
				K15_OPENGL_CALL(kglCompressedTextureImage2DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glInternalFormat, width, height, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				K15_OPENGL_CALL(kglTextureImage2DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glFormat, width, height, 0, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
			}

			width = (uint32)((float)width * 0.5f);
			height = (uint32)((float)height * 0.5f);

			if (height == 0 || width == 0)
			{
				break;
			}
		}
		else if (glTextureType == GL_TEXTURE_3D)
		{
			if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
			{
				K15_OPENGL_CALL(kglCompressedTextureImage3DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glInternalFormat, width, height, depth, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				K15_OPENGL_CALL(kglTextureImage3DEXT(p_GLTextureHandle, glTextureType, mipmapIndex, glFormat, width, height, depth, 0, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
			}

			width = (uint32)((float)width * 0.5f);
			height = (uint32)((float)height * 0.5f);
			depth = (uint32)((float)depth * 0.5f);

			if (height == 0 || width == 0 || depth == 0)
			{
				break;
			}
		}
		else
		{
			assert(false);
		}
	}
}
/*********************************************************************************/
intern inline void K15_InternalCreateAutomaticMipmapTexture(K15_GLTexture* p_GLTexture, K15_RenderFormat p_Format, GLuint p_GLTextureHandle, byte* p_Data, uint32 p_DataSize)
{
	uint32 width = p_GLTexture->width;
	uint32 height = p_GLTexture->height;
	uint32 depth = p_GLTexture->depth;

	GLenum glTextureType = p_GLTexture->glTextureTarget;
	GLenum glFormat = p_GLTexture->glFormat;
	GLenum glInternalFormat = p_GLTexture->glInternalFormat;
	GLenum glType = p_GLTexture->glFormatType;

	if (glTextureType == GL_TEXTURE_1D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_Format) == TRUE)
		{
			K15_OPENGL_CALL(kglCompressedTextureImage1DEXT(p_GLTextureHandle, glTextureType, 0, glInternalFormat, width, 0, p_DataSize, p_Data));
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage1DEXT(p_GLTextureHandle, glTextureType, 0, glFormat, width, 0, glFormat, glType, p_Data));
		}
	}
	else if(glTextureType == GL_TEXTURE_2D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_Format) == TRUE)
		{
			K15_OPENGL_CALL(kglCompressedTextureImage2DEXT(p_GLTextureHandle, glTextureType, 0, glInternalFormat, width, height, 0, p_DataSize, p_Data));
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage2DEXT(p_GLTextureHandle, glTextureType, 0, glFormat, width, height, 0, glFormat, glType, p_Data));
		}
	}
	else if(glTextureType == GL_TEXTURE_3D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_Format) == TRUE)
		{
			K15_OPENGL_CALL(kglCompressedTextureImage3DEXT(p_GLTextureHandle, glTextureType, 0, glInternalFormat, width, height, depth, 0, p_DataSize, p_Data));
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage3DEXT(p_GLTextureHandle, glTextureType, 0, glFormat, width, height, depth, 0, glFormat, glType, p_Data));
		}
	}
	else
	{
		assert(false);
	}

	K15_OPENGL_CALL(kglGenerateTextureMipmapEXT(p_GLTextureHandle, glTextureType));
}
/*********************************************************************************/
intern inline uint8 K15_GLCreateTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	assert(p_RenderTextureDesc && p_RenderContext);

	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;
	K15_RenderFormat textureFormat = p_RenderTextureDesc->format;

	GLuint glTextureHandle = 0;
	GLenum glTextureType = K15_GLConvertTextureType(p_RenderTextureDesc->type);

	GLenum glInternalFormat = K15_GLConvertInternalRenderFormat(p_RenderTextureDesc->format);
	GLenum glFormat = K15_GLConvertRenderFormat(p_RenderTextureDesc->format);
	GLenum glType = K15_GLConvertRenderFormatType(p_RenderTextureDesc->format);

	uint32 height = p_RenderTextureDesc->dimension.height;
	uint32 width = p_RenderTextureDesc->dimension.width;
	uint32 depth = p_RenderTextureDesc->dimension.depth;

	K15_OPENGL_CALL(glGenTextures(1, &glTextureHandle));
	K15_OPENGL_CALL(glBindTexture(glTextureType, glTextureHandle));

	uint32 mipmapCount = p_RenderTextureDesc->mipmaps.count;

	//Create gl texture struct
	K15_GLTexture glTexture = {};

	glTexture.width = width;
	glTexture.height = height;
	glTexture.depth = depth;

	glTexture.glTextureTarget = glTextureType;
	glTexture.glFormat = glFormat;
	glTexture.glInternalFormat = glInternalFormat;
	glTexture.glFormatType = glType;

	glTexture.glTexture = glTextureHandle;


	if (p_RenderTextureDesc->createMipChain == FALSE)
	{
		//mipmaps are provided
		K15_InternalCreateManualMipmapTexture(&glTexture, textureFormat, glTextureHandle, mipmapCount, p_RenderTextureDesc->mipmaps.data, p_RenderTextureDesc->mipmaps.dataSize);
	}
	else
	{
		uint32 dataSize = p_RenderTextureDesc->mipmaps.dataSize ? p_RenderTextureDesc->mipmaps.dataSize[0] : 0;
		byte* data = p_RenderTextureDesc->mipmaps.data[0];
		//mipmaps are not provided, generate automatically.
		K15_InternalCreateAutomaticMipmapTexture(&glTexture, textureFormat, glTextureHandle, p_RenderTextureDesc->mipmaps.data[0], dataSize);
	}


	glContext->glObjects.textures[*p_RenderTextureHandle] = glTexture;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLUpdateTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureUpdateDesc* p_RenderTextureUpdateDesc, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLTexture* glTexture = &glContext->glObjects.textures[*p_RenderTextureHandle];

	uint32 width = p_RenderTextureUpdateDesc->dimension.width;
	uint32 height = p_RenderTextureUpdateDesc->dimension.height;
	uint32 depth = p_RenderTextureUpdateDesc->dimension.depth;

	uint32 x = p_RenderTextureUpdateDesc->offset.x;
	uint32 y = p_RenderTextureUpdateDesc->offset.y;
	uint32 z = p_RenderTextureUpdateDesc->offset.z;

	GLenum glTextureTarget = glTexture->glTextureTarget;
	GLenum glFormat = glTexture->glFormat;
	GLenum glFormatType = glTexture->glFormatType;

	GLuint glTextureHandle = glTexture->glTexture;

	if (glTextureTarget == GL_TEXTURE_1D)
	{
		K15_OPENGL_CALL(kglTextureSubImage1DEXT(glTextureHandle, glTextureTarget, 0, x, width, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureTarget == GL_TEXTURE_2D)
	{
		K15_OPENGL_CALL(kglTextureSubImage2DEXT(glTextureHandle, glTextureTarget, 0, x, y, width, height, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureTarget == GL_TEXTURE_3D)
	{
		K15_OPENGL_CALL(kglTextureSubImage3DEXT(glTextureHandle, glTextureTarget, 0, x, y, z, width, height, depth, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLDeleteTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLTexture* glTexture = &glContext->glObjects.textures[*p_RenderTextureHandle];

	K15_OPENGL_CALL(glDeleteTextures(1, &glTexture->glTexture));

	return K15_SUCCESS;
}
/*********************************************************************************/