#include "OpenGL/K15_RenderGLContext.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"
#elif defined K15_OS_ANDROID
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#endif 

#include "K15_RenderContext.h"

#include <K15_Logging.h>

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>

//#include "OpenGL/GL2/K15_RenderGLBuffer.cpp"
//#include "OpenGL/GL2/K15_RenderGLFrame.cpp"

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_GLRenderContext*, K15_OSLayerContext*);

#ifdef K15_OS_WINDOWS
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_Win32CreateGLContext;
#elif defined K15_OS_ANDROID
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_AndroidCreateGLContext;
#else
	intern K15_CreatePlatformContextFnc K15_CreateContext = 0;
#endif //K15_OS_WINDWOS


/*********************************************************************************/
intern int K15_CmpStrings(const void* a, const void* b)
{
	const char* stringA = (char*)a;
	const char* const *stringB = (char**)b; //why?

	return strcmp(stringA, *stringB);
}
/*********************************************************************************/
intern void K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	
	return K15_SUCCESS;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext)
{
	
	return K15_SUCCESS;
}
/*********************************************************************************/