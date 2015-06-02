#ifndef _K15_OSContext_h_
#define _K15_OSContext_h_

#include "K15_Prerequisites.h"
#include "generated/K15_ThreadStretchBuffer.h"
#include "generated/K15_DynamicLibraryStretchBuffer.h"
#include "generated/K15_DirectoryWatchEntryStretchBuffer.h"

///Window
typedef uint8 (*createWindowFnc)(K15_OSContext*, K15_Window*, uint32, uint8);
typedef uint8 (*setWindowDimensionFnc)(K15_OSContext*, K15_Window*, uint32, uint32);
typedef uint8 (*setWindowFullscreenFnc)(K15_OSContext*, K15_Window*, bool8);
typedef uint8 (*setWindowTitleFnc)(K15_OSContext*, K15_Window*, char*);
typedef uint8 (*closeWindowFnc)(K15_OSContext*, K15_Window*);

///System
typedef double (*getElapsedSecondsFnc)();
typedef uint8 (*loadDynamicLibraryFnc)(K15_OSContext*, K15_DynamicLibrary*, const char*);
typedef uint8 (*unloadDynamicLibraryFnc)(K15_OSContext*, K15_DynamicLibrary*);
typedef void* (*getProcAddressFnc)(K15_OSContext*, K15_DynamicLibrary*, const char*);
typedef uint8 (*registerFileWatchFnc)(K15_OSContext*, K15_FileWatchEntry*, K15_DirectoryWatchEntry*);

///Events
typedef uint8 (*pumpSystemEventsFnc)(K15_OSContext*);

///Threads
typedef uint8 (*createThreadFnc)(K15_OSContext*, K15_Thread*, K15_ThreadFnc, void*, uint32);
typedef uint8 (*startThreadFnc)(K15_OSContext*, K15_Thread*);
typedef uint8 (*joinThreadFnc)(K15_OSContext*, K15_Thread*);
typedef uint8 (*tryJoinThreadFnc)(K15_OSContext*, K15_Thread*, uint32);
typedef uint8 (*interruptThreadFnc)(K15_OSContext*, K15_Thread*);
typedef uint8 (*freeThreadFnc)(K15_OSContext*, K15_Thread*);
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

struct K15_OSContext
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
		K15_DynamicLibraryStretchBuffer dynamicLibraries;
		K15_DirectoryWatchEntryStretchBuffer directoryWatchEntries;
		getElapsedSecondsFnc getElapsedSeconds;
		loadDynamicLibraryFnc loadDynamicLibrary;
		unloadDynamicLibraryFnc unloadDynamicLibrary;
		registerFileWatchFnc registerFileWatch;
		getProcAddressFnc getProcAddress;
		OSIdentifier systemIdentifier;
		char* homeDir;
	} system;

	/// Threading
	struct  
	{
		K15_ThreadStretchBuffer threads;
		createThreadFnc createThread;
		startThreadFnc startThread;
		joinThreadFnc joinThread;
		tryJoinThreadFnc tryJoinThread;
		interruptThreadFnc interruptThread;
		setThreadNameFnc setThreadName;
		getCurrentThreadFnc getCurrentThread;
		freeThreadFnc freeThread;
		uint32 numHardwareThreads;
		//K15_Thread** threads;
	} threading;

	double timeStarted;
	uint32 commandLineArgCount;
	char** commandLineArgs;
	void* userData;
};

K15_OSContext* K15_GetOSLayerContext();
void K15_InternalSetOSLayerContext(K15_OSContext p_OSLayerContext);

void K15_ParseCommandLineArguments();

#endif //_K15_OSContext_h_
