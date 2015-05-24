#include "K15_DynamicLibrary.h"
#include "K15_DefaultCLibraries.h"
#include "K15_FileSystem.h"
#include "K15_FileWatch.h"
#include "K15_OSContext.h"
#include "K15_Logging.h"
#include "K15_System.h"

#include "generated/K15_DynamicLibraryStretchBuffer.cpp"
#include "generated/K15_DynamicFunctionPointerStretchBuffer.cpp"

/*********************************************************************************/
struct K15_DynamicReloadableLibrary
{
	K15_DynamicLibrary* dynamicLibrary;
	char* libraryPath; //the real path. the path in the dynamic library variable is the temp path.
};

/*********************************************************************************/
intern inline char* K15_InternalCreateLibraryFileName(char* p_LibraryName, char* p_Buffer)
{
	sprintf(p_Buffer, "%s%s", p_LibraryName, K15_LIBRARY_FILE_EXTENSION);

	return p_Buffer;
}
/*********************************************************************************/
intern inline char* K15_InternalCreateDynamicLibraryCopy(char* p_LibraryPath, char* p_LibraryFilePath, char* p_OutputBuffer)
{
	size_t tempLibAbsolutePathLength = strlen(p_LibraryPath) + 5; //+5 = '_temp'
	size_t tempLibAbsoluteFilePathLength = strlen(p_LibraryFilePath) + 5; //+5 = '_temp'

	//wihtout extension
	sprintf(p_OutputBuffer, "%s_temp", p_LibraryPath);

	//with extension
	char* tempLibAbsoluteFilePath = K15_InternalCreateLibraryFileName(p_OutputBuffer, (char*)alloca(tempLibAbsoluteFilePathLength + 1)); //+1 for 0 terminator

	K15_CopyFile(p_LibraryFilePath, tempLibAbsoluteFilePath);

	memcpy(p_OutputBuffer, tempLibAbsoluteFilePath, tempLibAbsoluteFilePathLength + 1);

	return p_OutputBuffer;
}
/*********************************************************************************/
intern void K15_InternalReloadDynamicLibrary(void* p_UserData)
{
	K15_DynamicReloadableLibrary* dynamicReloadableLibrary = (K15_DynamicReloadableLibrary*)p_UserData;
	K15_OSContext* osContext = K15_GetOSLayerContext();

	K15_DynamicLibrary* dynamicLibrary = dynamicReloadableLibrary->dynamicLibrary;
	char* dynamicLibraryFilePath = dynamicReloadableLibrary->libraryPath;
	char* dynamicLibraryPath = dynamicLibrary->path;

	size_t tempLibraryFilePathLength = strlen(dynamicLibraryFilePath) + 5; // +5 = '_temp'

	uint8 result = 0;
	
	result = K15_UnloadDynamicLibrary(dynamicLibrary);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not automatically unload dynamic library '%s' (%s)", dynamicLibraryPath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		return;
	}

	char* tempLibraryFilePath = K15_InternalCreateDynamicLibraryCopy(dynamicLibraryPath, dynamicLibraryFilePath, (char*)alloca(tempLibraryFilePathLength + 1));

	result = osContext->system.loadDynamicLibrary(osContext, dynamicLibrary, tempLibraryFilePath);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not automatically reload dynamic library '%s' (%s)", dynamicLibraryPath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
	}

	//reinitialize pointer
	K15_DynamicFunctionPointerStretchBuffer* functionPointerBuffer = &dynamicLibrary->functionPointerBuffer;
	uint32 numFunctionPointer = functionPointerBuffer->numElements;

	for (uint32 functionPointerIndex = 0;
		functionPointerIndex < numFunctionPointer;
		++functionPointerIndex)
	{
		K15_DynamicFunctionPointer* currentFunctionPointer = &functionPointerBuffer->elements[functionPointerIndex];
		char* procName = currentFunctionPointer->name;
		void* procAddress = osContext->system.getProcAddress(osContext, dynamicLibrary, procName);

		*currentFunctionPointer->externalFunctionPointer = procAddress;
	}
}
/*********************************************************************************/
K15_DynamicLibrary* K15_LoadDynamicLibrary(const char* p_LibraryName, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_LibraryName, "Library Name is NULL.");

	K15_OSContext* context = K15_GetOSLayerContext();
	K15_DynamicLibraryStretchBuffer* dynamicLibraryStretchBuffer = &context->system.dynamicLibraries;
	K15_DynamicLibrary* dynamicLibrary = (K15_DynamicLibrary*)K15_OS_MALLOC(sizeof(K15_DynamicLibrary));
	memset(dynamicLibrary, 0, sizeof(K15_DynamicLibrary));

	dynamicLibrary->flags = p_Flags;

	K15_CreateDynamicFunctionPointerStretchBuffer(&dynamicLibrary->functionPointerBuffer);

	//without extension
	char* libAbsolutePath = K15_ConvertToAbsolutePath(p_LibraryName);
	size_t libAbsolutFilePathLength = strlen(libAbsolutePath) + K15_LIBRARY_FILE_EXTENSION_LENGTH;

	//with extension
	char* libAbsoluteFilePath = K15_InternalCreateLibraryFileName(libAbsolutePath, (char*)alloca(libAbsolutFilePathLength + 1));  //+1 for 0 terminator

	//create temp lib file if reloading is enabled
	if ((p_Flags & K15_RELOAD_LIBRARY) > 0)
	{
		//create copy of the 'real' library path. We want to be watching this file.
		char* libAbsoluteFilePathBuffer = (char*)K15_OS_MALLOC(libAbsolutFilePathLength + 1);
		libAbsoluteFilePathBuffer[libAbsolutFilePathLength] = 0;
		memcpy(libAbsoluteFilePathBuffer, libAbsoluteFilePath, libAbsolutFilePathLength);

		libAbsolutFilePathLength = strlen(libAbsoluteFilePath) + 5; //+5 = '_temp'

		//get the name of the temp library file
		char* libAbsoluteTempFilePath = K15_InternalCreateDynamicLibraryCopy(libAbsolutePath, libAbsoluteFilePath, (char*)alloca(libAbsolutFilePathLength + 1));  //+1 for 0 terminator

		K15_DynamicReloadableLibrary dynamicReloadableLibrary = {};
		dynamicReloadableLibrary.dynamicLibrary = dynamicLibrary;
		dynamicReloadableLibrary.libraryPath = libAbsoluteFilePathBuffer; //Memory leak here for now.

		K15_AddFileWatchAndCopyUserData(libAbsoluteFilePath, K15_InternalReloadDynamicLibrary, &dynamicReloadableLibrary, sizeof(K15_DynamicReloadableLibrary));
		libAbsoluteFilePath = libAbsoluteTempFilePath;
	}

	uint8 result = context->system.loadDynamicLibrary(context, dynamicLibrary, libAbsoluteFilePath);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load dynamic library '%s' (%s).", p_LibraryName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		K15_OS_FREE(dynamicLibrary);
		dynamicLibrary = 0;
	}
	else
	{
		size_t libNameLength = strlen(p_LibraryName);
		char* libNameBuffer = (char*)K15_OS_MALLOC(libNameLength + 1);
		char* libPathBuffer = (char*)K15_OS_MALLOC(libAbsolutFilePathLength + 1);
		libNameBuffer[libNameLength] = 0;
		libPathBuffer[libAbsolutFilePathLength] = 0;

		memcpy(libNameBuffer, p_LibraryName, libNameLength);
		memcpy(libPathBuffer, libAbsoluteFilePath, libAbsolutFilePathLength);

		dynamicLibrary->name = libNameBuffer;
		dynamicLibrary->path = libAbsolutePath;
		dynamicLibrary->systemPath = libPathBuffer;

		K15_PushDynamicLibrary(dynamicLibraryStretchBuffer, dynamicLibrary);
	}

	return dynamicLibrary;
}
/*********************************************************************************/
uint8 K15_UnloadDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary)
{
	K15_ASSERT_TEXT(p_DynamicLibrary, "Dynamic Library is NULL.");

	K15_OSContext* context = K15_GetOSLayerContext();
	K15_DynamicLibraryStretchBuffer* dynamicLibraryStretchBuffer = &context->system.dynamicLibraries;
	uint8 returnValue = K15_OS_ERROR_LIBRARY_NOT_LOADED;

	uint8 removedElement = K15_PopDynamicLibraryElement(dynamicLibraryStretchBuffer, p_DynamicLibrary);

	if (removedElement == K15_TRUE)
	{
		returnValue = context->system.unloadDynamicLibrary(context, p_DynamicLibrary);
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

	K15_OSContext* osContext = K15_GetOSLayerContext();

	void* functionPointer = osContext->system.getProcAddress(osContext, p_DynamicLibrary, p_ProcName);
	K15_ASSERT_TEXT(functionPointer, "Could not load function pointer for symbol '%s' (%s)", p_ProcName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));

	size_t procNameLength = strlen(p_ProcName);
	char* procNameBuffer = (char*)K15_OS_MALLOC(procNameLength + 1);
	procNameBuffer[procNameLength] = 0;
	memcpy(procNameBuffer, p_ProcName, procNameLength);

	K15_DynamicFunctionPointer dynamicFunctionPointer = {};
	dynamicFunctionPointer.functionPointer = functionPointer;
	dynamicFunctionPointer.name = procNameBuffer;
	dynamicFunctionPointer.externalFunctionPointer = &functionPointer;

	K15_DynamicFunctionPointerStretchBuffer* functionPointerBuffer = &p_DynamicLibrary->functionPointerBuffer;
	K15_PushDynamicFunctionPointer(functionPointerBuffer, dynamicFunctionPointer);

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