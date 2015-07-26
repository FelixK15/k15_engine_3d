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

#include "generated/K15_ResourceStretchBuffer.cpp"

/*********************************************************************************/
struct K15_AsyncResourceLoadParameter
{
	K15_ResourceContext* resourceContext;
	K15_Resource** resourcePtr;
	const char* resourcePath;
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
intern uint8 K15_InternalFileSystemResourceExists(K15_ResourceContext* p_ResourceContext, const char* p_Path)
{
	uint32 dirPathLength= (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_FileExists(resourceFilePath);
}
/*********************************************************************************/
intern uint32 K15_InternalFileSystemGetResourceSize(K15_ResourceContext* p_ResourceContext, const char* p_Path)
{
	uint32 dirPathLength= (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	return K15_GetFileSize(resourceFilePath) + 1;
}
/*********************************************************************************/
intern byte* K15_InternalFileSystemLoadResource(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceSize, const char* p_Path)
{
	K15_CustomMemoryAllocator* resourceMemoryAllocator = p_ResourceContext->memoryAllocator;

	uint32 dirPathLength= (uint32)strlen(p_ResourceContext->resourcePath);
	uint32 filePathLength= (uint32)strlen(p_Path);

	uint32 filePathBufferSize = dirPathLength + filePathLength + 1; //+1 = 0 terminator

	char* resourceFilePath = K15_InternalCopyResourcePathIntoBuffer(p_ResourceContext->resourcePath, p_Path, (char*)alloca(filePathBufferSize));

	byte* fileContentBuffer = (byte*)K15_AllocateFromMemoryAllocator(resourceMemoryAllocator, p_ResourceSize);
	K15_CopyWholeFileContentIntoBuffer(resourceFilePath, fileContentBuffer);

	return fileContentBuffer;
}
/*********************************************************************************/
intern uint8 K15_InternalCheckResource(K15_Resource* p_Resource, void* p_UserData)
{
	char* resourcePath = (char*)p_UserData;
	
	return strcmp(p_Resource->resourceFilePath, resourcePath);
}
/*********************************************************************************/
intern void K15_InternalAsyncResourceLoad(void* p_Parameter)
{
	K15_AsyncResourceLoadParameter* loadParamter = (K15_AsyncResourceLoadParameter*)p_Parameter;

	K15_ResourceContext* context = loadParamter->resourceContext;
	K15_Resource** resourcePtr = loadParamter->resourcePtr;
	const char* path = loadParamter->resourcePath;
	uint32 flags = loadParamter->flags;

	K15_CustomMemoryAllocator* customAllocator = context->memoryAllocator;

	(*resourcePtr) = K15_LoadResource(context, path, flags);

	//clean parameter
	K15_FreeFromMemoryAllocator(customAllocator, p_Parameter);
}
/*********************************************************************************/
intern void K15_InternalInitializeResourceContext(K15_ResourceContext* p_ResourceContext, K15_CustomMemoryAllocator* p_MemoryAllocator, const char* p_ResourceCollectionPath, uint32 p_Flags)
{
	//Create resource cache
	K15_ResourceStretchBuffer resourceCache = {};
	K15_CreateResourceStretchBuffer(&resourceCache);

	char* resourcePath = K15_ConvertToAbsolutePath(p_ResourceCollectionPath);

	p_ResourceContext->memoryAllocator = p_MemoryAllocator;
	p_ResourceContext->resourceCacheLock = K15_CreateMutex();
	p_ResourceContext->flags = p_Flags;
	p_ResourceContext->resourcePath = resourcePath;
	p_ResourceContext->resourceCache = resourceCache;

	//check if the resource context path is a directory or file. 
	if (K15_DirectoryExists(resourcePath) == K15_TRUE)
	{
		p_ResourceContext->loadResource = K15_InternalFileSystemLoadResource;
		p_ResourceContext->resourceExists = K15_InternalFileSystemResourceExists;
		p_ResourceContext->resourceSize = K15_InternalFileSystemGetResourceSize;
	}
	else
	{
		//TODO: CHECK FOR PACK FILE EXTENSION
	}
}
/*********************************************************************************/
intern bool8 K15_InternalResourcePathExists(const char* p_ResourcePath)
{
	char* resourcePath = 0;

	if (K15_IsRelativePath(p_ResourcePath))
	{
		resourcePath = K15_ConvertToAbsolutePath(p_ResourcePath);
	}
	else
	{
		resourcePath = K15_ConvertToSystemPath(p_ResourcePath);
	}

	bool8 resourcePathExists = K15_DirectoryExists(resourcePath) || K15_FileExists(resourcePath);

	free(resourcePath);

	return resourcePathExists;
}
/*********************************************************************************/


/*********************************************************************************/
K15_ResourceContext* K15_CreateResourceContext(const char* p_ResourceCollectionPath, uint32 p_Flags)
{
	return K15_CreateResourceContextWithCustomAllocator(p_ResourceCollectionPath, K15_CreateDefaultMemoryAllocator(), p_Flags);
}
/*********************************************************************************/
K15_ResourceContext* K15_CreateResourceContextWithCustomAllocator(const char* p_ResourceCollectionPath, K15_CustomMemoryAllocator* p_CustomMemoryAllocator, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceCollectionPath, "Resource collection path is NULL.");
	K15_ASSERT_TEXT(p_CustomMemoryAllocator, "Custom Memory Allocator is NULL.");

	K15_ResourceContext* resourceContext = 0;

	//Check if the resource path is existent
	if (K15_InternalResourcePathExists(p_ResourceCollectionPath))
	{
		resourceContext = (K15_ResourceContext*)K15_AllocateFromMemoryAllocator(p_CustomMemoryAllocator, sizeof(K15_ResourceContext));
		K15_InternalInitializeResourceContext(resourceContext, p_CustomMemoryAllocator, p_ResourceCollectionPath, p_Flags);
	}
	else
	{
		K15_LOG_ERROR_MESSAGE("Could not find resource path '%s'.", p_ResourceCollectionPath);
	}

	
	

	return resourceContext;
}
/*********************************************************************************/
K15_Resource* K15_LoadResource(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_ResourceContext* resourceContext = p_ResourceContext;
	K15_ResourceStretchBuffer* resourceCache = &resourceContext->resourceCache;

	K15_Mutex* resourceCacheLock = resourceContext->resourceCacheLock;

	//check if the resource has already been loaded and is inside the cache
	K15_LockMutex(resourceCacheLock);
	K15_Resource* resource = K15_GetResourceStretchBufferElementConditional(resourceCache, K15_InternalCheckResource, (void*)p_ResourcePath);
	K15_UnlockMutex(resourceCacheLock);

	if (resource == 0)
	{
		K15_LOG_NORMAL_MESSAGE("Resource '%s' could not be found in the cache. Loading resource...", p_ResourcePath);

		if (!resourceContext->resourceExists(resourceContext, p_ResourcePath))
		{
			//K15_LOG_ERROR_MESSAGE("Could not load resource '%s' (%s).", p_ResourcePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
			K15_ASSERT_TEXT(false, "Could not load resource '%s' (%s).", p_ResourcePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
			return 0;
		}

		uint32 resourceSize = resourceContext->resourceSize(resourceContext, p_ResourcePath);
		
		K15_ASSERT_TEXT(resourceSize, "Size of the resource file '%s' is 0 (Maybe file was not accessible?).", p_ResourcePath);

		byte* resourceBuffer = resourceContext->loadResource(resourceContext, resourceSize, p_ResourcePath);

		K15_ASSERT_TEXT(resourceBuffer, "Resource data from resource file '%s' is NULL (Maybe file was not accessible?).", p_ResourcePath);

		//get next free resource from the resource cache
		resource = K15_GetResourceStretchBufferElementUnsafe(resourceCache, resourceCache->numElements);

		//fill data
		resource->resourceFilePath = K15_CopyString(p_ResourcePath);
		resource->data = resourceBuffer;
		resource->dataSizeInBytes = resourceSize;
		resource->flags = p_Flags;

		//put it back in the cache
		K15_PushResourceStretchBufferElement(resourceCache, *resource);

		K15_LOG_SUCCESS_MESSAGE("Successfully loaded resource '%s'.", p_ResourcePath);
	}

	return resource;
}
/*********************************************************************************/
K15_AsyncOperation* K15_AsyncLoadResource(K15_AsyncContext* p_AsyncContext, K15_Resource** p_ResourcePtr, K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags)
{
	K15_ASSERT_TEXT(p_AsyncContext, "AsyncContext is NULL.");
	K15_ASSERT_TEXT(p_ResourceContext, "ResourceContext is NULL.");
	K15_ASSERT_TEXT(p_ResourcePath, "resource path is NULL.");

	K15_CustomMemoryAllocator* resourceContextMalloc = p_ResourceContext->memoryAllocator;

	K15_AsyncResourceLoadParameter* asyncParameter = (K15_AsyncResourceLoadParameter*)K15_AllocateFromMemoryAllocator(resourceContextMalloc, sizeof(K15_AsyncResourceLoadParameter));
	asyncParameter->resourceContext = p_ResourceContext;
	asyncParameter->resourcePath = p_ResourcePath;
	asyncParameter->resourcePtr = p_ResourcePtr;
	asyncParameter->flags = p_ResourceFlags;
	
	K15_AsyncOperation* asyncOperation = K15_CreateAsyncOperation(p_AsyncContext, K15_InternalAsyncResourceLoad, 0, asyncParameter, sizeof(asyncParameter), p_AsyncFlags);
	K15_IssueAsyncOperation(p_AsyncContext, asyncOperation);

	return asyncOperation;
}
/*********************************************************************************/