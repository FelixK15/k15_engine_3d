#include "K15_ResourceCompilerConfig.h"

#include "K15_OSContext.h"
#include "K15_FileSystem.h"
#include "K15_FileWatch.h"
#include "K15_Logging.h"
#include "K15_String.h"
#include "K15_AsyncOperation.h"

#include "enums/K15_FormatValues.h"
#include "K15_DataAccessHelper.h"
#include "K15_MeshFormat.h"
#include "K15_ConfigFile.h"

#include <stdio.h>

#define K15_RESOURCE_COMPILER_VERSION_MAJOR "1"
#define K15_RESOURCE_COMPILER_VERSION_MINOR "0"

#ifdef K15_OS_WINDOWS
#include "win32/K15_OSContextWin32.h"
#define K15_InitializeOSLayer K15_Win32InitializeLightOSLayer
#else
#error platform missing
#endif 

/*********************************************************************************/
void K15_PrintHelp()
{
	const char* message = "\n\n"
		"K15 Resource Compiler Version %s.%s\n"
		"====================================\n"
		"Usage:\n"
		"k15_resourcecompiler -i [file|dir] -o [dir] -q [high|medium|low]\n"
		"                     -f [mesh|shader|textures|all] -r\n"
		"\n"
		"Parameters:\n"
		"-i   -   Input [file|dir] either a single file or a complete directory.\n"
		"-o   -   Output [dir] directory where to put compiled resources.\n"
		"-u	  -   Replace old compiled file with new compiled file.\n"
		"-r   -   Recursively search directory if input is a directory.\n"
		"-d   -   Start the resource compiler as a demon.\n"
		"\n"
		"Description:\n"
		"The K15 Resource compiler is a command line tool, which compiles specific\n"
		"game resource files (e.g textures or meshes) into binary files which can be\n"
		"used by the 3D game engine \"K15 Engine 3D\".\n\n";

	printf(message, K15_RESOURCE_COMPILER_VERSION_MAJOR, K15_RESOURCE_COMPILER_VERSION_MINOR);
}
/*********************************************************************************/
void K15_PrintDaemonInfo()
{
	const char* message = "\n\n"
		"K15 Resource Compiler Version %s.%s\n"
		"====================================\n"
		"K15 Resource Compiler started in daemon mode. In this mode the compiler\n"
		"application stays alive until you terminate it by hand. The compiler\n"
		"will install a directory watch on the input directory and automatically\n"
		"compile all .k15resourcefiles that will be added/modified in the\n"
		"input directory. (NOTE: for best usage also start with the -u flag.)\n\n";

	printf(message, K15_RESOURCE_COMPILER_VERSION_MAJOR, K15_RESOURCE_COMPILER_VERSION_MINOR);
}
/*********************************************************************************/

#include "K15_ArgumentParser.cpp"
#include "K15_ResourceCompiler.cpp"

/*********************************************************************************/
void K15_OnResourceFileChanged(const char* p_ResourceFilePath, void* p_UserData)
{
	K15_ResourceCompilerContext* compilerContext = (K15_ResourceCompilerContext*)p_UserData;

	//filter for k15resourceinfo files
	if (K15_IsSubstringR(p_ResourceFilePath, ".k15resourceinfo"))
	{
		uint32 numFilesToCompile = 0;
		K15_CompileResources(compilerContext, K15_CreateStringArray(1, p_ResourceFilePath), 1);
	}
}
/*********************************************************************************/
int main(int argc, char** argv)
{
	K15_InitializeOSLayer();
	K15_OSContext* osContext = K15_GetOSLayerContext();

	K15_ArgumentParser argumentParser = {};
	K15_AsyncContext* asyncContext = K15_CreateAsyncContext(osContext);

	//resource compiler arguments
	K15_ParseArguments(&argumentParser, argc - 1, argv + 1);

	if (K15_CheckArguments(&argumentParser) == K15_FALSE)
	{
		return -1;
	}

	uint32 numFilesToCompile = 0;

	K15_ResourceCompilerContext compilerContext = {};

	compilerContext.asyncContext = asyncContext;
	compilerContext.argumentParser = &argumentParser;

	K15_CreateDefaultResourceCompiler(&compilerContext);

	K15_AddDirectoryWatch(argumentParser.inputPath, K15_OnResourceFileChanged, (void*)&compilerContext);

	char** filesToCompile = K15_GetFilesInDirectory(argumentParser.inputPath, &numFilesToCompile, "*.k15resourceinfo");

	K15_CompileResources(&compilerContext, filesToCompile, numFilesToCompile);

	if (argumentParser.daemon)
	{
		while(true)
		{
			K15_SleepThreadForSeconds(10);
		}
	}

	return 0;
}
/*********************************************************************************/