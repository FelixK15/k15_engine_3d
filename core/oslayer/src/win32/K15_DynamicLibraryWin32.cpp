#include "win32/K15_DynamicLibraryWin32.h"
#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HelperWin32.h"

#include "K15_DynamicLibrary.h"
#include "K15_FileSystem.h"
#include "K15_DefaultCLibraries.h"
#include "K15_String.h"

#define K15_WIN32_LIBRARY_EXTENSION ".dll"
#define K15_WIN32_LIBRARY_EXTENSION_LENGTH 4

/*********************************************************************************/
struct K15_Win32DynamicLibrary
{
	HMODULE module;
};
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_Win32LoadDynamicLibrary(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary, const char* p_Path)
{
	uint8 returnValue = K15_SUCCESS;

	uint32 pathLength= (uint32)strlen(p_Path) + 1;

	wchar_t* libAbsolutePathW = (wchar_t*)alloca(pathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_Path, pathLength, libAbsolutePathW);

	HMODULE libModule = LoadLibraryW(libAbsolutePathW);

	if (!libModule)
	{
		returnValue = K15_OS_ERROR_SYSTEM;
	}
	else
	{
		K15_Win32DynamicLibrary* win32Library = (K15_Win32DynamicLibrary*)p_DynamicLibrary->userData;
		
		if (!win32Library)
		{
			win32Library = (K15_Win32DynamicLibrary*)K15_OS_MALLOC(sizeof(K15_Win32DynamicLibrary));
			p_DynamicLibrary->userData = (void*)win32Library;
		}

		win32Library->module = libModule;
	}

	return returnValue;
}
/*********************************************************************************/
uint8 K15_Win32UnloadDynamicLibrary(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary)
{
	uint8 returnValue = K15_SUCCESS;

	K15_Win32DynamicLibrary* win32DynamicLibrary = (K15_Win32DynamicLibrary*)p_DynamicLibrary->userData;

	BOOL result = FreeLibrary(win32DynamicLibrary->module);

	if (result != TRUE)
	{
		returnValue = K15_OS_ERROR_SYSTEM;
	}

	return returnValue;
}
/*********************************************************************************/
void* K15_Win32GetProcAddress(K15_OSContext* p_OSContext, K15_DynamicLibrary* p_DynamicLibrary, const char* p_ProcName)
{
	K15_Win32DynamicLibrary* win32Library = (K15_Win32DynamicLibrary*)p_DynamicLibrary->userData;
	K15_ASSERT_TEXT(win32Library && win32Library->module, "Library '%s' is not loaded.", p_DynamicLibrary->name);

	HMODULE libModule = win32Library->module;

	return GetProcAddress(libModule, p_ProcName);;
}
/*********************************************************************************/