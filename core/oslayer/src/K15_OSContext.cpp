#include "K15_OSContext.h"
#include "K15_FileSystem.h"

#include <K15_Logging.h>

#include <malloc.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

intern K15_OSContext g_InternalOSContext = {};
intern uint8 g_InternalOSContextInitialized = K15_FALSE;

/*********************************************************************************/
K15_OSContext* K15_GetOSLayerContext()
{
	K15_ASSERT_TEXT(g_InternalOSContextInitialized == K15_TRUE, "No os context has been created yet.");
	
	return &g_InternalOSContext;
}
/*********************************************************************************/
void K15_InternalSetOSLayerContext(K15_OSContext p_OSLayerContext)
{
	K15_ASSERT_TEXT(g_InternalOSContextInitialized == K15_FALSE, "OS Context has already been set.");

 	g_InternalOSContext = p_OSLayerContext;
 	g_InternalOSContextInitialized = K15_TRUE;
}
/*********************************************************************************/
intern char* K15_InternalGetWorkingDirectory(char* p_Argument)
{
	uint32 characterIndex = 0;

	while (p_Argument[characterIndex++] != '=');

	return p_Argument + characterIndex;
}
/*********************************************************************************/
intern uint32 K15_InternalGetNumWorkingThreads(char* p_Argument)
{
	uint32 characterIndex = 0;

	while (p_Argument[characterIndex++] != '=');

	char* numWorkingThreadsString = p_Argument + characterIndex;

	characterIndex = 0;

	int numWorkingThreads = atoi(numWorkingThreadsString);

	if (numWorkingThreads <= 0)
	{
		K15_LOG_ERROR_MESSAGE("Amount '%d' is invalid as the number of working threads. Using default (which is '%d').", numWorkingThreads, g_InternalOSContext.threading.numHardwareThreads);
		numWorkingThreads = g_InternalOSContext.threading.numHardwareThreads;
	}

	return numWorkingThreads;
}
/*********************************************************************************/
void K15_ParseCommandLineArguments()
{
	uint32 numArguments = g_InternalOSContext.commandLineArgCount;
	char** arguments = g_InternalOSContext.commandLineArgs;

	for (uint32 argumentIndex = 1;
		argumentIndex < numArguments;
		++argumentIndex)
	{
		char* argument = arguments[argumentIndex];

		//check if working dir path has been set
		if (strstr(argument, "/workingdir=") == argument)
		{
			char* workingDir = K15_InternalGetWorkingDirectory(argument);

			g_InternalOSContext.system.homeDir = K15_ConvertToSystemPath(workingDir);
		}
		else if (strstr(argument, "/numWorkThreads=") == argument)
		{
			int numWorkingThreads = K15_InternalGetNumWorkingThreads(argument);

			g_InternalOSContext.threading.numHardwareThreads = (uint32)numWorkingThreads;
		}
		else
		{
			K15_LOG_WARNING_MESSAGE("Could not interpret command line argument %d ('%s').", argumentIndex, argument);
		}
	}
}
/*********************************************************************************/