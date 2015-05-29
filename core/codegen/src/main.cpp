#include "K15_FileSystem.h"
#include "K15_Logging.h"
#include "K15_CodeGenerator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_ArgumentParser.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_OSContextWin32.h"	
#endif //K15_OS_WINDOWS

/*********************************************************************************/
int main(int argc, const char** argv)
{
	int returnCode = 0;

#ifdef K15_OS_WINDOWS
	K15_Win32InitializeOSLayer(0);
#endif //K15_OS_WINDOWS

	if (argc < 3)
	{
		K15_LOG_ERROR_MESSAGE("Too few arguments. Add '-h' to show help.");
		returnCode = -1;
	}
	else
	{
		K15_ArgumentParserContext parserContext = {};
		parserContext.workingDir = K15_GetWorkingDirectory();

		for (int argumentIndex = 1; //1st arg is always working dir
			argumentIndex < argc;
			++argumentIndex)
		{
			const char* argument = argv[argumentIndex];

			K15_ArgumentParserState parserState = K15_ParseArgument(&parserContext, argument);
			parserContext.state = parserState;
		}

		if (K15_CheckArguments(&parserContext) == 0)
		{
			K15_GenerateCode(&parserContext);
		}
	}

	return returnCode;
}
/*********************************************************************************/