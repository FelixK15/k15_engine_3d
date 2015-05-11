#ifndef _K15_OSContext_h_
#define _K15_OSContext_h_

#include "K15_Prerequisites.h"

struct K15_Window;

///Window
typedef uint8 (*createWindowFnc)(K15Context*, K15_Window*, uint32, uint8);
typedef uint8 (*setWindowDimensionFnc)(K15Context*, K15_Window*, uint32, uint32);
typedef uint8 (*setWindowFullscreenFnc)(K15Context*, K15_Window*, bool8);
typedef uint8 (*setWindowTitleFnc)(K15Context*, K15_Window*, char*);
typedef uint8 (*closeWindowFnc)(K15Context*, K15_Window*);
typedef double (*getElapsedSecondsFnc)();

///Events
typedef uint8 (*pumpSystemEventsFnc)(K15Context*);
typedef void (*sleepFnc)(double);
typedef char* (*getErrorFnc)();

///Threads
typedef uint8 (*createThreadFnc)(K15Context*, K15_Thread*, K15_ThreadFnc, void*);
typedef uint8 (*joinThreadFnc)(K15Context*, K15_Thread*);
typedef uint8 (*tryJoinThreadFnc)(K15Context*, K15_Thread*, uint32);
typedef uint8 (*interruptThreadFnc)(K15Context*, K15_Thread*);
typedef uint8 (*freeThreadFnc)(K15Context*, K15_Thread*);
typedef K15_Thread* (*getCurrentThreadFnc)();
typedef uint8 (*setThreadNameFnc)(K15_Thread*);

enum OSIdentifier
{
	OS_WINDOWS = 0,
	OS_LINUX,
	OS_ANDROID,
	OS_MAC_OS,
	OS_RASPERRY,

	OS_COUNT
};

struct K15Context
{
	///Window Management
	struct
	{
		createWindowFnc createWindow;
		setWindowDimensionFnc setWindowDimension;
		setWindowFullscreenFnc setWindowFullscreen;
		setWindowTitleFnc setWindowTitle;
		closeWindowFnc closeWindow;
		K15_Window* window;
	} window;

	///Event management
	struct  
	{
		pumpSystemEventsFnc pumpSystemEvents;
	} events;
	
	/// System
	struct  
	{
		char* homeDir;
		sleepFnc sleep;
		getErrorFnc getError;
		getElapsedSecondsFnc getElapsedSeconds;
		OSIdentifier systemIdentifier;
	} system;

	/// Threading
	struct  
	{
		createThreadFnc createThread;
		joinThreadFnc joinThread;
		tryJoinThreadFnc tryJoinThread;
		interruptThreadFnc interruptThread;
		setThreadNameFnc setThreadName;
		getCurrentThreadFnc getCurrentThread;
		freeThreadFnc freeThread;
		K15_Thread** threads;
	} threading;

	double timeStarted;
	uint32 commandLineArgCount;
	char** commandLineArgs;
	void* userData;
};

K15Context* K15_GetOSLayerContext();

void K15_InternalSetOSLayerContext(K15Context p_OSLayerContext);

#endif //_K15_OSContext_h_
