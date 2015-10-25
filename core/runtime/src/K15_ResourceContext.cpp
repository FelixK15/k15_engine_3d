#include "K15_ResourceContext.h"
#include "K15_FileSystem.h"
#include "K15_String.h"
#include "K15_OSContext.h"
#include "K15_MemoryBuffer.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_AsyncOperation.h"
#include "K15_System.h"
#include "K15_Logging.h"
#include "K15_Thread.h"
#include "K15_DefaultCLibraries.h"

#include "K15_RenderMaterialDesc.h"
#include "K15_RenderFontDesc.h"

#include "K15_MaterialFormat.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"
#include "K15_SamplerFormat.h"

#include "K15_RenderContext.h"
#include "K15_RenderCommands.h"
#include "K15_RenderShaderSemantics.h"

#include "generated/K15_ResourceStretchBuffer.cpp"

/*********************************************************************************/
struct K15_AsyncResourceLoadParameter
{
	K15_ResourceContext* resourceContext;
	K15_ResourceHandle* resourceHandlePtr;
	const char* resourcePath;
	uint32 resourceIdentifier;
	uint32 flags;
};
/*********************************************************************************/



/*********************************************************************************/
intern char* K15_InternalCopyResourcePathIntoBuffer(const char* p_ResourcePackPath, const char* p_ResourcePath, char* p_Buffer)
{
	uint32 resourcePackPathLength= (uint32)strlen(p_ResourcePackPath);
	uint32 resourcePathLength= (uint32)strlen(p_ResourcePath);
	uint32 bufferOffset = 0;

	uint8 addSeparator = K15_FALSE;

	//check if last character of the pack path and the first character of the resource path is a directory separator
	if (p_ResourcePackPath[resourcePackPathLength - 1] == K15_DIR_SEPARATOR
		&& p_ResourcePath[0] == K15_DIR_SEPARATOR)
	{
		resourcePackPathLength -= 1;
	}
	else if(p_ResourcePackPath[resourcePackPathLength - 1] != K15_DIR_SEPARATOR
		&& p_ResourcePath[0] != K15_DIR_SEPARATOR)
	{
		addSeparator = K15_TRUE;
	}
	
	memcpy(p_Buffer + bufferOffset, p_ResourcePackPath, resourcePackPathLength);
	bufferOffset += resourcePackPathLength;
	
	if (addSeparator == K15_TRUE)
	{
		p_Buffer[bufferOffset++] = K15_DIR_SEPARATOR;
	}

	memcpy(p_Buffer + bufferOffset, p_ResourcePath, resourcePathLength);
	bufferOffset += resourcePathLength;
	
	p_Buffer[bufferOffset] = 0;

	return p_Buffer;
}
/*********************************************************************************/
intern uint8 K15_InternalFileSystemResourceExists(K15_ResourceContext* p_ResourceContext, K15_ResourceArchive* p_ResourceArchive, const char* p_Path)
{
	uint32 dirPathLength= (uint32)strlen(p_ResourceArchive->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceArchive->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_FileExists(resourceFilePath);
}
/*********************************************************************************/
intern uint32 K15_InternalFileSystemGetResourceSize(K15_ResourceContext* p_ResourceContext, K15_ResourceArchive* p_ResourceArchive, const char* p_Path)
{
	uint32 dirPathLength= (uint32)strlen(p_ResourceArchive->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceArchive->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_GetFileSize(resourceFilePath);
}
/*********************************************************************************/
intern byte* K15_InternalFileSystemLoadResource(K15_ResourceContext* p_ResourceContext, K15_ResourceArchive* p_ResourceArchive, uint32 p_ResourceSize, const char* p_Path)
{
	K15_CustomMemoryAllocator* resourceMemoryAllocator = &p_ResourceContext->memoryAllocator;

	uint32 dirPathLength= (uint32)strlen(p_ResourceArchive->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceArchive->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	byte* fileContentBuffer = (byte*)K15_AllocateFromMemoryAllocator(resourceMemoryAllocator, p_ResourceSize);
	K15_CopyWholeFileContentIntoBuffer(resourceFilePath, fileContentBuffer);

	return fileContentBuffer;
}
/*********************************************************************************/
intern void K15_InternalAsyncResourceLoad(void* p_Parameter)
{
	K15_AsyncResourceLoadParameter* loadParamter = (K15_AsyncResourceLoadParameter*)p_Parameter;

	K15_ResourceContext* context = loadParamter->resourceContext;
	K15_ResourceHandle* resourceHandlePtr = loadParamter->resourceHandlePtr;
	const char* path = loadParamter->resourcePath;
	uint32 resourceIdentifier = loadParamter->resourceIdentifier;
	uint32 flags = loadParamter->flags;

	K15_CustomMemoryAllocator* customAllocator = &context->memoryAllocator;

	(*resourceHandlePtr) = K15_LoadResource(context, resourceIdentifier, path, flags);

	//clean parameter
	K15_FreeFromMemoryAllocator(customAllocator, p_Parameter);
}
/*********************************************************************************/
intern void K15_InternalInitializeResourceArchive(K15_ResourceArchive* p_ResourceArchive, const char* p_ResourceArchivePath)
{
	char* resourcePath = K15_ConvertToAbsolutePath(p_ResourceArchivePath);
	p_ResourceArchive->resourcePath = resourcePath;

	//check if the resource context path is a directory or file. 
	if (K15_DirectoryExists(resourcePath) == K15_TRUE)
	{
		p_ResourceArchive->getResourceData = K15_InternalFileSystemLoadResource;
		p_ResourceArchive->resourceExists = K15_InternalFileSystemResourceExists;
		p_ResourceArchive->getResourceSize = K15_InternalFileSystemGetResourceSize;
	}
	else
	{
		//TODO: CHECK FOR PACK FILE EXTENSION
	}
}
/*********************************************************************************/
intern void K15_InternalInitializeResourceContext(K15_ResourceContext* p_ResourceContext, K15_RenderContext* p_RenderContext, 
												  K15_CustomMemoryAllocator p_MemoryAllocator, const char* p_ArchivePath)
{
	//Create resource cache
// 	K15_ResourceStretchBuffer resourceCache = {};
// 	K15_CreateResourceStretchBufferWithCustomAllocator(&resourceCache, p_MemoryAllocator);
	K15_InternalInitializeResourceArchive(&p_ResourceContext->resourceArchive, p_ArchivePath);

	memset(p_ResourceContext->resourceCache, 0, sizeof(K15_PTR_SIZE) * K15_MAX_RESOURCES);

	p_ResourceContext->memoryAllocator = p_MemoryAllocator;
	p_ResourceContext->resourceCacheLock = K15_CreateMutex();
	p_ResourceContext->renderContext = p_RenderContext;
	p_ResourceContext->commandQueue = K15_CreateRenderCommandQueue(p_RenderContext, "Resource Loading Queue");
	//p_ResourceContext->flags = p_Flags;
	//p_ResourceContext->resourceCache = resourceCache;
	p_ResourceContext->numResourceLoader = 0;
}
/*********************************************************************************/
intern bool8 K15_InternalIsValidArchive(const char* p_ArchivePath)
{
	bool8 validArchive = K15_FALSE;

	char* absoluteArchivePath = K15_ConvertToAbsolutePath(p_ArchivePath);

	if (K15_DirectoryExists(absoluteArchivePath) == K15_TRUE)
	{
		validArchive = K15_TRUE;
	}

	free(absoluteArchivePath);

	return validArchive;
}
/*********************************************************************************/
intern void K15_InternalLoadTextureResource(K15_ResourceContext* p_ResourceContext, 
											K15_ResourceCompilerInput* p_ResourceCompilerInput,
											K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_TextureFormat textureFormat = {};

	byte* resourceFileContent = p_ResourceCompilerInput->fileContent;
	uint32 resourceFileSize = p_ResourceCompilerInput->fileContentSizeInBytes;
	result8 textureLoadResult = K15_LoadTextureFormatFromMemory(&textureFormat, resourceFileContent, resourceFileSize);

	if (textureLoadResult != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load texture '%s'.", p_ResourceCompilerInput->path);
		//load default texture
	}

	K15_RenderResourceHandle* textureRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateTextureFromTextureFormat(commandQueue, textureRenderHandle, &textureFormat);

	p_ResourceCompilerOutput->compiledResourceData = (byte*)textureRenderHandle;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = sizeof(K15_RenderResourceHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadSamplerResource(K15_ResourceContext* p_ResourceContext, 
											K15_ResourceCompilerInput* p_ResourceCompilerInput,
											K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_SamplerFormat samplerFormat = {};

	byte* resourceFileContent = p_ResourceCompilerInput->fileContent;
	uint32 resourceFileSize = p_ResourceCompilerInput->fileContentSizeInBytes;
	result8 samplerLoadResult = K15_LoadSamplerFormatFromMemory(&samplerFormat, resourceFileContent, resourceFileSize);

	if (samplerLoadResult != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load sampler '%s'.", p_ResourceCompilerInput->path);
		//load default sampler
	}

	K15_RenderResourceHandle* samplerResourceHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateSamplerFromSamplerFormat(commandQueue, samplerResourceHandle, &samplerFormat);

	p_ResourceCompilerOutput->compiledResourceData = (byte*)samplerResourceHandle;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = sizeof(K15_RenderResourceHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadFontResource(K15_ResourceContext* p_ResourceContext, 
										 K15_ResourceCompilerInput* p_ResourceCompilerInput,
										 K15_ResourceData* p_ResourceCompilerOutput)
{
	result8 result = K15_SUCCESS;

	K15_RenderContext* renderContext = p_ResourceContext->renderContext;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_FontFormat fontFormat = {};

	byte* resourceFileContent = p_ResourceCompilerInput->fileContent;
	uint32 resourceFileSize = p_ResourceCompilerInput->fileContentSizeInBytes;
	result = K15_LoadFontFormatFromMemory(&fontFormat, resourceFileContent, resourceFileSize);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load font '%s'.", p_ResourceCompilerInput->path);
		//load default sampler
	}

	K15_ResourceHandle blaresource = K15_LoadResource(p_ResourceContext, K15_SAMPLER_RESOURCE_IDENTIFIER, "linear_clamp_sampler.k15sampler", 0);

	K15_RenderFontDesc* renderFontDesc = (K15_RenderFontDesc*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderFontDesc));
	renderFontDesc->textureHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	renderFontDesc->bla = K15_GetResourceRenderHandle(p_ResourceContext, blaresource);

	uint32 fontTextureSize = K15_GetFontTextureSize(&fontFormat);
	byte* fontTextureData = K15_GetFontTexture(&fontFormat);

	K15_RenderTextureDesc fontTextureDesc = {};

	fontTextureDesc.createMipChain = K15_TRUE;
	fontTextureDesc.dimension.width = fontFormat.texture.width;
	fontTextureDesc.dimension.height = fontFormat.texture.height;
	fontTextureDesc.format = K15_RENDER_FORMAT_R8_UINT;
	fontTextureDesc.mipmaps.count = 1;
	fontTextureDesc.mipmaps.data[0] = fontTextureData;
	fontTextureDesc.mipmaps.dataSize[0] = fontTextureSize;
	fontTextureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;

	K15_RenderCommandCreateTexture(commandQueue, renderFontDesc->textureHandle, &fontTextureDesc);

	if (result == K15_SUCCESS)
	{
		renderFontDesc->startCharacter = fontFormat.startCharacter;
		renderFontDesc->endCharacter = fontFormat.endCharacter;
		renderFontDesc->fontNameHash = fontFormat.fontNameHash;
		renderFontDesc->scaleFactor = fontFormat.scaleFactor;

		uint32 numGlyphs = fontFormat.endCharacter - fontFormat.startCharacter;
		uint32 numKerning = numGlyphs * numGlyphs;

		renderFontDesc->glyphDescs = (K15_RenderGlyphDesc*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderGlyphDesc) * numGlyphs);
		renderFontDesc->kernDescs = (K15_RenderKerningDesc*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderKerningDesc) * numKerning);
		renderFontDesc->textureHeight = fontFormat.texture.height;
		renderFontDesc->textureWidth = fontFormat.texture.width;

		//transfer glyph data
		for (uint32 glyphIndex = fontFormat.startCharacter;
			glyphIndex < fontFormat.endCharacter;
			++glyphIndex)
		{
			uint32 arrayGlyphIndex = glyphIndex - fontFormat.startCharacter;
			K15_GlyphFormat* glyphFormat = K15_GetFontGlyphData(&fontFormat, glyphIndex);
			K15_RenderGlyphDesc* renderGlyphDesc = &renderFontDesc->glyphDescs[arrayGlyphIndex];

			float x = (float)glyphFormat->posX;
			float y = (float)glyphFormat->posY;
			float w = (float)glyphFormat->width / (float)fontFormat.texture.width;
			float h = (float)glyphFormat->height / (float)fontFormat.texture.height;
			
			renderGlyphDesc->character = glyphIndex;
			renderGlyphDesc->x = x;
			renderGlyphDesc->y = y;
			renderGlyphDesc->width = w;
			renderGlyphDesc->height = h;
		}

		//transfer kerning data (literally the same structure)
		memcpy(renderFontDesc->kernDescs, fontFormat.kernFormats, sizeof(K15_RenderKerningDesc) * numKerning);
	}

	p_ResourceCompilerOutput->compiledResourceData = (byte*)renderFontDesc;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = sizeof(K15_RenderFontDesc);
}
/*********************************************************************************/
intern void K15_InternalLoadShaderResource(K15_ResourceContext* p_ResourceContext,
										   K15_ResourceCompilerInput* p_ResourceCompilerInput,
										   K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_RenderProgramDesc programDesc = {};

	char* shaderName = K15_GetFileNameWithoutExtension(p_ResourceCompilerInput->path);
	char* shaderExtensions = K15_GetFileExtension(p_ResourceCompilerInput->path);

	K15_RenderProgramType programType = K15_RENDER_PROGRAM_TYPE_VERTEX;

	if (shaderExtensions)
	{
		if (strcmp(shaderExtensions, ".vert") == 0)
		{
			programType = K15_RENDER_PROGRAM_TYPE_VERTEX;
		}
		else if (strcmp(shaderExtensions, ".frag") == 0)
		{
			programType = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
		}
		else
		{
			K15_LOG_ERROR_MESSAGE("Unknown shader file extension '%s'.", shaderExtensions);
			result = K15_FALSE;
		}
	}
	else
	{
		K15_LOG_ERROR_MESSAGE("shader file '%s' has no extension.", p_ResourceCompilerInput->path);
		result = K15_FALSE;
	}
	
	if (result == K15_TRUE)
	{
		char* shaderCode = K15_CopyString((char*)p_ResourceCompilerInput->fileContent, 
			p_ResourceCompilerInput->fileContentSizeInBytes);

		programDesc.code = shaderCode;
		programDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;
		programDesc.nameHash = K15_GenerateStringHash(shaderName);
		programDesc.type = programType;

		K15_RenderResourceHandle* shaderRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
		K15_RenderCommandCreateProgram(commandQueue, shaderRenderHandle, &programDesc);

		p_ResourceCompilerOutput->compiledResourceData = (byte*)shaderRenderHandle;
		p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = sizeof(K15_RenderResourceHandle);
	}

	free(shaderName);
	free(shaderExtensions);
}
/*********************************************************************************/
intern void K15_InternalLoadMaterialResource(K15_ResourceContext* p_ResourceContext, 
											 K15_ResourceCompilerInput* p_ResourceCompilerInput,
											 K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderContext* renderContext = p_ResourceContext->renderContext;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	byte* fileContent = p_ResourceCompilerInput->fileContent;
	uint32 fileSize = p_ResourceCompilerInput->fileContentSizeInBytes;

	K15_MaterialFormat materialFormat = {};
	K15_LoadMaterialFormatFromMemory(&materialFormat, fileContent, fileSize);

	byte* compiledResourceData = (byte*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderMaterialDesc));
	uint32 compiledResourceDataSizeInBytes = sizeof(K15_RenderMaterialDesc);

	K15_RenderMaterialDesc* renderMaterialDesc = (K15_RenderMaterialDesc*)compiledResourceData;
	renderMaterialDesc->numMaterialPasses = materialFormat.numMaterialPasses;
	renderMaterialDesc->materialPasses = (K15_RenderMaterialPassDesc*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderMaterialPassDesc) * materialFormat.numMaterialPasses);

	p_ResourceCompilerOutput->compiledResourceData = compiledResourceData;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = compiledResourceDataSizeInBytes;

	for (uint32 passIndex = 0;
		passIndex < materialFormat.numMaterialPasses;
		++passIndex)
	{
		K15_RenderMaterialPassDesc* renderMaterialPass = &renderMaterialDesc->materialPasses[passIndex];
		K15_RenderMaterialDataDescStretchBuffer* materialDataDescBuffer = &renderMaterialPass->materialData;

		K15_MaterialPassFormat* materialPassFormat = &materialFormat.materialPasses[passIndex];

		K15_MaterialPassDataFormat* materialPassData = &materialPassFormat->materialPassData;
		K15_MaterialPassTemplateFormat* materialPassTemplate = &materialPassFormat->materialPassTemplate;

		K15_CreateRenderMaterialDataDescStretchBuffer(&renderMaterialPass->materialData, materialPassData->numValues);

		//create shader from template
// 		K15_Resource* vertexShaderResource = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, materialPassTemplate->vertexShaderPath, 0);
// 		K15_Resource* fragmentShaderResource = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, materialPassTemplate->fragmentShaderPath, 0);
		K15_ResourceHandle vsResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, materialPassTemplate->vertexShaderPath, 0);
		K15_ResourceHandle fsResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, materialPassTemplate->fragmentShaderPath, 0);

		renderMaterialPass->vertexShaderHandle = K15_GetResourceRenderHandle(p_ResourceContext, vsResourceHandle);
		renderMaterialPass->fragmentShaderHandle = K15_GetResourceRenderHandle(p_ResourceContext, fsResourceHandle);

		for (uint32 dataIndex = 0;
			dataIndex < materialPassData->numValues;
			++dataIndex)
		{
			K15_RenderMaterialDataDesc materialdataDesc = {};
			K15_MaterialPassDataValue* materialDataValue = &materialPassData->values[dataIndex];

			materialdataDesc.semanticID = K15_SEMANTIC_DATA_ID;
			materialdataDesc.nameHash = K15_GenerateStringHash(materialDataValue->name);
			//materialdataDesc.name = K15_CopyString(materialDataValue->name);

			if (materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_TEXTURE ||
				materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_SAMPLER)
			{
				char* path = materialDataValue->asString;
				uint32 identifier = 0;

				if (materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_TEXTURE)
				{
					identifier = K15_TEXTURE_RESOURCE_IDENTIFIER;
					materialdataDesc.typeID = K15_TYPE_TEXTURE_2D_ID;
				}
				else if (materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_SAMPLER)
				{
					identifier = K15_SAMPLER_RESOURCE_IDENTIFIER;
					materialdataDesc.typeID = K15_TYPE_SAMPLER_2D_ID;
				}

				K15_ResourceHandle externResourceHandle = K15_LoadResource(p_ResourceContext, identifier, path, 0);
				K15_ResourceData* externalResourceData = K15_GetResourceData(p_ResourceContext, externResourceHandle);

				materialdataDesc.data.renderResourceHandle = (K15_RenderResourceHandle*)externalResourceData->compiledResourceData;
			}
			else if (materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_INT)
			{
				materialdataDesc.typeID = K15_TYPE_INT_ID;
				memcpy(&materialdataDesc.data.intValue, &materialDataValue->asInt, sizeof(int));
			}
			else if (materialDataValue->dataType == K15_MATERIAL_DATA_TYPE_FLOAT)
			{
				materialdataDesc.typeID = K15_TYPE_FLOAT_ID;
				memcpy(&materialdataDesc.data.floatValue, &materialDataValue->asFloat, sizeof(float));
			}

			K15_PushRenderMaterialDataDescStretchBufferElement(materialDataDescBuffer, materialdataDesc);
		}
	}
}
/*********************************************************************************/
intern void K15_InternalResourceFileChangedCallback(const char* p_ResourcePath, void* p_UserData)
{

}
/*********************************************************************************/



/*********************************************************************************/
K15_ResourceLoader* K15_AddResourceLoader(K15_ResourceContext* p_ResourceContext, const char* p_Name, uint32 p_Identifier, K15_LoadResourceFromLoaderFnc p_ResourceLoaderFnc)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");

	uint32 loaderIndex = p_ResourceContext->numResourceLoader;

	K15_ASSERT_TEXT(loaderIndex < K15_MAX_RESOURCE_LOADER, "Too many resource loader (limit is %d)", K15_MAX_RESOURCE_LOADER);

	K15_ResourceLoader* resourceLoader = &p_ResourceContext->resourceLoader[loaderIndex];
	
	resourceLoader->name = p_Name;
	resourceLoader->identifier = p_Identifier;
	resourceLoader->resourceLoader = p_ResourceLoaderFnc;

	++p_ResourceContext->numResourceLoader;

	return resourceLoader;
}
/*********************************************************************************/
K15_ResourceData* K15_GetResourceData(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");

	K15_ResourceData* resourceData = 0;

	if (p_ResourceHandle != K15_INVALID_RESOURCE_HANDLE)
	{
		K15_Resource* resource = p_ResourceContext->resourceCache[p_ResourceHandle];
		resourceData = &resource->resourceData;
	}

	return resourceData;
}
/*********************************************************************************/
K15_RenderResourceHandle* K15_GetResourceRenderHandle(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle)
{
	K15_ResourceData* resourceData = K15_GetResourceData(p_ResourceContext, p_ResourceHandle);
	K15_RenderResourceHandle* renderResourceHandle = 0;

	if (resourceData)
	{
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == sizeof(K15_RenderResourceHandle));
		renderResourceHandle = (K15_RenderResourceHandle*)resourceData->compiledResourceData;
	}

	return renderResourceHandle;
}
/*********************************************************************************/
K15_RenderMaterialDesc* K15_GetResourceRenderMaterialDesc(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle)
{
	K15_ResourceData* resourceData = K15_GetResourceData(p_ResourceContext, p_ResourceHandle);
	K15_RenderMaterialDesc* renderMaterialDesc = 0;

	if (resourceData)
	{
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == sizeof(K15_RenderMaterialDesc));
		renderMaterialDesc = (K15_RenderMaterialDesc*)resourceData->compiledResourceData;
	}

	return renderMaterialDesc;
}
/*********************************************************************************/
K15_RenderFontDesc* K15_GetResourceFontDesc(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle)
{
	K15_ResourceData* resourceData = K15_GetResourceData(p_ResourceContext, p_ResourceHandle);
	K15_RenderFontDesc* renderFontDesc = 0;

	if (resourceData)
	{
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == sizeof(K15_RenderFontDesc));
		renderFontDesc = (K15_RenderFontDesc*)resourceData->compiledResourceData;
	}

	return renderFontDesc;
}
/*********************************************************************************/
K15_ResourceContext* K15_CreateResourceContext(K15_RenderContext* p_RenderContext, const char* p_ArchivePath)
{
	return K15_CreateResourceContextWithCustomAllocator(p_RenderContext, p_ArchivePath, K15_CreateDefaultMemoryAllocator());
}
/*********************************************************************************/
K15_ResourceContext* K15_CreateResourceContextWithCustomAllocator(K15_RenderContext* p_RenderContext, const char* p_ArchivePath, K15_CustomMemoryAllocator p_CustomMemoryAllocator)
{
	K15_ASSERT_TEXT(p_ArchivePath, "Resource collection path is NULL.");

	K15_ResourceContext* resourceContext = 0;

	//Check if the resource path is existent
	if (K15_InternalIsValidArchive(p_ArchivePath))
	{
		resourceContext = (K15_ResourceContext*)K15_AllocateFromMemoryAllocator(&p_CustomMemoryAllocator, sizeof(K15_ResourceContext));
		K15_InternalInitializeResourceContext(resourceContext, p_RenderContext, p_CustomMemoryAllocator, p_ArchivePath);
	}
	else
	{
		K15_LOG_ERROR_MESSAGE("Could not find resource path '%s'.", p_ArchivePath);
	}

	K15_AddResourceLoader(resourceContext, "Material Loader", K15_MATERIAL_RESOURCE_IDENTIFIER, K15_InternalLoadMaterialResource);
	K15_AddResourceLoader(resourceContext, "Texture Loader", K15_TEXTURE_RESOURCE_IDENTIFIER, K15_InternalLoadTextureResource);
	K15_AddResourceLoader(resourceContext, "Sampler Loader", K15_SAMPLER_RESOURCE_IDENTIFIER, K15_InternalLoadSamplerResource);
	K15_AddResourceLoader(resourceContext, "Shader Loader", K15_SHADER_RESOURCE_IDENTIFIER, K15_InternalLoadShaderResource);
	K15_AddResourceLoader(resourceContext, "Font Loader", K15_FONT_RESOURCE_IDENTIFIER, K15_InternalLoadFontResource);

	return resourceContext;
}
/*********************************************************************************/
K15_ResourceHandle K15_LoadResource(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceIdentifier, const char* p_ResourcePath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_ResourceContext* resourceContext = p_ResourceContext;

	K15_Mutex* resourceCacheLock = resourceContext->resourceCacheLock;
	uint32 resourceHash = K15_GenerateStringHash(p_ResourcePath) % K15_MAX_RESOURCES;

	//check if the resource has already been loaded and is inside the cache
	K15_Resource* resource = p_ResourceContext->resourceCache[resourceHash];

	if (resource == 0)
	{
		bool8 foundLoader = K15_FALSE;
		K15_ResourceLoader* loader = 0;

		for (uint32 loaderIndex = 0;
			loaderIndex < p_ResourceContext->numResourceLoader;
			++loaderIndex)
		{
			loader = &p_ResourceContext->resourceLoader[loaderIndex];

			if (loader->identifier == p_ResourceIdentifier)
			{
				foundLoader = K15_TRUE;
				break;
			}
		}

		if (foundLoader)
		{
			K15_LOG_NORMAL_MESSAGE("Resource '%s' could not be found in the cache. Loading resource via loader '%s'", p_ResourcePath, loader->name);

			K15_ResourceArchive* archive = &resourceContext->resourceArchive;
			if (!archive->resourceExists(resourceContext, archive, p_ResourcePath))
			{
				K15_LOG_ERROR_MESSAGE("Could not load resource '%s' (%s).", p_ResourcePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
				return 0;
			}

			K15_ResourceCompilerInput resourceCompilerInput = {};
			K15_ResourceData resourceCompilerOutput = {};

			resourceCompilerInput.path = K15_CopyString(p_ResourcePath);

			resourceCompilerInput.fileContentSizeInBytes = archive->getResourceSize(resourceContext, archive, p_ResourcePath);
			K15_ASSERT_TEXT(resourceCompilerInput.fileContentSizeInBytes, "Size of the resource file '%s' is 0 (Maybe file was not accessible?).", p_ResourcePath);

			resourceCompilerInput.fileContent = archive->getResourceData(resourceContext, archive, resourceCompilerInput.fileContentSizeInBytes, p_ResourcePath);
			K15_ASSERT_TEXT(resourceCompilerInput.fileContent, "Resource data from resource file '%s' is NULL (Maybe file was not accessible?).", p_ResourcePath);

			//get next free resource from the resource cache
			resource = (K15_Resource*)K15_AllocateFromMemoryAllocator(&p_ResourceContext->memoryAllocator, sizeof(K15_Resource));
			loader->resourceLoader(p_ResourceContext, &resourceCompilerInput, &resourceCompilerOutput);
			
			//fill data
			resource->compilerInput = resourceCompilerInput;
			resource->resourceData = resourceCompilerOutput;
			resource->flags = p_Flags;
			resource->refCount = 0;
			
			p_ResourceContext->resourceCache[resourceHash] = resource;

			K15_AddFileWatch(p_ResourcePath, , (void*)resourceContext);

			K15_LOG_SUCCESS_MESSAGE("Successfully loaded resource '%s'.", p_ResourcePath);
		}
		else
		{
			K15_LOG_ERROR_MESSAGE("Could not find a loader for resource '%s' (identifier '%d').", p_ResourcePath, p_ResourceIdentifier);
		}
	}

	if (resource)
	{
		K15_InterlockedIncrement(&resource->refCount);
	}

	return resource ? resourceHash : K15_INVALID_RESOURCE_HANDLE;
}
/*********************************************************************************/
void K15_UnloadResource(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");

	if (p_ResourceHandle == K15_INVALID_RESOURCE_HANDLE)
	{
		return;
	}

	K15_Resource* resource = p_ResourceContext->resourceCache[p_ResourceHandle];
	K15_InterlockedDecrement(&resource->refCount);
	if (resource->refCount == 0)
	{
		K15_InterlockedCompareExchangePointer((PVOID*)p_ResourceContext->resourceCache[p_ResourceHandle], 0, 0);
		K15_CustomMemoryAllocator* allocator = &p_ResourceContext->memoryAllocator;
		K15_FreeFromMemoryAllocator(allocator, resource);
	}
}
/*********************************************************************************/
K15_AsyncOperation* K15_AsyncLoadResource(K15_AsyncContext* p_AsyncContext, K15_ResourceHandle* p_ResourceHandlePtr, 
										  K15_ResourceContext* p_ResourceContext, uint32 p_ResourceIdentifier, const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags)
{
	K15_ASSERT_TEXT(p_AsyncContext, "AsyncContext is NULL.");
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_CustomMemoryAllocator* resourceContextAllocator = &p_ResourceContext->memoryAllocator;

	K15_AsyncResourceLoadParameter* asyncParameter = (K15_AsyncResourceLoadParameter*)K15_AllocateFromMemoryAllocator(resourceContextAllocator, sizeof(K15_AsyncResourceLoadParameter));
	asyncParameter->resourceContext = p_ResourceContext;
	asyncParameter->resourcePath = p_ResourcePath;
	asyncParameter->resourceHandlePtr = p_ResourceHandlePtr;
	asyncParameter->resourceIdentifier = p_ResourceIdentifier;
	asyncParameter->flags = p_ResourceFlags;
	
	K15_AsyncOperation* asyncOperation = K15_CreateAsyncOperation(p_AsyncContext, K15_InternalAsyncResourceLoad, 0, asyncParameter, sizeof(asyncParameter), p_AsyncFlags);
	K15_IssueAsyncOperation(p_AsyncContext, asyncOperation);

	return asyncOperation;
}
/*********************************************************************************/