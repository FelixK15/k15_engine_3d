/*********************************************************************************/
intern inline bool8 K15_InternalIsCompressedRenderFormat(K15_RenderFormat p_RenderFormat)
{
	bool8 result = FALSE;

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
intern inline void K15_InternalSetTextureUniforms(K15_RenderBackEnd* p_RenderBackEnd, K15_GLTexture* p_GLTexture, 
												  uint8 p_GLTextureSlot)
{
	K15_RenderUniformCache* uniformCache = &p_RenderBackEnd->uniformCache;
	K15_ShaderUniformSemantics uniformSemantic = (K15_ShaderUniformSemantics)(K15_UNIFORM_SEMANTIC_TEX1_DIMENSION + p_GLTextureSlot);
	
	int textureDimension[2] = {(int)p_GLTexture->width, (int)p_GLTexture->height};
	K15_UpdateUniformCacheEntry(uniformCache, uniformSemantic, (byte*)&textureDimension);
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

	//Create texture storage
// 	if (glTextureType == GL_TEXTURE_1D)
// 	{
// 		kglTextureStorage1D(p_GLTextureHandle, p_MipmapCount, glInternalFormat, width);
// 	}
// 	else if (glTextureType == GL_TEXTURE_2D)
// 	{
// 		kglTextureStorage2D(p_GLTextureHandle, p_MipmapCount, glInternalFormat, width, height);
// 	}
// 	else 
// 	{
// 		kglTextureStorage3D(p_GLTextureHandle, p_MipmapCount, glInternalFormat, width, height, depth);
// 	}

	K15_OPENGL_CALL(kglBindTexture(glTextureType, p_GLTextureHandle));
	K15_OPENGL_CALL(kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
	K15_OPENGL_CALL(kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, p_MipmapCount));

	for (uint32 mipmapIndex = 0;
		mipmapIndex < p_MipmapCount;
		++mipmapIndex)
	{
		if (glTextureType == GL_TEXTURE_1D)
		{
			if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
			{
				//K15_OPENGL_CALL(kglCompressedTextureSubImage1D(p_GLTextureHandle, mipmapIndex, 0, width, glInternalFormat, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglCompressedTexImage1D(glTextureType, mipmapIndex, glInternalFormat, width, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				//K15_OPENGL_CALL(kglTextureSubImage1D(p_GLTextureHandle, mipmapIndex, 0, width, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglTexImage1D(glTextureType, mipmapIndex, glInternalFormat, width, 0, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
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
				//K15_OPENGL_CALL(kglCompressedTextureSubImage2D(p_GLTextureHandle, mipmapIndex, 0, 0, width, height, glInternalFormat, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglCompressedTexImage2D(glTextureType, mipmapIndex, glInternalFormat, width, height, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				//K15_OPENGL_CALL(kglTextureSubImage2D(p_GLTextureHandle, mipmapIndex, 0, 0, width, height, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglTexImage2D(glTextureType, mipmapIndex, glInternalFormat, width, height, 0, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
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
				//K15_OPENGL_CALL(kglCompressedTextureSubImage3D(p_GLTextureHandle, mipmapIndex, 0, 0, 0, width, height, depth, glInternalFormat, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglCompressedTexImage3D(glTextureType, mipmapIndex, glInternalFormat, width, height, depth, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
			}
			else
			{
				//K15_OPENGL_CALL(kglTextureSubImage3D(p_GLTextureHandle, mipmapIndex, 0, 0, 0, width, height, depth, glFormat, glType, p_CompressedMipmapData[mipmapIndex]));
				K15_OPENGL_CALL(kglCompressedTexImage3D(glTextureType, mipmapIndex, glInternalFormat, width, height, depth, 0, p_CompressedMipmapDataSize[mipmapIndex], p_CompressedMipmapData[mipmapIndex]));
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

	K15_OPENGL_CALL(kglBindTexture(glTextureType, 0));
}
/*********************************************************************************/
intern inline void K15_InternalCreateAutomaticMipmapTexture(K15_GLTexture* p_GLTexture, K15_RenderFormat p_RenderFormat, GLuint p_GLTextureHandle, byte* p_Data, uint32 p_DataSize)
{
	uint32 width = p_GLTexture->width;
	uint32 height = p_GLTexture->height;
	uint32 depth = p_GLTexture->depth;

	uint32 maxValue = max(width,max(height, depth));

	GLenum glTextureType = p_GLTexture->glTextureTarget;
	GLenum glFormat = p_GLTexture->glFormat;
	GLenum glInternalFormat = p_GLTexture->glInternalFormat;
	GLenum glType = p_GLTexture->glFormatType;

	GLint mipmapCount = K15_Log2(maxValue);

	//Create texture storage
// 	if (glTextureType == GL_TEXTURE_1D)
// 	{
// 		kglTextureStorage1D(p_GLTextureHandle, mipmapCount, glInternalFormat, width);
// 	}
// 	else if (glTextureType == GL_TEXTURE_2D)
// 	{
// 		kglTextureStorage2D(p_GLTextureHandle, mipmapCount, glInternalFormat, width, height);
// 	}
// 	else 
// 	{
// 		kglTextureStorage3D(p_GLTextureHandle, mipmapCount, glInternalFormat, width, height, depth);
// 	}

	K15_OPENGL_CALL(kglBindTexture(glTextureType, p_GLTextureHandle));

	if (glTextureType == GL_TEXTURE_1D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
		{
			//K15_OPENGL_CALL(kglCompressedTextureSubImage1D(p_GLTextureHandle, 0, 0, width, glInternalFormat, p_DataSize, p_Data));
			K15_OPENGL_CALL(kglCompressedTexImage1D(glTextureType, 0, glInternalFormat, width, 0, p_DataSize, p_Data));
		}
		else
		{
			//K15_OPENGL_CALL(kglTextureSubImage1D(p_GLTextureHandle, 0, 0, width, glFormat, glType, p_Data));
			K15_OPENGL_CALL(kglTexImage1D(glTextureType, 0, glInternalFormat, width, 0, glFormat, glType, p_Data));
		}
	}
	else if(glTextureType == GL_TEXTURE_2D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
		{
			//K15_OPENGL_CALL(kglCompressedTextureSubImage2D(p_GLTextureHandle, 0, 0, 0, width, height, glInternalFormat, p_DataSize, p_Data));
			K15_OPENGL_CALL(kglCompressedTexImage2D(glTextureType, 0, glInternalFormat, width, height, 0, p_DataSize, p_Data));
		}
		else
		{
			//K15_OPENGL_CALL(kglTextureSubImage2D(p_GLTextureHandle, 0, 0, 0, width, height, glFormat, glType, p_Data));
			K15_OPENGL_CALL(kglTexImage2D(glTextureType, 0, glInternalFormat, width, height, 0, glFormat, glType, p_Data));
		}
	}
	else if(glTextureType == GL_TEXTURE_3D)
	{
		if (K15_InternalIsCompressedRenderFormat(p_RenderFormat))
		{
			//K15_OPENGL_CALL(kglCompressedTextureSubImage3D(p_GLTextureHandle, 0, 0, 0, 0, width, height, depth, glInternalFormat, p_DataSize, p_Data));
			K15_OPENGL_CALL(kglCompressedTexImage3D(glTextureType, 0, glInternalFormat, width, height, depth, 0, p_DataSize, p_Data));
		}
		else
		{
			//K15_OPENGL_CALL(kglTextureSubImage3D(p_GLTextureHandle, 0, 0, 0, 0, width, height, depth, glFormat, glType, p_Data));
			K15_OPENGL_CALL(kglTexImage3D(glTextureType, 0, glInternalFormat, width, height, depth, 0, glFormat, glType, p_Data));
		}
	}
	else
	{
		assert(false);
	}

	K15_OPENGL_CALL(kglGenerateMipmap(glTextureType));

	K15_OPENGL_CALL(kglBindTexture(glTextureType, 0));
}
/*********************************************************************************/
intern inline uint8 K15_GLCreateTextureFromTextureDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderResourceHandle* p_RenderTextureHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_RenderFormat textureFormat = p_RenderTextureDesc->format;

	K15_CustomMemoryAllocator* memoryAllocator = &p_RenderBackEnd->renderContext->memoryAllocator;

	GLuint glTextureHandle = 0;
	GLenum glTextureType = K15_GLConvertTextureType(p_RenderTextureDesc->type);

	GLenum glInternalFormat = K15_GLConvertInternalRenderFormat(p_RenderTextureDesc->format);
	GLenum glFormat = K15_GLConvertRenderFormat(p_RenderTextureDesc->format);
	GLenum glType = K15_GLConvertRenderFormatType(p_RenderTextureDesc->format);

	uint32 height = p_RenderTextureDesc->dimension.height;
	uint32 width = p_RenderTextureDesc->dimension.width;
	uint32 depth = p_RenderTextureDesc->dimension.depth;

	uint32 hash = p_RenderTextureDesc->nameHash;

	K15_OPENGL_CALL(kglGenTextures(1, &glTextureHandle));
	K15_OPENGL_CALL(kglBindTexture(glTextureType, glTextureHandle));

	uint32 mipmapCount = p_RenderTextureDesc->mipmaps.count;

	//Create gl texture struct
	K15_GLTexture glTexture = {};

	glTexture.boundSlot = K15_GL_INVALID_TEXTURE_SLOT;
	glTexture.width = width;
	glTexture.height = height;
	glTexture.depth = depth;

	glTexture.glTextureTarget = glTextureType;
	glTexture.glFormat = glFormat;
	glTexture.glInternalFormat = glInternalFormat;
	glTexture.glFormatType = glType;

	glTexture.glTextureHandle = glTextureHandle;

	glTexture.numMipMaps = mipmapCount;
	glTexture.useAutoMipMaps = p_RenderTextureDesc->createMipChain;

	glTexture.textureData = (GLubyte**)K15_AllocateFromMemoryAllocator(memoryAllocator, mipmapCount * K15_PTR_SIZE);
	glTexture.textureDataSizeInBytes = (GLuint*)K15_AllocateFromMemoryAllocator(memoryAllocator, mipmapCount * sizeof(GLuint));

	//copy mip map texture memory
	for (uint32 mipMapIndex = 0;
		mipMapIndex < (mipmapCount == 0 ? 1 : mipmapCount);
		++mipMapIndex)
	{
		uint32 mipmapSizeInBytes = p_RenderTextureDesc->mipmaps.dataSize[mipMapIndex];
		byte* mipmapData = p_RenderTextureDesc->mipmaps.data[mipMapIndex];

		glTexture.textureData[mipMapIndex] = (GLubyte*)K15_AllocateFromMemoryAllocator(memoryAllocator, mipmapSizeInBytes);

		//copy texture memory
		memcpy(glTexture.textureData[mipMapIndex], mipmapData, mipmapSizeInBytes);
	}

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

	*p_RenderTextureHandle = K15_InternalAddGLObject(glContext, &glTexture, sizeof(glTexture), hash, K15_GL_TYPE_TEXTURE);

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLUpdateTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureUpdateDesc* p_RenderTextureUpdateDesc, K15_RenderResourceHandle* p_RenderTextureHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	K15_GLTexture* glTexture = (K15_GLTexture*)K15_InternalGetGLObjectData(glContext, *p_RenderTextureHandle, K15_GL_TYPE_TEXTURE);

	uint32 width = p_RenderTextureUpdateDesc->dimension.width;
	uint32 height = p_RenderTextureUpdateDesc->dimension.height;
	uint32 depth = p_RenderTextureUpdateDesc->dimension.depth;

	uint32 x = p_RenderTextureUpdateDesc->offset.x;
	uint32 y = p_RenderTextureUpdateDesc->offset.y;
	uint32 z = p_RenderTextureUpdateDesc->offset.z;

	GLenum glTextureTarget = glTexture->glTextureTarget;
	GLenum glFormat = glTexture->glFormat;
	GLenum glFormatType = glTexture->glFormatType;

	GLuint glTextureHandle = glTexture->glTextureHandle;

	if (glTextureTarget == GL_TEXTURE_1D)
	{
		K15_OPENGL_CALL(kglTextureSubImage1D(glTextureHandle, 0, x, width, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureTarget == GL_TEXTURE_2D)
	{
		K15_OPENGL_CALL(kglTextureSubImage2D(glTextureHandle, 0, x, y, width, height, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}
	else if (glTextureTarget == GL_TEXTURE_3D)
	{
		K15_OPENGL_CALL(kglTextureSubImage3D(glTextureHandle, 0, x, y, z, width, height, depth, glFormat, glFormatType, p_RenderTextureUpdateDesc->data));
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline result8 K15_GLBindTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderTextureHandle, K15_RenderResourceHandle* p_RenderSamplerHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_GLTextureManager* glTextureManager = &glContext->textureManager;

	if (*p_RenderTextureHandle == K15_INVALID_GPU_RESOURCE_HANDLE ||
		*p_RenderSamplerHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return K15_SUCCESS;
	}

	K15_GLTexture* glTexture = (K15_GLTexture*)K15_InternalGetGLObjectData(glContext, *p_RenderTextureHandle, K15_GL_TYPE_TEXTURE);
	K15_GLSampler* glSampler = (K15_GLSampler*)K15_InternalGetGLObjectData(glContext, *p_RenderSamplerHandle, K15_GL_TYPE_SAMPLER);

	result8 result = K15_SUCCESS;
	bool8 slotFound = K15_FALSE;

	if (glTexture &&
		glSampler)
	{
		uint8 textureSlot = glTexture->boundSlot;
		uint8 samplerSlot = glSampler->boundSlot;
		uint8 activeSlot = K15_GL_INVALID_TEXTURE_SLOT;

		if (textureSlot == K15_GL_INVALID_TEXTURE_SLOT &&
			samplerSlot != K15_GL_INVALID_TEXTURE_SLOT)
		{
			activeSlot = samplerSlot;
			slotFound = K15_TRUE;
		}
		else if(textureSlot != K15_GL_INVALID_TEXTURE_SLOT &&
			samplerSlot == K15_GL_INVALID_TEXTURE_SLOT)
		{
			activeSlot = textureSlot;
			slotFound = K15_TRUE;
		}
		else if (textureSlot == samplerSlot &&
			textureSlot != K15_GL_INVALID_TEXTURE_SLOT &&
			samplerSlot != K15_GL_INVALID_TEXTURE_SLOT)
		{
			slotFound = K15_FALSE;
		}
		else
		{
			uint32 numSlots = glTextureManager->numTextureSlots;

			if (numSlots < glTextureManager->maxTextureSlots)
			{
				slotFound = K15_TRUE;
				activeSlot = numSlots;
				++glTextureManager->numTextureSlots;
			}
			else
			{
				result = K15_ERROR_TOO_MANY_TEXTURES_ACTIVE;
			}
		}

		if (slotFound)
		{
			K15_GLTextureSlot* slot = &glTextureManager->textureSlots[activeSlot];

			if (activeSlot != glTexture->boundSlot)
			{
				GLenum glTextureType = glTexture->glTextureTarget;
				GLuint glTextureHandle = glTexture->glTextureHandle;

				K15_OPENGL_CALL(kglActiveTexture(GL_TEXTURE0 + activeSlot));
				K15_OPENGL_CALL(kglBindTexture(glTextureType, glTextureHandle));

				if (slot->glTexture)
				{
					slot->glTexture->boundSlot = K15_GL_INVALID_TEXTURE_SLOT;
				}

				slot->glTexture = glTexture;

				K15_InternalSetTextureUniforms(p_RenderBackEnd, glTexture, activeSlot);

				if (glTexture->boundSlot != K15_GL_INVALID_TEXTURE_SLOT)
				{
					K15_GLTextureSlot* oldSlot = &glTextureManager->textureSlots[glTexture->boundSlot];
					oldSlot->glTexture = 0;
				}
			}

			if (activeSlot != glSampler->boundSlot)
			{
				GLenum glSamplerHandle = glSampler->glSamplerHandle;
				K15_OPENGL_CALL(kglBindSampler(activeSlot, glSamplerHandle));
				
				if (slot->glSampler)
				{
					slot->glSampler->boundSlot = K15_GL_INVALID_TEXTURE_SLOT;
				}

				slot->glSampler = glSampler;

				if (glSampler->boundSlot != K15_GL_INVALID_TEXTURE_SLOT)
				{
					K15_GLTextureSlot* oldSlot = &glTextureManager->textureSlots[glSampler->boundSlot];
					oldSlot->glSampler = 0;
				}
			}

			glSampler->boundSlot = activeSlot;
			glTexture->boundSlot = activeSlot;
		}
	}
	
	return result;
}
/*********************************************************************************/
intern inline uint8 K15_GLDeleteTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle p_RenderTextureHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_GLTextureManager* glTextureManager = &glContext->textureManager;

	K15_GLTexture* glTexture = (K15_GLTexture*)K15_InternalGetGLObjectData(glContext, p_RenderTextureHandle, K15_GL_TYPE_TEXTURE);
	GLenum glTextureType = glTexture->glTextureTarget;

	if (glTexture->boundSlot != K15_GL_INVALID_TEXTURE_SLOT)
	{
		uint32 boundSlot = glTexture->boundSlot;

		//unbind texture
		K15_OPENGL_CALL(kglActiveTexture(GL_TEXTURE0 + boundSlot));
		K15_OPENGL_CALL(kglBindTexture(glTextureType, 0));

		K15_GLTextureSlot* textureSlot = &glTextureManager->textureSlots[boundSlot];
		textureSlot->glTexture = 0;
		//TODO CHECK IF SLOT IS EMPTY
	}

	K15_OPENGL_CALL(kglDeleteTextures(1, &glTexture->glTextureHandle));

	K15_InternalRemoveGLObject(glContext, p_RenderTextureHandle, K15_GL_TYPE_TEXTURE);

	return K15_SUCCESS;
}
/*********************************************************************************/