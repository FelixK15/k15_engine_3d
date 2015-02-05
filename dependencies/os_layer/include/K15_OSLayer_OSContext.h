#ifndef _K15_OSLayer_OSContext_h_
#define _K15_OSLayer_OSContext_h_

#include "K15_OSLayer_Prerequisites.h"

struct K15_Window;

///Window
typedef uint8 (*createWindowFnc)(K15_OSLayerContext*, K15_Window*, uint32, uint8);
typedef uint8 (*setWindowDimensionFnc)(K15_OSLayerContext*, K15_Window*, uint32, uint32);
typedef uint8 (*setWindowFullscreenFnc)(K15_OSLayerContext*, K15_Window*, bool8);
typedef uint8 (*setWindowTitleFnc)(K15_OSLayerContext*, K15_Window*, char*);
typedef uint8 (*closeWindowFnc)(K15_OSLayerContext*, K15_Window*);
typedef float (*getTicksFnc)();

///Events
typedef uint8 (*pumpSystemEventsFnc)(K15_OSLayerContext*);
typedef void (*sleepFnc)(float);
typedef char* (*getErrorFnc)();

enum OSIdentifier
{
	OS_WINDOWS = 0,
	OS_LINUX,
	OS_ANDROID,
	OS_MAC_OS,
	OS_RASPERRY,

	OS_COUNT
};

struct K15_OSLayerContext
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
		getTicksFnc getTicks;
		OSIdentifier systemIdentifier;
	} system;

	uint32 commandLineArgCount;
	char** commandLineArgs;
	void* userData;
};

K15_OSLayerContext* K15_GetOSLayerContext();

void K15_InternalSetOSLayerContext(K15_OSLayerContext p_OSLayerContext);

#endif //_K15_OSLayer_OSContext_h_