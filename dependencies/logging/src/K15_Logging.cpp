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
K15_LogContext g_LogContexts[K15_MAX_LOG_CONTEXT_COUNT] = {};
unsigned int g_LogCount = 0;

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