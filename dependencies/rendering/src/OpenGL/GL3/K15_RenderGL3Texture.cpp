#define K15_RENDER_GL_MAX_TEXTURE_COUNT 32

struct K15_GLTextureDesc
{
	K15_RenderTextureDesc desc;
	GLuint texture;
};

intern K15_GLTextureDesc internOpenGLTextures[K15_RENDER_GL_MAX_TEXTURE_COUNT] = {};

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
intern inline uint8 K15_GLCreateTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	assert(p_RenderTextureDesc && p_RenderContext);

	GLuint glTextureHandle = 0;
	GLenum glTextureType = K15_GLConvertTextureType(p_RenderTextureDesc->type);
	GLenum glInternalFormat = K15_GLConvertInternalRenderFormat(p_RenderTextureDesc->format);
	GLenum glFormat = K15_GLConvertRenderFormat(p_RenderTextureDesc->format);
	GLenum glType = K15_GLConvertRenderFormatType(p_RenderTextureDesc->format);

	K15_OPENGL_CALL(glGenTextures(1, &glTextureHandle));
	K15_OPENGL_CALL(glBindTexture(glTextureType, glTextureHandle));

	uint32 height = p_RenderTextureDesc->dimension.height;
	uint32 width = p_RenderTextureDesc->dimension.width;
	uint32 depth = p_RenderTextureDesc->dimension.depth;

	if (p_RenderTextureDesc->type == K15_RENDER_TEXTURE_TYPE_1D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderTextureDesc->format) == TRUE)
		{
			//TODO
			assert(false);
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage1DEXT(glTextureHandle, glTextureType, 0, glFormat, width, 0, glFormat, glType, p_RenderTextureDesc->data));
		}
	}
	else if(p_RenderTextureDesc->type == K15_RENDER_TEXTURE_TYPE_2D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderTextureDesc->format) == TRUE)
		{
			//TODO
			assert(false);
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage2DEXT(glTextureHandle, glTextureType, 0, glFormat, width, height, 0, glFormat, glType, p_RenderTextureDesc->data));
		}
	}
	else if(p_RenderTextureDesc->type == K15_RENDER_TEXTURE_TYPE_2D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderTextureDesc->format) == TRUE)
		{
			//TODO
			assert(false);
		}
		else
		{
			K15_OPENGL_CALL(kglTextureImage3DEXT(glTextureHandle, glTextureType, 0, glFormat, width, height, depth, 0, glFormat, glType, p_RenderTextureDesc->data));
		}
	}
	else
	{
		assert(false);
	}

	if (p_RenderTextureDesc->createMipChain == TRUE)
	{
		K15_OPENGL_CALL(kglGenerateTextureMipmapEXT(glTextureHandle, glTextureType));
	}

	K15_GLTextureDesc textureData = {};

	textureData.desc = *p_RenderTextureDesc;
	textureData.texture = glTextureHandle;

	internOpenGLTextures[*p_RenderTextureHandle] = textureData;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLUpdateTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureUpdateDesc* p_RenderTextureUpdateDesc, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	K15_GLTextureDesc* glTextureDesc = &internOpenGLTextures[*p_RenderTextureHandle];

	uint32 width = p_RenderTextureUpdateDesc->dimension.width;
	uint32 height = p_RenderTextureUpdateDesc->dimension.height;
	uint32 depth = p_RenderTextureUpdateDesc->dimension.depth;

	uint32 x = p_RenderTextureUpdateDesc->offset.x;
	uint32 y = p_RenderTextureUpdateDesc->offset.y;
	uint32 z = p_RenderTextureUpdateDesc->offset.z;

	GLenum glTextureTarget = K15_GLConvertTextureType(glTextureDesc->desc.type);
	GLenum glFormat = K15_GLConvertRenderFormat(glTextureDesc->desc.format);
	GLenum glFormatType = K15_GLConvertRenderFormatType(glTextureDesc->desc.format);

	if (glTextureDesc->desc.type == K15_RENDER_TEXTURE_TYPE_1D)
	{
		K15_OPENGL_CALL(kglTextureSubImage1DEXT(glTextureDesc->texture, glTextureTarget, 0, x, width, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureDesc->desc.type == K15_RENDER_TEXTURE_TYPE_2D)
	{
		K15_OPENGL_CALL(kglTextureSubImage2DEXT(glTextureDesc->texture, glTextureTarget, 0, x, y, width, height, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureDesc->desc.type == K15_RENDER_TEXTURE_TYPE_3D)
	{
		K15_OPENGL_CALL(kglTextureSubImage3DEXT(glTextureDesc->texture, glTextureTarget, 0, x, y, z, width, height, depth, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLDeleteTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	K15_GLTextureDesc* glTextureDesc = &internOpenGLTextures[*p_RenderTextureHandle];

	K15_OPENGL_CALL(glDeleteTextures(1, &glTextureDesc->texture));

	return K15_SUCCESS;
}
/*********************************************************************************/