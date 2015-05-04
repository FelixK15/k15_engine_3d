#include "android/K15_WindowAndroid.h"
#include "android/K15_EnvironmentAndroid.h"

#include "K15_OSLayer_OSContext.h"
#include "K15_OSLayer_Window.h"

/*********************************************************************************/
uint8 K15_AndroidCreateWindow(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, uint32 p_Flags, uint8 p_MonitorIndex)
{
	K15_AndroidWindow* androidWindow = (K15_AndroidWindow*)malloc(sizeof(K15_AndroidWindow));
	K15_AndroidContext* androidContext = (K15_AndroidContext*)p_OSContext->userData;

	if (!androidWindow)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	jobject surfaceHandle = androidContext->surfaceHandle;
	JNIEnv* jniEnv = androidContext->jniEnv;

	ANativeWindow* nativeWindow = 0;
	
	K15_JNI_CALL(jniEnv, nativeWindow = ANativeWindow_fromSurface(jniEnv, surfaceHandle));
	
	if (!nativeWindow)
	{
		return K15_OS_ERROR_SYSTEM;	
	}

	androidWindow->nativeWindow = nativeWindow;

	p_Window->userData = (void*)androidWindow;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AndroidSetWindowDimension(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, uint32 p_Height, uint32 p_Width)
{
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AndroidSetWindowFullscreen(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, bool8 p_Fullscreen)
{
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AndroidSetWindowTitle(K15_OSLayerContext* p_OSContext, K15_Window* p_Window, char* p_Title)
{
	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AndroidCloseWindow(K15_OSLayerContext* p_OSContext, K15_Window* p_Window)
{
	return K15_SUCCESS;
}
/*********************************************************************************/