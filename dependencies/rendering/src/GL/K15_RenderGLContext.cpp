#include "GL/K15_RenderGLContext.h"
#include "GL/WGL/K15_Win32RenderWGLContext.h"

#include <K15_OSLayer_OSContext.h>

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_GLRenderContext*, K15_OSLayerContext*);

intern K15_CreatePlatformContextFnc ContextCreation[OS_COUNT] = {
#ifdef K15_OS_WINDOWS
	K15_Win32CreateGLContext,
#elif
	0,
#endif //K15_OS_WINDOWS
};

/*********************************************************************************/
intern int K15_CmpStrings(const void *a, const void* b)
{
	const char* stringA = (char*)a;
	const char* stringB = (char*)b;

	return strcmp(stringA, stringB);
}
/*********************************************************************************/
intern void K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	//try to get extensions via glGetStringi...
	PFNGLGETSTRINGIPROC kglGetStringi = (PFNGLGETSTRINGIPROC)p_GLRenderContext->kglGetProcAddress("glGetStringi");

	if (kglGetStringi)
	{
		GLint numExtensions = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		p_GLRenderContext->extensions.count = numExtensions;
		p_GLRenderContext->extensions.names = (char**)(malloc(sizeof(char*) * numExtensions));

		for(int i = 0; i < numExtensions; ++i)
		{
			char* extensionName = (char*)kglGetStringi(GL_EXTENSIONS, i);
			size_t extensionNameLength = strlen(extensionName);
		
			p_GLRenderContext->extensions.names[i] = (char*)malloc(extensionNameLength);
			memcpy(p_GLRenderContext->extensions.names[i], extensionName, extensionNameLength);
		}
	}
	else
	{
		// ... didn't work...get extensions via glGetString
		const char* separator = " ";
		char* extensionsString = (char*)glGetString(GL_EXTENSIONS);

		p_GLRenderContext->extensions.names = (char**)strtok(extensionsString, separator);

		while (extensionsString)
		{
			++p_GLRenderContext->extensions.count;
			++extensionsString;
		}
	}

	//sort extensions
	qsort(p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings);
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	if(bsearch("GL_AMD_debug_output", p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings))
	{
		kglDebugMessageCallbackAMD = glDebugMessageCallbackAMD;
	}
}
/*********************************************************************************/
uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)malloc(sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	K15_CreatePlatformContextFnc K15_CreatePlatformContext = ContextCreation[p_OSLayerContext->system.systemIdentifier];

	if (!K15_CreatePlatformContext)
	{
		return K15_ERROR_SYSTEM;
	}

	uint8 result = K15_CreatePlatformContext(glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	glContext->vendorString = glGetString(GL_VENDOR);
	glContext->rendererString = glGetString(GL_RENDERER);
	
	glGetIntegerv(GL_MINOR_VERSION, &glContext->version.minor);
	glGetIntegerv(GL_MINOR_VERSION, &glContext->version.major);

	if (glContext->version.major != K15_MIN_GL_VERSION_MAJOR ||
		glContext->version.minor != K15_MIN_GL_VERSION_MINOR)
	{
		return K15_ERROR_SYSTEM;
	}

	K15_GLGetExtensions(glContext);
	
	result = K15_GLLoadExtensions(glContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}
}
/*********************************************************************************/
