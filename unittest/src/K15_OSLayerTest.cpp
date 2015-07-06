#include "K15_FileSystem.h"

/*********************************************************************************/
bool8 K15_FileSystemTest()
{
#ifdef K15_OS_WINDOWS
	const char* absolutePath = "C:/Windows/";
	const char* relativePath = "/Windows/";
#else
	#error missing platform
#endif 

	return K15_IsRelativePath(relativePath) && K15_IsAbsolutePath(absolutePath);
}
/*********************************************************************************/