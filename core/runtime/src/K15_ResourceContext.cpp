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

#include "K15_FileWatch.h"

#include "K15_RenderMaterialDesc.h"
#include "K15_RenderFontDesc.h"

#include "K15_MaterialFormat.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"
#include "K15_SamplerFormat.h"

#include "K15_RenderContext.h"
#include "K15_RenderCommands.h"
#include "K15_RenderShaderSemantics.h"

#include "K15_FallbackResources.cpp"
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
struct K15_ResourceFileChangedInfo
{
	uint32 resourceIdentifier;
	const char* resourcePath;
	K15_ResourceHandle resourceHandle;
	K15_ResourceContext* resourceContext;
};
/*********************************************************************************/



/*********************************************************************************/
intern bool8 K15_InternalGetResourceFileData(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, K15_ResourceFileData* resourceFileData)
{
	K15_ResourceArchive* archive = &p_ResourceContext->resourceArchive;
	resourceFileData->path = K15_CopyString(p_ResourcePath);

	resourceFileData->fileContentSizeInBytes = archive->getResourceSize(p_ResourceContext, archive, p_ResourcePath);
	K15_ASSERT_TEXT(resourceFileData->fileContentSizeInBytes, "Size of the resource file '%s' is 0 (Maybe file was not accessible?).", p_ResourcePath);

	if (resourceFileData->fileContentSizeInBytes > 0)
	{
		resourceFileData->fileContent = archive->getResourceData(p_ResourceContext, archive, resourceFileData->fileContentSizeInBytes, p_ResourcePath);
		K15_ASSERT_TEXT(resourceFileData->fileContent, "Resource data from resource file '%s' is NULL (Maybe file was not accessible?).", p_ResourcePath);
	}

	return resourceFileData->fileContent != 0;
}
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

	uint32 fileSize = 0;

	if (K15_FileExists(resourceFilePath))
	{
		while(fileSize == 0)
		{
			fileSize = K15_GetFileSize(resourceFilePath);
		}
	}

	return fileSize;
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
											K15_ResourceFileData* p_ResourceFileData,
											K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_TextureFormat textureFormat = {};

	byte* resourceFileContent = p_ResourceFileData->fileContent;
	uint32 resourceFileSize = p_ResourceFileData->fileContentSizeInBytes;
	result8 textureLoadResult = K15_LoadTextureFormatFromMemory(&textureFormat, resourceFileContent, resourceFileSize);

	if (textureLoadResult != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load texture '%s'.", p_ResourceFileData->path);
		//load default texture
	}

	K15_RenderResourceHandle* textureRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateTextureFromTextureFormat(commandQueue, textureRenderHandle, &textureFormat);

	p_ResourceCompilerOutput->compiledResourceData = (byte*)textureRenderHandle;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalLoadBackupTextureResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_TextureFormat textureFormat = {};
	K15_LoadTextureFormatFromMemory(&textureFormat, K15_BackupTexture, K15_BackupTexture_size);

	K15_RenderResourceHandle* textureRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateTextureFromTextureFormat(commandQueue, textureRenderHandle, &textureFormat);

	p_Resource->resourceData.compiledResourceData = (byte*)textureRenderHandle;
	p_Resource->resourceData.compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalUnloadTextureResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderResourceHandle textureHandle = *(K15_RenderResourceHandle*)p_Resource->resourceData.compiledResourceData;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;

	K15_RenderCommandDeleteTexture(commandQueue, textureHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadSamplerResource(K15_ResourceContext* p_ResourceContext, 
											K15_ResourceFileData* p_ResourceFileData,
											K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_SamplerFormat samplerFormat = {};

	byte* resourceFileContent = p_ResourceFileData->fileContent;
	uint32 resourceFileSize = p_ResourceFileData->fileContentSizeInBytes;
	result8 samplerLoadResult = K15_LoadSamplerFormatFromMemory(&samplerFormat, resourceFileContent, resourceFileSize);

	if (samplerLoadResult != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load sampler '%s'.", p_ResourceFileData->path);
		//load default sampler
	}

	K15_RenderResourceHandle* samplerResourceHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateSamplerFromSamplerFormat(commandQueue, samplerResourceHandle, &samplerFormat);

	p_ResourceCompilerOutput->compiledResourceData = (byte*)samplerResourceHandle;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalLoadBackupSamplerResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_SamplerFormat samplerFormat = {};
	K15_LoadSamplerFormatFromMemory(&samplerFormat, K15_BackupSampler, K15_BackupSampler_size);

	K15_RenderResourceHandle* samplerRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderResourceHandle));
	K15_RenderCommandCreateSamplerFromSamplerFormat(commandQueue, samplerRenderHandle, &samplerFormat);

	p_Resource->resourceData.compiledResourceData = (byte*)samplerRenderHandle;
	p_Resource->resourceData.compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalUnloadSamplerResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderResourceHandle samplerHandle = *(K15_RenderResourceHandle*)p_Resource->resourceData.compiledResourceData;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;

	K15_RenderCommandDeleteSampler(commandQueue, samplerHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadFontResource(K15_ResourceContext* p_ResourceContext, 
										 K15_ResourceFileData* p_ResourceFileData,
										 K15_ResourceData* p_ResourceCompilerOutput)
{
	result8 result = K15_SUCCESS;

	K15_RenderContext* renderContext = p_ResourceContext->renderContext;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_FontFormat fontFormat = {};

	byte* resourceFileContent = p_ResourceFileData->fileContent;
	uint32 resourceFileSize = p_ResourceFileData->fileContentSizeInBytes;
	result = K15_LoadFontFormatFromMemory(&fontFormat, resourceFileContent, resourceFileSize);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load font '%s'.", p_ResourceFileData->path);
		return;
	}

	K15_RenderFontDesc* renderFontDesc = K15_CreateRenderFontDescFromFontFormat(&fontFormat, commandQueue, resourceAllocator);

	p_ResourceCompilerOutput->compiledResourceData = (byte*)renderFontDesc;
	p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalLoadBackupFontResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_FontFormat fontFormat = {};
	K15_LoadFontFormatFromMemory(&fontFormat, K15_BackupFont, K15_BackupFont_size);

	K15_RenderFontDesc* renderFontDesc = K15_CreateRenderFontDescFromFontFormat(&fontFormat, commandQueue, resourceAllocator);

	p_Resource->resourceData.compiledResourceData = (byte*)renderFontDesc;
	p_Resource->resourceData.compiledResourceDataSizeInBytes = K15_PTR_SIZE;
}
/*********************************************************************************/
intern void K15_InternalUnloadFontResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderFontDesc* renderFontDesc = (K15_RenderFontDesc*)p_Resource->resourceData.compiledResourceData;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* allocator = &p_ResourceContext->memoryAllocator;

	K15_RenderResourceHandle renderFontTextureHandle = *renderFontDesc->textureHandle;
	K15_RenderCommandDeleteTexture(commandQueue, renderFontTextureHandle);

	K15_FreeFromMemoryAllocator(allocator, renderFontDesc->glyphDescs);
	K15_FreeFromMemoryAllocator(allocator, renderFontDesc->kernDescs);
	K15_FreeFromMemoryAllocator(allocator, renderFontDesc->textureHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadShaderResource(K15_ResourceContext* p_ResourceContext,
										   K15_ResourceFileData* p_ResourceFileData,
										   K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;

	K15_RenderProgramDesc programDesc = {};

	char* shaderName = K15_GetFileNameWithoutExtension(p_ResourceFileData->path);
	char* shaderExtensions = K15_GetFileExtension(p_ResourceFileData->path);

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
		K15_LOG_ERROR_MESSAGE("shader file '%s' has no extension.", p_ResourceFileData->path);
		result = K15_FALSE;
	}
	
	if (result == K15_TRUE)
	{
		char* shaderCode = K15_CopyString((char*)p_ResourceFileData->fileContent, 
			p_ResourceFileData->fileContentSizeInBytes);

		programDesc.code = shaderCode;
		programDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;
		programDesc.nameHash = K15_GenerateStringHash(shaderName);
		programDesc.type = programType;

		K15_RenderResourceHandle* shaderRenderHandle = 0;
		
		if(!p_ResourceCompilerOutput->compiledResourceData)
		{
			shaderRenderHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(resourceAllocator, K15_PTR_SIZE);	
		}
		else
		{
			shaderRenderHandle = (K15_RenderResourceHandle*)p_ResourceCompilerOutput->compiledResourceData;
		}

		K15_RenderCommandCreateProgram(commandQueue, shaderRenderHandle, &programDesc);

		p_ResourceCompilerOutput->compiledResourceData = (byte*)shaderRenderHandle;
		p_ResourceCompilerOutput->compiledResourceDataSizeInBytes = K15_PTR_SIZE;
	}

	free(shaderName);
	free(shaderExtensions);
}
/*********************************************************************************/
intern void K15_InternalUnloadShaderResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderResourceHandle programHandle = *(K15_RenderResourceHandle*)p_Resource->resourceData.compiledResourceData;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;

	K15_RenderCommandDeleteProgram(commandQueue, programHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadMaterialResource(K15_ResourceContext* p_ResourceContext, 
											 K15_ResourceFileData* p_ResourceFileData,
											 K15_ResourceData* p_ResourceCompilerOutput)
{
	bool8 result = K15_TRUE;

	K15_RenderContext* renderContext = p_ResourceContext->renderContext;
	K15_RenderCommandQueue* commandQueue = p_ResourceContext->commandQueue;
	K15_CustomMemoryAllocator* resourceAllocator = &p_ResourceContext->memoryAllocator;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	byte* fileContent = p_ResourceFileData->fileContent;
	uint32 fileSize = p_ResourceFileData->fileContentSizeInBytes;

	K15_MaterialFormat materialFormat = {};
	K15_LoadMaterialFormatFromMemory(&materialFormat, fileContent, fileSize);

	byte* compiledResourceData = (byte*)K15_AllocateFromMemoryAllocator(resourceAllocator, sizeof(K15_RenderMaterialDesc));
	uint32 compiledResourceDataSizeInBytes = K15_PTR_SIZE;

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
intern void K15_InternalUnloadMaterialResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderMaterialDesc* materialDesc = (K15_RenderMaterialDesc*)p_Resource->resourceData.compiledResourceData;
	K15_CustomMemoryAllocator* allocator = &p_ResourceContext->memoryAllocator;

	//TODO
}
/*********************************************************************************/
intern K15_ResourceLoader* K15_InternalGetResourceLoaderForIdentifier(K15_ResourceContext* p_ResourceContext, uint32 p_Identifier)
{
	K15_ResourceLoader* loader = 0;

	for (uint32 loaderIndex = 0;
		loaderIndex < p_ResourceContext->numResourceLoader;
		++loaderIndex)
	{
		loader = &p_ResourceContext->resourceLoader[loaderIndex];

		if (loader->identifier == p_Identifier)
		{
			break;
		}
		else
		{
			loader = 0;
		}
	}

	return loader;
}
/*********************************************************************************/
intern K15_Resource* K15_InternalLoadResource(K15_ResourceContext* p_ResourceContext, K15_ResourceLoader* p_Loader, const char* p_ResourcePath, uint32 p_ResourceIdentifier, K15_ResourceHandle p_ResourceHandle, uint32 p_Flags)
{
	K15_ResourceArchive* archive = &p_ResourceContext->resourceArchive;
	if (!archive->resourceExists(p_ResourceContext, archive, p_ResourcePath))
	{
		K15_LOG_ERROR_MESSAGE("Could not load resource '%s' (%s).", p_ResourcePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		return 0;
	}

	K15_ResourceFileData resourceFileData = {};
	K15_ResourceData resourceCompilerOutput = {};
	K15_Resource* resource = 0;

	if (K15_InternalGetResourceFileData(p_ResourceContext, p_ResourcePath, &resourceFileData))
	{
		//create and load resource
		resource = (K15_Resource*)K15_AllocateFromMemoryAllocator(&p_ResourceContext->memoryAllocator, sizeof(K15_Resource));
		p_Loader->resourceLoader(p_ResourceContext, &resourceFileData, &resourceCompilerOutput);

		//fill data
		resource->resourceFileData = resourceFileData;
		resource->resourceData = resourceCompilerOutput;
		resource->flags = p_Flags;
		resource->identifier = p_ResourceIdentifier;
		resource->handle = p_ResourceHandle;
		resource->refCount = 0;
	}

	return resource;
}
/*********************************************************************************/
intern void K15_InternalUnloadResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_RenderResourceHandle resourceHandle = p_Resource->handle;
	uint32 resourceIdentifier = p_Resource->identifier;

	K15_ResourceLoader* loader = K15_InternalGetResourceLoaderForIdentifier(p_ResourceContext, resourceIdentifier);
	loader->resourceUnloader(p_ResourceContext, p_Resource);
}
/*********************************************************************************/
intern void K15_InternalDeleteResource(K15_ResourceContext* p_ResourceContext, K15_Resource* p_Resource)
{
	K15_CustomMemoryAllocator* allocator = &p_ResourceContext->memoryAllocator;

	K15_FreeFromMemoryAllocator(allocator, p_Resource->resourceFileData.fileContent);
	K15_FreeFromMemoryAllocator(allocator, p_Resource->resourceData.compiledResourceData);
	K15_FreeFromMemoryAllocator(allocator, p_Resource);
}
/*********************************************************************************/
intern void K15_InternalReloadResource(const char* p_ResourcePath, void* p_UserData)
{
	K15_ResourceFileChangedInfo* resourceChangedInfo = (K15_ResourceFileChangedInfo*)p_UserData;
	K15_SleepThreadForMilliseconds(500);
	//TODO: check if file write has been finished

	const char* path = resourceChangedInfo->resourcePath;
	K15_ResourceContext* resourceContext = resourceChangedInfo->resourceContext;
	K15_ResourceHandle resourceHandle = resourceChangedInfo->resourceHandle;
	uint32 resourceIdentifier = resourceChangedInfo->resourceIdentifier;

	K15_CustomMemoryAllocator* memoryAlloctor = &resourceContext->memoryAllocator;
	K15_ResourceLoader* loader = K15_InternalGetResourceLoaderForIdentifier(resourceContext, resourceIdentifier);

	K15_Resource* resource = resourceContext->resourceCache[resourceHandle];
	K15_ResourceFileData resourceFileData = {};

	if (K15_InternalGetResourceFileData(resourceContext, path, &resourceFileData))
	{
		K15_InternalUnloadResource(resourceContext, resource);
		loader->resourceLoader(resourceContext, &resourceFileData, &resource->resourceData);

		K15_FreeFromMemoryAllocator(memoryAlloctor, resource->resourceFileData.fileContent);
		resource->resourceFileData = resourceFileData;
	}
}
/*********************************************************************************/
intern K15_FileWatchEntry* K15_InternalWatchResourceFile(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle, const char* p_ResourcePath, uint32 p_ResourceIdentifier)
{
	K15_ResourceFileChangedInfo* fileChangedInfo = (K15_ResourceFileChangedInfo*)malloc(sizeof(K15_ResourceFileChangedInfo));
	fileChangedInfo->resourceHandle = p_ResourceHandle;
	fileChangedInfo->resourceContext = p_ResourceContext;
	fileChangedInfo->resourceIdentifier = p_ResourceIdentifier;
	fileChangedInfo->resourcePath = p_ResourcePath;

	uint32 dirPathLength= (uint32)strlen(p_ResourceContext->resourceArchive.resourcePath);
	uint32 filePathLength= (uint32)strlen(p_ResourcePath);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourceArchive.resourcePath, p_ResourcePath, (char*)alloca(filePathBufferSize));

	return K15_AddFileWatch(resourceFilePath, K15_InternalReloadResource, fileChangedInfo, K15_USER_DATA_OWNERSHIP);
}
/*********************************************************************************/
intern K15_Resource* K15_InternalLoadBackupResource(K15_ResourceContext* p_ResourceContext, K15_ResourceLoader* p_ResourceLoader, uint32 p_ReosurceIdentifier, uint32 p_ResourceHash)
{
	K15_Resource* resource = 0;
	
	if (p_ResourceLoader->resourceBackupLoader)
	{
		resource = (K15_Resource*)K15_AllocateFromMemoryAllocator(&p_ResourceContext->memoryAllocator, sizeof(K15_Resource));

		resource->handle = p_ResourceHash;
		resource->identifier = p_ReosurceIdentifier;
		resource->flags = 0;

		p_ResourceLoader->resourceBackupLoader(p_ResourceContext, resource);
		
		K15_InterlockedIncrement(&resource->refCount);
		K15_InterlockedExchangePointer((void**)&p_ResourceContext->resourceCache[p_ResourceHash], resource);
	}
	
	return resource;
}
/*********************************************************************************/



/*********************************************************************************/
K15_ResourceLoader* K15_AddResourceLoader(K15_ResourceContext* p_ResourceContext, const char* p_Name, uint32 p_Identifier, K15_LoadResourceFromLoaderFnc p_ResourceLoaderFnc, K15_UnloadResourceFromLoaderFnc p_ResourceUnloaderFnc, K15_LoadBackupResourceFromLoaderFnc p_ResourceBackupLoaderFnc)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");

	uint32 loaderIndex = p_ResourceContext->numResourceLoader;

	K15_ASSERT_TEXT(loaderIndex < K15_MAX_RESOURCE_LOADER, "Too many resource loader (limit is %d)", K15_MAX_RESOURCE_LOADER);

	K15_ResourceLoader* resourceLoader = &p_ResourceContext->resourceLoader[loaderIndex];
	
	resourceLoader->name = p_Name;
	resourceLoader->identifier = p_Identifier;
	resourceLoader->resourceLoader = p_ResourceLoaderFnc;
	resourceLoader->resourceUnloader = p_ResourceUnloaderFnc;
	resourceLoader->resourceBackupLoader = p_ResourceBackupLoaderFnc;

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
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == K15_PTR_SIZE);
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
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == K15_PTR_SIZE);
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
		K15_ASSERT(resourceData->compiledResourceDataSizeInBytes == K15_PTR_SIZE);
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

	K15_AddResourceLoader(resourceContext, "Material Loader", K15_MATERIAL_RESOURCE_IDENTIFIER, K15_InternalLoadMaterialResource, K15_InternalUnloadMaterialResource, 0);
	K15_AddResourceLoader(resourceContext, "Texture Loader", K15_TEXTURE_RESOURCE_IDENTIFIER, K15_InternalLoadTextureResource, K15_InternalUnloadTextureResource, K15_InternalLoadBackupTextureResource);
	K15_AddResourceLoader(resourceContext, "Sampler Loader", K15_SAMPLER_RESOURCE_IDENTIFIER, K15_InternalLoadSamplerResource, K15_InternalUnloadSamplerResource, K15_InternalLoadBackupSamplerResource);
	K15_AddResourceLoader(resourceContext, "Shader Loader", K15_SHADER_RESOURCE_IDENTIFIER, K15_InternalLoadShaderResource, K15_InternalUnloadShaderResource, 0);
	K15_AddResourceLoader(resourceContext, "Font Loader", K15_FONT_RESOURCE_IDENTIFIER, K15_InternalLoadFontResource, K15_InternalUnloadFontResource, K15_InternalLoadBackupFontResource);

	return resourceContext;
}
/*********************************************************************************/
K15_ResourceHandle K15_LoadResource(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceIdentifier, 
									const char* p_ResourcePath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_ResourceContext* resourceContext = p_ResourceContext;

	char* resourcePath = K15_CopyString(p_ResourcePath);

	if (K15_IsRelativePath(p_ResourcePath))
	{
		if (resourcePath[0] == '/')
		{
			uint32 newResourcePathLength = (uint32)(strlen(resourcePath) - 1);
			memmove(resourcePath, resourcePath + 1, newResourcePathLength);
			resourcePath[newResourcePathLength] = 0;
		}
	}

	K15_Mutex* resourceCacheLock = resourceContext->resourceCacheLock;
	uint32 resourceHash = K15_GenerateStringHash(resourcePath) % K15_MAX_RESOURCES;

	//check if the resource has already been loaded and is inside the cache
	K15_Resource* resource = p_ResourceContext->resourceCache[resourceHash];

	if (resource == 0)
	{
		K15_ResourceLoader* loader = K15_InternalGetResourceLoaderForIdentifier(p_ResourceContext, p_ResourceIdentifier);

		if (loader)
		{
			K15_LOG_NORMAL_MESSAGE("Resource '%s' could not be found in the cache. Loading resource via loader '%s'", resourcePath, loader->name);

			resource = K15_InternalLoadResource(p_ResourceContext, loader, resourcePath, p_ResourceIdentifier, resourceHash, p_Flags);

			if (resource)
			{
				p_ResourceContext->resourceCache[resourceHash] = resource;
				K15_LOG_SUCCESS_MESSAGE("Successfully loaded resource '%s'.", resourcePath);

#ifdef K15_WATCH_RESOURCE_FILES
				K15_InternalWatchResourceFile(p_ResourceContext, resourceHash, resourcePath, p_ResourceIdentifier);
#endif //K15_WATCH_RESOURCE_FILES
			}
			else
			{
				resource = K15_InternalLoadBackupResource(p_ResourceContext, loader, p_ResourceIdentifier, resourceHash);
			}

		}
		else
		{
			K15_LOG_ERROR_MESSAGE("Could not find a loader for resource '%s' (identifier '%d').", resourcePath, p_ResourceIdentifier);
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
		K15_InternalUnloadResource(p_ResourceContext, resource);
	}
}
/*********************************************************************************/
K15_AsyncOperation* K15_AsyncLoadResource(K15_AsyncContext* p_AsyncContext, K15_ResourceHandle* p_ResourceHandlePtr, 
										  K15_ResourceContext* p_ResourceContext, uint32 p_ResourceIdentifier, 
										  const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags)
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