#include "K15_ResourceContext.h"
#include "K15_FileSystem.h"
#include "K15_String.h"
#include "K15_OSContext.h"
#include "K15_MemoryBuffer.h"
#include "K15_System.h"
#include "K15_Logging.h"
#include "K15_DefaultCLibraries.h"

#include "generated/K15_ResourceFixedBuffer.cpp"

/*********************************************************************************/
intern char* K15_InternalCopyResourcePathIntoBuffer(const char* p_ResourcePackPath, const char* p_ResourcePath, char* p_Buffer)
{
	uint32 resourcePackPathLength = (uint32)strlen(p_ResourcePackPath);
	uint32 resourcePathLength = (uint32)strlen(p_ResourcePath);
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
intern uint8 K15_InternalFileSystemResourceExists(K15_ResourceContext* p_ResourceContext, const char* p_Path)
{
	uint32 dirPathLength = (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength = (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_FileExists(resourceFilePath);
}
/*********************************************************************************/
intern uint32 K15_InternalFileSystemGetResourceSize(K15_ResourceContext* p_ResourceContext, const char* p_Path)
{
	uint32 dirPathLength = (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength = (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_GetFileSize(resourceFilePath) + 1;
}
/*********************************************************************************/
intern byte* K15_InternalFileSystemLoadResource(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceSize, const char* p_Path)
{
	K15_MemoryBuffer* resourceMemoryBuffer = p_ResourceContext->resourceMemoryBuffer;

	uint32 dirPathLength = (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength = (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	byte* fileContentBuffer = K15_GetMemoryFromMemoryBuffer(resourceMemoryBuffer, p_ResourceSize);
	K15_CopyWholeFileContentIntoBuffer(resourceFilePath, fileContentBuffer);

	return fileContentBuffer;
}
/*********************************************************************************/
intern K15_ResourceContext* K15_InternalCreateResourceContext(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceMemoryBufferSizeInBytes, uint32 p_Flags)
{
	uint8 directoryExists = K15_DirectoryExists(p_ResourceContext->resourcePath);
	uint8 fileExists = K15_FileExists(p_ResourceContext->resourcePath);
	K15_ResourceContext* resourceContext = p_ResourceContext;
	K15_MemoryBuffer* resourceMemoryBuffer = p_ResourceContext->resourceMemoryBuffer;

	//this should be a super temporary solution!
	uint32 memorySizeInBytes = (uint32)((float)p_ResourceMemoryBufferSizeInBytes * 0.05f);

	uint32 numCapcity = memorySizeInBytes / sizeof(K15_Resource);
	byte* resourceCacheMemory = K15_GetMemoryFromMemoryBuffer(resourceMemoryBuffer, memorySizeInBytes);

	K15_ResourceFixedBuffer resourceCache = {};
	K15_CreateResourceFixedBufferWithPreallocatedMemory(&resourceCache, resourceCacheMemory, numCapcity);

	resourceContext->resourceCache = resourceCache;

	if (directoryExists == K15_SUCCESS)
	{
		resourceContext->loadResource = K15_InternalFileSystemLoadResource;
		resourceContext->resourceExists = K15_InternalFileSystemResourceExists;
		resourceContext->resourceSize = K15_InternalFileSystemGetResourceSize;
	}
	else if(fileExists == K15_SUCCESS)
	{
		//TODO: CHECK FOR PACK FILE EXTENSION
	}

	return resourceContext;
}
/*********************************************************************************/
intern uint8 K15_InternalCheckResource(K15_Resource* p_Resource, void* p_UserData)
{
	char* resourcePath = (char*)p_UserData;
	
	return strcmp(p_Resource->resourceFilePath, resourcePath);
}
/*********************************************************************************/




/*********************************************************************************/
K15_ResourceContext* K15_CreateAndInitializeResourceContext(const char* p_ResourceCollectionPath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceCollectionPath, "Resource collection path is NULL.");

	K15_ResourceContext* resourceContext = (K15_ResourceContext*)K15_RT_MALLOC(sizeof(K15_ResourceContext));
	K15_MemoryBuffer* memoryBuffer = (K15_MemoryBuffer*)K15_RT_MALLOC(sizeof(K15_MemoryBuffer));
	
	K15_InitializeDefaultMemoryBuffer(memoryBuffer, K15_DEFAULT_RESOURCE_CONTEXT_RESOURCE_BUFFER_SIZE, 0);
	
	K15_InitializeResourceContext(resourceContext, memoryBuffer, p_ResourceCollectionPath, p_Flags);

	return resourceContext;
}
/*********************************************************************************/
uint8 K15_InitializeResourceContext(K15_ResourceContext* p_ResourceContext, K15_MemoryBuffer* p_ResourceMemoryBuffer, const char* p_ResourceCollectionPath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourceMemoryBuffer, "Resource Memory Buffer is NULL.");
	K15_ASSERT_TEXT(p_ResourceCollectionPath, "Resource collection path is NULL.");

	p_ResourceContext->flags = p_Flags;
	p_ResourceContext->resourceMemoryBuffer = p_ResourceMemoryBuffer;
	p_ResourceContext->resourcePath = K15_ConvertToAbsolutePath(p_ResourceCollectionPath);

	K15_InternalCreateResourceContext(p_ResourceContext, p_ResourceMemoryBuffer->sizeInBytes, p_Flags);

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_Resource* K15_LoadResource(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_ResourceContext* resourceContext = p_ResourceContext;
	K15_ResourceFixedBuffer* resourceCache = &resourceContext->resourceCache;

	//check if the resource has already been loaded and is inside the cache
	K15_Resource* resource = K15_GetResourceFixedBufferElementConditional(resourceCache, K15_InternalCheckResource, (void*)p_ResourcePath);

	if (resource == 0)
	{
		K15_LOG_NORMAL_MESSAGE("Resource '%s' could not be found in the cache. Loading resource...", p_ResourcePath);

		if (resourceContext->resourceExists(resourceContext, p_ResourcePath) != K15_SUCCESS)
		{
			K15_LOG_ERROR_MESSAGE("Could not load resource '%s' (%s).", p_ResourcePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
			return 0;
		}

		uint32 resourceSize = resourceContext->resourceSize(resourceContext, p_ResourcePath);
		
		K15_ASSERT_TEXT(resourceSize, "Size of the resource file '%s' is 0 (Maybe file was not accessible?).", p_ResourcePath);

		byte* resourceBuffer = resourceContext->loadResource(resourceContext, resourceSize, p_ResourcePath);

		K15_ASSERT_TEXT(resourceBuffer, "Resource data from resource file '%s' is NULL (Maybe file was not accessible?).", p_ResourcePath);

		//get next free resource from the resource cache
		resource = K15_GetResourceFixedBufferElementUnsafe(resourceCache, resourceCache->numElements);

		resource->resourceFilePath = K15_CopyString(p_ResourcePath);
		resource->data = resourceBuffer;
		resource->dataSizeInBytes = resourceSize;
		resource->flags = p_Flags;

		//put it back in the cache
		K15_PushResourceFixedBufferElement(resourceCache, *resource);

		K15_LOG_SUCCESS_MESSAGE("Successfully loaded resource '%s'.", p_ResourcePath);
	}

	return resource;
}
/*********************************************************************************/
K15_AsyncOperation* K15_AsyncLoadResource(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_Flags)
{
	return 0;
}
/*********************************************************************************/