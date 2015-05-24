#ifndef _K15_OSLayer_Dynamic_Library_h_
#define _K15_OSLayer_Dynamic_Library_h_

#include "K15_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_DynamicLibraryWin32.h"
#elif defined K15_OS_LINUX || defined K15_OS_ANDROID
	#include "posix/K15_DynamicLibraryPosix.h"
#endif

#include "generated/K15_DynamicFunctionPointerStretchBuffer.h"

/*********************************************************************************/
enum K15_DynamicLibraryFlags
{
	K15_RELOAD_LIBRARY = 0x02  //reload library if library file has been touched
};
/*********************************************************************************/
struct K15_DynamicFunctionPointer
{
	void* functionPointer;			//Pointer to the function
	void** externalFunctionPointer;	
	char* name;						//Name of the exported symbol
};
/*********************************************************************************/
struct K15_DynamicLibrary
{
	char* name;			//name of the library (file name without extension)
	char* path;			//path without the system depended library extensions
	char* systemPath;	//path with the system depended library extension
	void* userData;		//platform specific user data

	K15_DynamicFunctionPointerStretchBuffer functionPointerBuffer;	//Buffer of all function pointer the user requested via K15_GetProcAddress

	uint32 flags;		//flags (see K15_DynamicLibraryFlags)
};
/*********************************************************************************/

K15_DynamicLibrary* K15_LoadDynamicLibrary(const char* p_LibraryName, uint32 p_Flags = 0);
uint8 K15_UnloadDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary);
uint8 K15_UnloadAndDeleteDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary);
void* K15_GetProcAddress(K15_DynamicLibrary* p_DynamicLibrary, const char* p_ProcName);
void K15_DeleteDynamicLibrary(K15_DynamicLibrary* p_DynamicLibrary);

#endif //_K15_OSLayer_Dynamic_Library_h_