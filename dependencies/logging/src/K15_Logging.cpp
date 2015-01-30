#include "K15_Logging.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define K15_MAX_LOG_CONTEXT_COUNT 8
#define K15_MAX_LOG_BUFFER_LENGTH 1024

/*********************************************************************************/
struct K15_LogContext
{
	K15_LogFnc fnc;
	unsigned int filterMask;
};
/*********************************************************************************/
#ifdef K15_LOG_ADD_STDOUT_LOGGING
void K15_LogStdoutLog(const char* p_Message, LogPriority p_LogPriority)
{
	static const char* NORMAL_CONSOLE_COLOR = "\033[0m";
	static const char* DEBUG_CONSOLE_COLOR = "\033[0;34m";
	static const char* ERROR_CONSOLE_COLOR = "\033[0;31m";
	static const char* WARNING_CONSOLE_COLOR = "\033[0;37m";
	static const char* SUCCESS_CONSOLE_COLOR = "\033[0;32m";
// 
// 	if(p_LogPriority == K15_LOG_PRIORITY_NORMAL)
// 	{
// 		printf(NORMAL_CONSOLE_COLOR);
// 	}
// 	else if(p_LogPriority == K15_LOG_PRIORITY_ERROR)
// 	{
// 		printf(ERROR_CONSOLE_COLOR);
// 	}
// 	else if(p_LogPriority == K15_LOG_PRIORITY_WARNING)
// 	{
// 		printf(WARNING_CONSOLE_COLOR);
// 	}
// 	else if(p_LogPriority == K15_LOG_PRIORITY_SUCCESS)
// 	{
// 		printf(SUCCESS_CONSOLE_COLOR);
// 	}
// 	else if(p_LogPriority == K15_LOG_PRIORITY_DEBUG)
// 	{
// 		printf(DEBUG_CONSOLE_COLOR);
//	}

	printf("%s", p_Message/*, NORMAL_CONSOLE_COLOR*/);
}

unsigned int g_LogCount = 1;
K15_LogContext g_LogContexts[K15_MAX_LOG_CONTEXT_COUNT] = {
	{K15_LogStdoutLog, K15_LOG_PRIORITY_ALL}
};
#else
unsigned int g_LogCount = 0;
#endif

/*********************************************************************************/


/*********************************************************************************/
void K15_LogRegisterLogFnc(K15_LogFnc p_LogFunction, unsigned int p_LogPriorityFilterMask)
{
	assert(g_LogCount + 1 < K15_MAX_LOG_BUFFER_LENGTH);

	g_LogContexts[g_LogCount].fnc = p_LogFunction;
	g_LogContexts[g_LogCount].filterMask = p_LogPriorityFilterMask;

	++g_LogCount;
}
/*********************************************************************************/
void K15_LogWrite(const char* p_Message, LogPriority p_LogPriority, ...)
{
	assert(p_Message && p_LogPriority > 0);
	
	char logBuffer[K15_MAX_LOG_BUFFER_LENGTH] = {0};

	va_list paramList;

	va_start(paramList, p_LogPriority);
	vsprintf(logBuffer, p_Message, paramList);
	va_end(paramList);

	for (unsigned int logIndex = 0;
		 logIndex < g_LogCount;
		 ++logIndex)
	{
		unsigned int logFilterMask = g_LogContexts[logIndex].filterMask;

		if ((logFilterMask & p_LogPriority) > 0)
		{
			g_LogContexts[logIndex].fnc(logBuffer, p_LogPriority);
		}
	}
}
/*********************************************************************************/