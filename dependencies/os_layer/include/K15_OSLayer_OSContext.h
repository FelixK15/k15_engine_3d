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

///Events
typedef uint8 (*pumpSystemEventsFnc)(K15_OSLayerContext*);
typedef uint8 (*sleepFnc)(float);
typedef uint8 (*getErrorFnc)(char*);

enum OSIdentifier
{
	OS_WINDOWS = 0,
	OS_LINUX,
	OS_ANDROID,
	OS_MAC_OS,
	OS_RASPERRY
};

struct K15_OSLayerContext
{
	///Window Management
	createWindowFnc createWindow;
	setWindowDimensionFnc setWindowDimension;
	setWindowFullscreenFnc setWindowFullscreen;
	setWindowTitleFnc setWindowTitle;
	closeWindowFnc closeWindow;

	///Event management
	pumpSystemEventsFnc pumpSystemEvents;

	sleepFnc sleep;
	getErrorFnc getError;

	OSIdentifier systemIdentifier;

	void* userData;
	K15_Window* window;
};

K15_OSLayerContext* K15_GetOSLayerContext();

void K15_InternalSetOSLayerContext(K15_OSLayerContext p_OSLayerContext);

#endif //_K15_OSLayer_OSContext_h_