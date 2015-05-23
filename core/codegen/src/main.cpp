#include "K15_FileSystem.h"
#include "K15_Logging.h"
#include "K15_CodeGenerator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_ArgumentParser.h"

/*********************************************************************************/
static void K15_PrintfLog(const char* p_Message, LogPriority p_Priority)
{
	printf("%s\n", p_Message);
}
/*********************************************************************************/
int main(int argc, const char** argv)
{
	int returnCode = 0;

	K15_LogRegisterLogFnc(K15_PrintfLog, K15_LOG_PRIORITY_ALL, K15_LOG_FLAG_ADD_TIME);

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