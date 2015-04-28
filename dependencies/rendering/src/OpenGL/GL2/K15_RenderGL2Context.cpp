#include "OpenGL/K15_RenderGLContext.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"
#elif defined K15_OS_ANDROID
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#endif 

#include "K15_RenderContext.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderTargetDesc.h"

#include <K15_Logging.h>

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>

#include "OpenGL/GL2/K15_RenderGL2Conversion.cpp"
#include "OpenGL/GL2/K15_RenderGL2Buffer.cpp"
//#include "OpenGL/GL2/K15_RenderGL2Texture.cpp"
#include "OpenGL/GL2/K15_RenderGL2Frame.cpp"
#include "OpenGL/GL2/K15_RenderGL2State.cpp"

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_GLRenderContext*, K15_OSLayerContext*);

#ifdef K15_OS_WINDOWS
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_Win32CreateGLContext;
#elif defined K15_OS_ANDROID
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_AndroidCreateGLContext;
#else
	intern K15_CreatePlatformContextFnc K15_CreateContext = 0;
#endif //K15_OS_WINDWOS

/*********************************************************************************/
intern void APIENTRY K15_DebugProcARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
	const char* sourceName = 0;
	const char* typeName = 0;

	switch(source)
	{
	case GL_DEBUG_SOURCE_API:
		{
			sourceName = "API";
			break;
		}
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		{
			sourceName = "Window System";
			break;
		}
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		{
			sourceName = "Shader Compiler";
			break;
		}
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		{
			sourceName = "Third Party";
			break;
		}
	case GL_DEBUG_SOURCE_APPLICATION:
		{
			sourceName = "Application";
			break;
		}
	}

	switch(type)
	{
	case GL_DEBUG_TYPE_ERROR:
		{
			typeName = "Error";
			break;
		}
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			typeName = "Deprecated";
			break;
		}
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			typeName = "Undefined Behavior";
			break;
		}
	case GL_DEBUG_TYPE_PORTABILITY:
		{
			typeName = "Portability";
			break;
		}
	case GL_DEBUG_TYPE_PERFORMANCE:
		{
			typeName = "Performance";
		}
	}

	K15_LOG_ERROR_MESSAGE("OpenGL %s Warning. Category: %s Message: %s", typeName, sourceName, message);
}
/*********************************************************************************/
intern int K15_CmpStrings(const void* a, const void* b)
{
	const char* stringA = (const char*)a;
	const char* stringB = (const char*)*(char**)b;

	return strcmp(stringA, stringB);
}
/*********************************************************************************/
intern int K15_CmpStringsSort(const void* a, const void* b)
{
	const char* stringA = (const char*)*(char**)a;
	const char* stringB = (const char*)*(char**)b;

	return strcmp(stringA, stringB);
}
/*********************************************************************************/
intern void K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	const char* separator = " ";
	const GLubyte* extensionsString = glGetString(GL_EXTENSIONS);

	size_t extensionStringLength = strlen((const char*)extensionsString);
	char* extensionStringBuffer = (char*)alloca(extensionStringLength + 1); //+1 for 0 terminator
	extensionStringBuffer[extensionStringLength] = 0;

	memcpy(extensionStringBuffer, extensionsString, extensionStringLength);

	int numExtensions = 0;

	/*********************************************************************************/
	//count extension
	int extensionStringIndex = 0;

	while (extensionStringBuffer[extensionStringIndex])
	{
		if (isspace(extensionStringBuffer[extensionStringIndex++]))
		{
			numExtensions = numExtensions + 1;
		}
	}
	/*********************************************************************************/

	char** extensions = (char**)malloc(sizeof(char*) * numExtensions);

	/*********************************************************************************/
	//get extension
	char* currentExtension = strtok(extensionStringBuffer, separator);
	numExtensions = 0;

	while (currentExtension)
	{
		size_t extensionLength = strlen(currentExtension); 
		char* currentExtensionBuffer = (char*)malloc(extensionLength + 1); //+1 for 0 terminator
		memcpy(currentExtensionBuffer, currentExtension, extensionLength);
		currentExtensionBuffer[extensionLength] = 0;

		extensions[numExtensions++] = currentExtensionBuffer;

		currentExtension = strtok(0, separator);
	}
	/*********************************************************************************/

	K15_Sort(extensions, numExtensions, sizeof(char*), K15_CmpStringsSort);

	p_GLRenderContext->extensions.count = numExtensions;
	p_GLRenderContext->extensions.names = extensions;
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	uint textureFormatMask = 0;

	if (K15_Search("GL_IMG_texture_compression_pvrtc", p_GLRenderContext->extensions.names,
		 p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		textureFormatMask |= K15_RENDER_GL_PVRTC_FLAG;
	}

	if ((K15_Search("GL_AMD_compressed_ATC_texture", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0) ||
		(K15_Search("GL_ATI_texture_compression_atitc", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)||
		(K15_Search("GL_EXT_texture_compression_latc", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)||
		(K15_Search("GL_NV_texture_compression_latc", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0))
	{
		textureFormatMask |= K15_RENDER_GL_ATC_FLAG;
	}

	if (K15_Search("GL_AMD_compressed_3DC_texture", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		textureFormatMask |= K15_RENDER_GL_3DC_FLAG;
	}
	
	if ((K15_Search("GL_EXT_texture_compression_dxt3", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0) ||
		(K15_Search("GL_EXT_texture_compression_s3tc", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0) ||
		(K15_Search("GL_OES_texture_compression_S3TC", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0) ||
		(K15_Search("GL_NV_texture_compression_s3tc_update", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0))
	{
		textureFormatMask |= K15_RENDER_GL_DXT_3_FLAG;
	}

	if (K15_Search("GL_EXT_texture_compression_dxt1", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		textureFormatMask |= K15_RENDER_GL_DXT_1_FLAG;
	}

	if (K15_Search("GL_EXT_texture_compression_dxt5", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		textureFormatMask |= K15_RENDER_GL_DXT_5_FLAG;
	}

	if (K15_Search("GL_KHR_debug", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)kglGetProcAddress("glDebugMessageCallbackKHR");
	
		if (kglDebugMessageCallback)
		{
			kglDebugMessageCallback(K15_DebugProcARB, 0);
		}
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern void K15_InternalExtractVersion(K15_GLRenderContext* p_GLRenderContext)
{
	const GLubyte* versionString = glGetString(GL_VERSION);

	size_t versionStringLength = strlen((const char*)versionString);
	char* versionStringBuffer = (char*)alloca(versionStringLength + 1); //+1 for 0 terminator
	memcpy(versionStringBuffer, versionString, versionStringLength);
	versionStringBuffer[versionStringLength] = 0;

	int majorVersion = 0;
	int minorVersion = 0;
	//first guess: get first two numbers from string
	for (size_t versionStringIndex = 0;
		 versionStringIndex < versionStringLength;
		 ++versionStringIndex)
	{
		if (isdigit(versionStringBuffer[versionStringIndex]))
		{
			if (majorVersion == 0)
			{
				majorVersion = atoi(versionStringBuffer + versionStringIndex);
			}
			else
			{
				minorVersion = atoi(versionStringBuffer + versionStringIndex);
				break;
			}
		}
	}

	p_GLRenderContext->version.major = majorVersion;
	p_GLRenderContext->version.minor = minorVersion;
}
/*********************************************************************************/
uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)malloc(sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	assert(K15_CreateContext);

	uint8 result = K15_CreateContext(glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		const char* errorMessage = "Could not create GL2 Context.";
		size_t errorMessageLength = (uint32)strlen(errorMessage);
		K15_SetRenderContextError(p_RenderContext, errorMessage, errorMessageLength);

		return result;
	}

	glContext->vendorString = glGetString(GL_VENDOR);
	glContext->rendererString = glGetString(GL_RENDERER);

	K15_InternalExtractVersion(glContext);

	if ((glContext->version.major < K15_MIN_GL_VERSION_MAJOR) ||
		(glContext->version.major == K15_MIN_GL_VERSION_MAJOR &&
		 glContext->version.minor < K15_MIN_GL_VERSION_MINOR))
	{
		const char* errorMessage = "Invalid OpenGL Version.";
		size_t errorMessageLength = (uint32)strlen(errorMessage);
		K15_SetRenderContextError(p_RenderContext, errorMessage, errorMessageLength);

		return K15_OS_ERROR_SYSTEM;
	}

	K15_GLGetExtensions(glContext);

	result = K15_GLLoadExtensions(glContext);

	if (result != K15_SUCCESS)
	{
		const char* errorMessage = "Missing OpenGL Extensions.";
		size_t errorMessageLength = (uint32)strlen(errorMessage);
		K15_SetRenderContextError(p_RenderContext, errorMessage, errorMessageLength);

		return result;
	}

	//buffer
	p_RenderContext->commandProcessing.bufferManagement.createBuffer = K15_GLCreateBuffer;
	p_RenderContext->commandProcessing.bufferManagement.deleteBuffer = K15_GLDeleteBuffer;
	p_RenderContext->commandProcessing.bufferManagement.updateBuffer = K15_GLUpdateBuffer;

	//states
	p_RenderContext->commandProcessing.stateManagement.setBlendState = K15_GLSetBlendStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setDepthState = K15_GLSetDepthStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setRasterizerState = K15_GLSetRasterizerStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setStencilState = K15_GLSetStencilStateDesc;

	//screen
	p_RenderContext->commandProcessing.screenManagement.clearScreen = K15_GLClearScreen;

	p_RenderContext->userData = (void*)glContext;

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	return K15_SUCCESS;
}
/*********************************************************************************/