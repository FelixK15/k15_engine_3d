#ifndef _K15_OSLayer_Thread_h_
#define _K15_OSLayer_Thread_h_

#include "K15_OSLayer_Prerequisites.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_ThreadWin32.h"
#elif defined K15_OS_LINUX || defined K15_OS_ANDROID
	#include "posix/K15_ThreadPosix.h"
#endif 

/*********************************************************************************/
enum K15_ThreadFlags : uint32
{
	K15_THREAD_START_FLAG = 0x01,

	K15_THREAD_DEFAULT_FLAGS = K15_THREAD_START_FLAG
};
/*********************************************************************************/
enum K15_ThreadState : uint8
{
	K15_THREAD_STATE_NOT_STARTED = 0,
	K15_THREAD_STATE_RUNNING = 1,
	K15_THREAD_STATE_FINISHED = 2,
	K15_THREAD_STATE_JOINED = 3,
	K15_THREAD_STATE_INTERRUPTED
};
/*********************************************************************************/
struct K15_ThreadContext
{
	K15_ThreadFnc function;
	void* parameter;

	char* name;
	uint32 nameLength;
	uint32 startFlags;

	K15_ThreadState state;
};
/*********************************************************************************/
struct K15_Thread
{
	K15_ThreadContext* context;

	void* userData;
};
/*********************************************************************************/

K15_Thread* K15_CreateThread(K15_ThreadFnc p_ThreadFunction, void* p_ThreadParameter, uint32 p_ThreadFlags = K15_THREAD_DEFAULT_FLAGS);
K15_Thread* K15_GetCurrentThread();
uint8 K15_FreeThread(K15_Thread* p_Thread);

uint8 K15_StartThread(K15_Thread* p_Thread);
uint8 K15_JoinThread(K15_Thread* p_Thread);
uint8 K15_TryJoinThread(K15_Thread* p_Thread, uint32 p_MilliSeconds);
uint8 K15_InterruptThread(K15_Thread* p_Thread);

uint8 K15_SetThreadName(K15_Thread* p_Thread, const char* p_ThreadName);

#endif //_K15_OSLayer_Thread_h_