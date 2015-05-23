#include "win32/K15_DynamicLibraryWin32.h"
#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HelperWin32.h"

#include "K15_DynamicLibrary.h"
#include "K15_FileSystem.h"
#include "K15_DefaultCLibraries.h"

#define K15_WIN32_LIBRARY_EXTENSION ".dll"
#define K15_WIN32_LIBRARY_EXTENSION_LENGTH 4

/*********************************************************************************/
struct K15_Win32DynamicLibrary
{
	HMODULE module;
};
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_Win32LoadDynamicLibrary(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary, const char* p_Name)
{
	uint8 returnValue = K15_SUCCESS;

	size_t libNameLength = strlen(p_Name) + K15_WIN32_LIBRARY_EXTENSION_LENGTH;

	char* libSystemName = (char*)alloca(libNameLength + 1);
	libSystemName[libNameLength] = 0;
	sprintf(libSystemName, "%s%s", p_Name, K15_WIN32_LIBRARY_EXTENSION);

	char* libAbsolutePath = K15_ConvertToAbsolutePath(libSystemName); //free(libAbsolutePath)
	size_t pathLength = strlen(libAbsolutePath) + 1;

	wchar_t* libAbsolutePathW = (wchar_t*)alloca(pathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(libAbsolutePath, pathLength, libAbsolutePathW);

	HMODULE libModule = LoadLibraryW(libAbsolutePathW);

	if (!libModule)
	{
		returnValue = K15_OS_ERROR_SYSTEM;
	}
	else
	{
		K15_Win32DynamicLibrary* win32Library = (K15_Win32DynamicLibrary*)K15_OS_MALLOC(sizeof(K15_Win32DynamicLibrary));
		win32Library->module = libModule;
		p_DynamicLibrary->userData = (void*)win32Library;
		p_DynamicLibrary->path = libAbsolutePath;
	}

	return returnValue;
}
/*********************************************************************************/
void* K15_Win32GetProcAddress(K15_DynamicLibrary* p_DynamicLibrary, const char* p_ProcName)
{
	K15_Win32DynamicLibrary* win32Library = (K15_Win32DynamicLibrary*)p_DynamicLibrary->userData;
	K15_ASSERT_TEXT(win32Library && win32Library->module, "Library '%s' is not loaded.", p_DynamicLibrary->name);

	HMODULE libModule = win32Library->module;

	void* address = GetProcAddress(libModule, p_ProcName);

	K15_ASSERT_TEXT(address, "Could not load address of symbol '%s' from library '%s'.", p_ProcName, p_DynamicLibrary->name);

	return address;
}
/*********************************************************************************/