#include "K15_DynamicLibrary.h"
#include "K15_DefaultCLibraries.h"
#include "K15_String.h"
#include "K15_FileSystem.h"
#include "K15_FileWatch.h"
#include "K15_OSContext.h"
#include "K15_Logging.h"
#include "K15_String.h"
#include "K15_System.h"

#include "K15_CustomMemoryAllocator.h"

#include "generated/K15_DynamicLibraryStretchBuffer.cpp"
#include "generated/K15_DynamicFunctionPointerStretchBuffer.cpp"

/*********************************************************************************/
intern uint8 K15_CompareFunctionPointerName(K15_DynamicFunctionPointer* p_FunctionPointer, void* p_UserData)
{
	const char* p_ProcName = (const char*)p_UserData;
	char* p_FunctionProcName = p_FunctionPointer->name;
	
	return strcmp(p_ProcName, p_FunctionProcName);
}
/*********************************************************************************/
intern inline char* K15_InternalCreateLibraryFileName(char* p_LibraryName, char* p_Buffer)
{
	sprintf(p_Buffer, "%s%s", p_LibraryName, K15_LIBRARY_FILE_EXTENSION);

	return p_Buffer;
}
/*********************************************************************************/
intern inline char* K15_InternalCreateDynamicLibraryCopy(char* p_LibraryPath, char* p_LibraryFilePath, char* p_OutputBuffer)
{
	uint32 tempLibAbsolutePathLength= (uint32)strlen(p_LibraryPath) + 5; //+5 = '_temp'
	uint32 tempLibAbsoluteFilePathLength= (uint32)strlen(p_LibraryFilePath) + 5; //+5 = '_temp'

	//wihtout extension
	sprintf(p_OutputBuffer, "%s_temp", p_LibraryPath);

	//with extension
	char* tempLibAbsoluteFilePath = K15_InternalCreateLibraryFileName(p_OutputBuffer, (char*)alloca(tempLibAbsoluteFilePathLength + 1)); //+1 for 0 terminator

	K15_CopyFile(p_LibraryFilePath, tempLibAbsoluteFilePath);

	memcpy(p_OutputBuffer, tempLibAbsoluteFilePath, tempLibAbsoluteFilePathLength + 1);

	return p_OutputBuffer;
}
/*********************************************************************************/
// intern void K15_InternalReloadDynamicLibrary(void* p_UserData)
// {
// 	K15_DynamicReloadableLibrary* dynamicReloadableLibrary = (K15_DynamicReloadableLibrary*)p_UserData;
// 	K15_OSContext* osContext = K15_GetOSLayerContext();
// 
// 	K15_DynamicLibrary* dynamicLibrary = dynamicReloadableLibrary->dynamicLibrary;
// 	char* dynamicLibraryFilePath = dynamicReloadableLibrary->libraryPath;
// 	char* dynamicLibraryPath = dynamicLibrary->path;
// 
// 	size_t tempLibraryFilePathLength = strlen(dynamicLibraryFilePath) + 5; // +5 = '_temp'
// 
// 	uint8 result = 0;
// 	
// 	result = K15_UnloadDynamicLibrary(dynamicLibrary);
// 
// 	if (result != K15_SUCCESS)
// 	{
// 		K15_LOG_ERROR_MESSAGE("Could not automatically unload dynamic library '%s' (%s)", dynamicLibraryPath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
// 		return;
// 	}
// 
// 	char* tempLibraryFilePath = K15_InternalCreateDynamicLibraryCopy(dynamicLibraryPath, dynamicLibraryFilePath, (char*)alloca(tempLibraryFilePathLength + 1));
// 
// 	result = osContext->system.loadDynamicLibrary(osContext, dynamicLibrary, tempLibraryFilePath);
// 
// 	if (result != K15_SUCCESS)
// 	{
// 		K15_LOG_ERROR_MESSAGE("Could not automatically reload dynamic library '%s' (%s)", dynamicLibraryPath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
// 	}
// 
// 	//reinitialize pointer
// 	K15_DynamicFunctionPointerStretchBuffer* functionPointerBuffer = &dynamicLibrary->functionPointerBuffer;
// 	uint32 numFunctionPointer = functionPointerBuffer->numElements;
// 
// 	for (uint32 functionPointerIndex = 0;
// 		functionPointerIndex < numFunctionPointer;
// 		++functionPointerIndex)
// 	{
// 		K15_DynamicFunctionPointer* currentFunctionPointer = &functionPointerBuffer->elements[functionPointerIndex];
// 		char* procName = currentFunctionPointer->name;
// 		void* procAddress = osContext->system.getProcAddress(osContext, dynamicLibrary, procName);
// 
// 		*currentFunctionPointer->externalFunctionPointer = procAddress;
// 
// 		procAddress = procAddress;
// 	}
// }
/*********************************************************************************/
K15_DynamicLibrary* K15_LoadDynamicLibrary(const char* p_LibraryName, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_LibraryName, "Library Name is NULL.");

	K15_OSContext* context = K15_GetOSLayerContext();
	K15_DynamicLibraryStretchBuffer* dynamicLibraryStretchBuffer = &context->system.dynamicLibraries;

	//TODO: Check if the library has already been loaded?

	K15_DynamicLibrary* dynamicLibrary = (K15_DynamicLibrary*)K15_OS_MALLOC(sizeof(K15_DynamicLibrary));
	memset(dynamicLibrary, 0, sizeof(K15_DynamicLibrary));

	dynamicLibrary->state = K15_STATE_UNLOADED;
	dynamicLibrary->flags = p_Flags;

	K15_CreateDynamicFunctionPointerStretchBuffer(&dynamicLibrary->functionPointerBuffer);

	//without extension
	char* libAbsolutePath = K15_ConvertToAbsolutePath(p_LibraryName);
	uint32 libAbsolutPathLength= (uint32)strlen(libAbsolutePath);
	uint32 libAbsolutFilePathLength = libAbsolutPathLength + K15_LIBRARY_FILE_EXTENSION_LENGTH;

	//with extension
	char* libAbsoluteFilePath = K15_InternalCreateLibraryFileName(libAbsolutePath, (char*)K15_OS_MALLOC(libAbsolutFilePathLength + 1));  //+1 for 0 terminator
	char* libOriginalAbsoluteFilePath = 0;
	char* libOriginalAbsolutePath = 0;

	//check if the library exists
	if (K15_FileExists(libAbsoluteFilePath) == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Could not find dynamic library '%s'.", libAbsoluteFilePath);
	
		K15_DeleteDynamicFunctionPointerStretchBuffer(&dynamicLibrary->functionPointerBuffer);
		
		K15_OS_FREE(libAbsoluteFilePath);
		K15_OS_FREE(dynamicLibrary);

		return 0;
	}

	//create temp lib file if reloading is enabled
	if ((p_Flags & K15_RELOADABLE_LIBRARY) > 0)
	{
		//create copy of the 'real' library path. We could need this for file watching.
		char* libAbsoluteFilePathBuffer = K15_CopyString(libAbsoluteFilePath, libAbsolutFilePathLength);
		libAbsolutFilePathLength= (uint32)strlen(libAbsoluteFilePath) + 5; //+5 = '_temp'

		//get the name of the temp library file
		char* libAbsoluteTempFilePath = K15_InternalCreateDynamicLibraryCopy(libAbsolutePath, libAbsoluteFilePath, (char*)K15_OS_MALLOC(libAbsolutFilePathLength + 1));  //+1 for 0 terminator
		char* libAbsoluteTempPath = (char*)K15_OS_MALLOC(libAbsolutPathLength + 6); //+6 = '_temp\0'
		sprintf(libAbsoluteTempPath, "%s_temp", libAbsolutePath);

		libOriginalAbsoluteFilePath = libAbsoluteFilePath;//K15_OS_CopyString(libAbsoluteFilePath, libAbsolutFilePathLength);
		libOriginalAbsolutePath = libAbsolutePath;//K15_OS_CopyString(libAbsolutePath, libAbsolutPathLength);

		//K15_OS_FREE(libAbsolutePath);
		//K15_OS_FREE(libAbsoluteFilePath);

		libAbsoluteFilePath = libAbsoluteTempFilePath;
		libAbsolutePath = libAbsoluteTempPath;
	}

	uint8 result = context->system.loadDynamicLibrary(context, dynamicLibrary, libAbsoluteFilePath);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load dynamic library '%s' (%s).", p_LibraryName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		K15_OS_FREE(libOriginalAbsoluteFilePath);
		K15_OS_FREE(libOriginalAbsolutePath);
		K15_OS_FREE(libAbsoluteFilePath);
		K15_OS_FREE(dynamicLibrary);
		dynamicLibrary = 0;
	}
	else
	{
		char* libNameBuffer = K15_CopyString(p_LibraryName);

		dynamicLibrary->state = K15_STATE_LOADED;
		dynamicLibrary->name = libNameBuffer;
		dynamicLibrary->path = libAbsolutePath;
		dynamicLibrary->originalSystemPath = libOriginalAbsoluteFilePath;
		dynamicLibrary->originalPath = libOriginalAbsolutePath;
		dynamicLibrary->systemPath = libAbsoluteFilePath;

		K15_PushDynamicLibraryStretchBufferElement(dynamicLibraryStretchBuffer, dynamicLibrary);
	}

	return dynamicLibrary;
}
/*********************************************************************************/
uint8 K15_ReloadDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");

	if (p_DynamicLibrary->state != K15_STATE_LOADED)
	{
		K15_LOG_WARNING_MESSAGE("Dynamic Library '%s' is not loaded.", p_DynamicLibrary->name);
		return K15_OS_ERROR_DYNAMIC_LIBRARY_NOT_LOADED;
	}

	if ((p_DynamicLibrary->flags & K15_RELOADABLE_LIBRARY) == 0)
	{
		K15_LOG_ERROR_MESSAGE("Dynamic Library '%s' is not a reloadable library. Call 'K15_LoadDynamicLibrary' with the 'K15_RELOADBLE_LIBRARY' flag to create a reloadable library.", p_DynamicLibrary->name);
		return K15_OS_ERROR_DYNAMIC_LIBRARY_NOT_RELOADABLE;
	}

	K15_OSContext* osContext = K15_GetOSLayerContext();

	uint8 result = K15_UnloadDynamicLibrary(p_DynamicLibrary);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	uint32 libraryFilePathLength= (uint32)strlen(p_DynamicLibrary->path) + K15_LIBRARY_FILE_EXTENSION_LENGTH;
	char* libraryFilePath = (char*)alloca(libraryFilePathLength + 1);
	libraryFilePath[libraryFilePathLength] = 0;
	sprintf(libraryFilePath, "%s%s", p_DynamicLibrary->path, K15_LIBRARY_FILE_EXTENSION);

	char* libraryPath = p_DynamicLibrary->originalPath;

	uint32 tempLibraryFilePathLength= (uint32)strlen(libraryFilePath) + 5; //+5 = '_temp'

	char* tempLibraryFilePath = K15_InternalCreateDynamicLibraryCopy(libraryPath, libraryFilePath, (char*)alloca(tempLibraryFilePathLength + 1));
	
	result = osContext->system.loadDynamicLibrary(osContext, p_DynamicLibrary, tempLibraryFilePath);

	if (result == K15_SUCCESS)
	{
		K15_DynamicLibraryStretchBuffer* dynamicLibraryBuffer = &osContext->system.dynamicLibraries;
		K15_PushDynamicLibraryStretchBufferElement(dynamicLibraryBuffer, p_DynamicLibrary);

		p_DynamicLibrary->state = K15_STATE_LOADED;
	}

	return result;
}
/*********************************************************************************/
uint8 K15_UnloadDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");

	if (p_DynamicLibrary->state == K15_STATE_UNLOADED)
	{
		K15_LOG_WARNING_MESSAGE("Trying to unload already unloaded dynamic library '%s'.", p_DynamicLibrary->name);
		return K15_SUCCESS;
	}

	K15_OSContext* context = K15_GetOSLayerContext();
	K15_DynamicFunctionPointerStretchBuffer* dynamicFunctionPointerBuffer = &p_DynamicLibrary->functionPointerBuffer;
	K15_ClearDynamicFunctionPointerStretchBuffer(dynamicFunctionPointerBuffer);

	K15_DynamicLibraryStretchBuffer* dynamicLibraryStretchBuffer = &context->system.dynamicLibraries;
	uint8 returnValue = K15_OS_ERROR_LIBRARY_NOT_LOADED;

	uint8 removedElement = K15_PopDynamicLibraryStretchBufferElement(dynamicLibraryStretchBuffer, p_DynamicLibrary);

	if (removedElement == K15_TRUE)
	{
		returnValue = context->system.unloadDynamicLibrary(context, p_DynamicLibrary);
	}

	if (returnValue == K15_SUCCESS)
	{
		p_DynamicLibrary->state = K15_STATE_UNLOADED;
	}

	return returnValue;
}
/*********************************************************************************/
uint8 K15_UnloadAndDeleteDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");

	uint8 result = K15_UnloadDynamicLibrary(p_DynamicLibrary);

	if (result == K15_SUCCESS)
	{
		K15_DeleteDynamicLibrary(p_DynamicLibrary);
	}

	return result;
}
/*********************************************************************************/
void* K15_GetProcAddress(K15_DynamicLibrary* p_DynamicLibrary, const char* p_ProcName)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");
	K15_ASSERT_TEXT(p_ProcName, "Proc Name is NULL.");

	if (p_DynamicLibrary->state == K15_STATE_UNLOADED)
	{
		K15_LOG_WARNING_MESSAGE("Trying to get symbol address from unloaded dynamic library '%s'.", p_DynamicLibrary->name);
		return 0;
	}

	K15_OSContext* osContext = K15_GetOSLayerContext();

	//check if function has been loaded before.
	K15_DynamicFunctionPointerStretchBuffer* functionPointerBuffer = &p_DynamicLibrary->functionPointerBuffer;

	K15_DynamicFunctionPointer* cachedFunctionPointer = K15_GetDynamicFunctionPointerStretchBufferElementConditional(functionPointerBuffer, K15_CompareFunctionPointerName, (void*)p_ProcName);

	if (cachedFunctionPointer)
	{
		return cachedFunctionPointer->functionPointer;
	}

	void* functionPointer = osContext->system.getProcAddress(osContext, p_DynamicLibrary, p_ProcName);
	K15_ASSERT_TEXT(functionPointer, "Could not load function pointer for symbol '%s' (%s)", p_ProcName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));

	char* procNameBuffer = K15_CopyString(p_ProcName);
	
	K15_DynamicFunctionPointer dynamicFunctionPointer = {};
	dynamicFunctionPointer.functionPointer = functionPointer;
	dynamicFunctionPointer.name = procNameBuffer;
	dynamicFunctionPointer.externalFunctionPointer = &functionPointer;

	K15_PushDynamicFunctionPointerStretchBufferElement(functionPointerBuffer, dynamicFunctionPointer);

	return (*dynamicFunctionPointer.externalFunctionPointer);
}
/*********************************************************************************/
void K15_DeleteDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");

	K15_DeleteDynamicFunctionPointerStretchBuffer(&p_DynamicLibrary->functionPointerBuffer);
	K15_OS_FREE(p_DynamicLibrary->name);
	K15_OS_FREE(p_DynamicLibrary->path);
	K15_OS_FREE(p_DynamicLibrary->systemPath);
	K15_OS_FREE(p_DynamicLibrary);
}
/*********************************************************************************/