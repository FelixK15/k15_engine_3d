#include "K15_Logging.h"

#ifdef _MSC_VER
	#pragma warning(disable : 4996) //function is deprecated, use safe function
#endif //_MSC_VER

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory>

#define K15_MAX_LOG_CONTEXT_COUNT 8
#define K15_MAX_LOG_BUFFER_LENGTH 2048

/*********************************************************************************/
static K15_LogContext g_LogContexts[K15_MAX_LOG_CONTEXT_COUNT] = {};
static unsigned int g_LogCount = 0;
/*********************************************************************************/

/*********************************************************************************/
static int K15_LogWriteTime(char* p_LogBuffer)
{
	char bufferHour[3];
	char bufferMinute[3];
	char bufferSecond[3];
	const char* priority = 0;

	time_t t(time(0));
	tm tm(*localtime(&t));

	itoa(tm.tm_hour, bufferHour, 10);
	itoa(tm.tm_min, bufferMinute, 10);
	itoa(tm.tm_sec, bufferSecond, 10);

	//hours
	if(tm.tm_hour < 10)
	{
		bufferHour[1] = bufferHour[0];
		bufferHour[0] = '0';
	}
	bufferHour[2] = 0;


	//minutes
	if(tm.tm_min < 10)
	{
		bufferMinute[1] = bufferMinute[0];
		bufferMinute[0] = '0';
	}
	bufferMinute[2] = 0;


	//seconds
	if(tm.tm_sec < 10)
	{
		bufferSecond[1] = bufferSecond[0];
		bufferSecond[0] = '0';
	}
	bufferSecond[2] = 0;

	return sprintf(p_LogBuffer, "%s:%s:%s: ", bufferHour, bufferMinute, bufferSecond);
}
/*********************************************************************************/



/*********************************************************************************/
K15_LogContext* K15_GetLogContexts(unsigned int *p_LogCount)
{
	if (p_LogCount)
	{
		*p_LogCount = g_LogCount;
	}

	return g_LogContexts;
}
/*********************************************************************************/
void K15_SetLogContexts(K15_LogContext* p_LogContexts, unsigned int p_LogCount)
{
	g_LogCount = p_LogCount;
	memcpy(g_LogContexts, p_LogContexts, sizeof(K15_LogContext) * K15_MAX_LOG_CONTEXT_COUNT);
}
/*********************************************************************************/
void K15_LogRegisterLogFnc(K15_LogFnc p_LogFunction, unsigned int p_LogPriorityFilterMask, unsigned int p_LogFlags)
{
	assert(g_LogCount + 1 < K15_MAX_LOG_BUFFER_LENGTH);

	g_LogContexts[g_LogCount].fnc = p_LogFunction;
	g_LogContexts[g_LogCount].filterMask = p_LogPriorityFilterMask;
	g_LogContexts[g_LogCount].flags = p_LogFlags;
	++g_LogCount;
}
/*********************************************************************************/
void K15_LogWrite(const char* p_Message, LogPriority p_LogPriority, ...)
{
	assert(p_Message && p_LogPriority > 0);
	
	for (unsigned int logIndex = 0;
		logIndex < g_LogCount;
		++logIndex)
	{
		unsigned int logFilterMask = g_LogContexts[logIndex].filterMask;
		unsigned int logFlags = g_LogContexts[logIndex].flags;

		if ((logFilterMask & p_LogPriority) > 0)
		{
			char logBuffer[K15_MAX_LOG_BUFFER_LENGTH] = {0};
			int offset = 0;

			if ((logFlags & K15_LOG_FLAG_ADD_TIME) > 0)
			{
				offset = K15_LogWriteTime(logBuffer);
			}

			va_list paramList;

			va_start(paramList, p_LogPriority);
			vsprintf(logBuffer + offset, p_Message, paramList);
			va_end(paramList);

			g_LogContexts[logIndex].fnc(logBuffer, p_LogPriority);
		}
	}
}
/*********************************************************************************/