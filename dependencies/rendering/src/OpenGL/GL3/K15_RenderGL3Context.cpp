#include "OpenGL/K15_RenderGLContext.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"
#elif defined K15_OS_ANDROID
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#endif 

#include "K15_RenderContext.h"
#include "K15_RenderBufferDesc.h"

#include <K15_Logging.h>

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>

#include "OpenGL/GL3/K15_RenderGL3Buffer.cpp"
#include "OpenGL/GL3/K15_RenderGL3Frame.cpp"

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_GLRenderContext*, K15_OSLayerContext*);

#ifdef K15_OS_WINDOWS
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_Win32CreateGLContext;
#elif defined K15_OS_ANDROID
	intern K15_CreatePlatformContextFnc K15_CreateContext = K15_AndroidCreateGLContext;
#else
	intern K15_CreatePlatformContextFnc K15_CreateContext = 0;
#endif //K15_OS_WINDWOS


/*********************************************************************************/
intern void APIENTRY K15_DebugProcAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	const char* categoryName = 0;
	switch(category)
	{
		case GL_DEBUG_CATEGORY_API_ERROR_AMD:
		{
			categoryName = "API";
			break;
		}
		case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
		{
			categoryName = "Window System";
			break;
		}
		case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
		{
			categoryName = "Deprecation";
			break;
		}
		case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
		{
			categoryName = "Performance";
			break;
		}
		case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
		{
			categoryName = "Shader Compiler";
			break;
		}
		case GL_DEBUG_CATEGORY_APPLICATION_AMD:
		{
			categoryName = "Application";
			break;
		}
		case GL_DEBUG_CATEGORY_OTHER_AMD:
		{
			categoryName = "Other";
			break;
		}
	}

	K15_LOG_ERROR_MESSAGE("OpenGL Error. Category: %s Message: %s", categoryName, message);
}
/*********************************************************************************/
intern void APIENTRY K15_DebugProcARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
	const char* sourceName = 0;
	const char* typeName = 0;

	switch(source)
	{
		case GL_DEBUG_SOURCE_API_ARB:
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
	const char* stringA = (char*)a;
	const char* const *stringB = (char**)b; //why?

	return strcmp(stringA, *stringB);
}
/*********************************************************************************/
intern void K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	//try to get extensions via glGetStringi...
	PFNGLGETSTRINGIPROC kglGetStringi = (PFNGLGETSTRINGIPROC)kglGetProcAddress("glGetStringi");

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
		
			p_GLRenderContext->extensions.names[i] = (char*)malloc(extensionNameLength) + 1; // +1 for 0 terminator
			memcpy(p_GLRenderContext->extensions.names[i], extensionName, extensionNameLength);
			p_GLRenderContext->extensions.names[i][extensionNameLength] = 0;
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
	//K15_Sort(p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings);
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	kglGenBuffers = (PFNGLGENBUFFERSPROC)kglGetProcAddress("glGenBuffers");

	if(K15_Search("GL_AMD_debug_output", p_GLRenderContext->extensions.names, 
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglDebugMessageCallbackAMD = (PFNGLDEBUGMESSAGECALLBACKAMDPROC)kglGetProcAddress("glDebugMessageCallbackAMD");
		kglDebugMessageCallbackAMD(K15_DebugProcAMD, 0);
	}
	else if(K15_Search("GL_ARB_debug_output", p_GLRenderContext->extensions.names, 
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)kglGetProcAddress("glDebugMessageCallback");
		kglDebugMessageCallback(K15_DebugProcARB, 0);
	}

	if(K15_Search("GL_ARB_vertex_array_object", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)kglGetProcAddress("glGenVertexArrays");
		kglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)kglGetProcAddress("glBindVertexArray");
		kglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)kglGetProcAddress("glDeleteVertexArrays");
	}

	if(K15_Search("GL_EXT_direct_state_access", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglTextureImage1DEXT = (PFNGLTEXTUREIMAGE1DEXTPROC)kglGetProcAddress("glTextureImage1DEXT");
		kglTextureImage2DEXT = (PFNGLTEXTUREIMAGE2DEXTPROC)kglGetProcAddress("glTextureImage2DEXT");
		kglTextureImage3DEXT = (PFNGLTEXTUREIMAGE3DEXTPROC)kglGetProcAddress("glTextureImage3DEXT");
		kglTextureSubImage1DEXT = (PFNGLTEXTURESUBIMAGE1DEXTPROC)kglGetProcAddress("glTextureSubImage1DEXT");
		kglTextureSubImage2DEXT = (PFNGLTEXTURESUBIMAGE2DEXTPROC)kglGetProcAddress("glTextureSubImage2DEXT");
		kglTextureSubImage3DEXT = (PFNGLTEXTURESUBIMAGE3DEXTPROC)kglGetProcAddress("glTextureSubImage3DEXT");
		kglNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXTPROC)kglGetProcAddress("glNamedBufferDataEXT");
		kglNamedBufferSubDataEXT = (PFNGLNAMEDBUFFERSUBDATAEXTPROC)kglGetProcAddress("glNamedBufferSubDataEXT");
		kglMapNamedBufferEXT = (PFNGLMAPNAMEDBUFFEREXTPROC)kglGetProcAddress("glMapNamedBufferEXT");
		kglUnmapNamedBufferEXT = (PFNGLUNMAPNAMEDBUFFEREXTPROC)kglGetProcAddress("glUnmapNamedBufferEXT");
	}

	if(K15_Search("GL_ARB_sampler_objects", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglGenSamplers = (PFNGLGENSAMPLERSPROC)kglGetProcAddress("glGetSamplers");
		kglDeleteSamplers = (PFNGLDELETESAMPLERSPROC)kglGetProcAddress("glDeleteSamplers");
		kglBindSampler = (PFNGLBINDSAMPLERPROC)kglGetProcAddress("glBindSamplers");
		kglSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)kglGetProcAddress("glSamplerParameteri");
		kglSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)kglGetProcAddress("glSamplerParameterf");
		kglSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)kglGetProcAddress("glSamplerParameteriv");
		kglSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)kglGetProcAddress("glSamplerParameterfv");
		kglSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)kglGetProcAddress("glSamplerParameterIiv");
		kglSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)kglGetProcAddress("glSamplerParameterIuiv");
		kglGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)kglGetProcAddress("glGetSamplerParameterIiv");
		kglGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)kglGetProcAddress("glGetSamplerParameteriv");
		kglGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)kglGetProcAddress("glGetSamplerParameterfv");
		kglGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)kglGetProcAddress("glGetSamplerParameterIuiv");
	}

	if(K15_Search("GL_ARB_separate_shader_objects", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		kglGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)kglGetProcAddress("glGenProgramPipelines");
		kglBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)kglGetProcAddress("glBindProgramPipeline");
		kglDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)kglGetProcAddress("glDeleteProgramPipelines");
		kglUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)kglGetProcAddress("glUseProgramStages");
		kglActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)kglGetProcAddress("glActiveShaderProgram");
		kglCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)kglGetProcAddress("glCreateShaderProgramv");
		kglProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)kglGetProcAddress("glProgramParameteri");
	
		kglProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)kglGetProcAddress("glProgramUniform1i");
		kglProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)kglGetProcAddress("glProgramUniform2i");
		kglProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)kglGetProcAddress("glProgramUniform3i");
		kglProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)kglGetProcAddress("glProgramUniform4i");

		kglProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)kglGetProcAddress("glProgramUniform1ui");
		kglProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)kglGetProcAddress("glProgramUniform2ui");
		kglProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)kglGetProcAddress("glProgramUniform3ui");
		kglProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)kglGetProcAddress("glProgramUniform4ui");

		kglProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)kglGetProcAddress("glProgramUniform1f");
		kglProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)kglGetProcAddress("glProgramUniform2f");
		kglProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)kglGetProcAddress("glProgramUniform3f");
		kglProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)kglGetProcAddress("glProgramUniform4f");

		kglProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)kglGetProcAddress("glProgramUniform1d");
		kglProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)kglGetProcAddress("glProgramUniform2d");
		kglProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)kglGetProcAddress("glProgramUniform3d");
		kglProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)kglGetProcAddress("glProgramUniform4d");

		kglProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)kglGetProcAddress("glProgramUniform1iv");
		kglProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)kglGetProcAddress("glProgramUniform2iv");
		kglProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)kglGetProcAddress("glProgramUniform3iv");
		kglProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)kglGetProcAddress("glProgramUniform4iv");

		kglProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)kglGetProcAddress("glProgramUniform1uiv");
		kglProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)kglGetProcAddress("glProgramUniform2uiv");
		kglProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)kglGetProcAddress("glProgramUniform3uiv");
		kglProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)kglGetProcAddress("glProgramUniform4uiv");

		kglProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)kglGetProcAddress("glProgramUniform1fv");
		kglProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)kglGetProcAddress("glProgramUniform2fv");
		kglProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)kglGetProcAddress("glProgramUniform3fv");
		kglProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)kglGetProcAddress("glProgramUniform4fv");

		kglProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)kglGetProcAddress("glProgramUniform1dv");
		kglProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)kglGetProcAddress("glProgramUniform2dv");
		kglProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)kglGetProcAddress("glProgramUniform3dv");
		kglProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)kglGetProcAddress("glProgramUniform4dv");

		kglProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)kglGetProcAddress("glProgramUniformMatrix2fv");
		kglProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)kglGetProcAddress("glProgramUniformMatrix3fv");
		kglProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)kglGetProcAddress("glProgramUniformMatrix4fv");

		kglProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)kglGetProcAddress("glProgramUniformMatrix2dv");
		kglProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)kglGetProcAddress("glProgramUniformMatrix3dv");
		kglProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)kglGetProcAddress("glProgramUniformMatrix4dv");

		kglValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)kglGetProcAddress("glValidateProgramPipeline");
		kglGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)kglGetProcAddress("glGetProgramPipelineInfoLog");
	}

	return K15_SUCCESS;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)malloc(sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	if (!K15_CreateContext)
	{
		return K15_ERROR_SYSTEM;
	}

	uint8 result = K15_CreateContext(glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//screen management
	p_RenderContext->commandProcessing.screenManagement.clearScreen = K15_GLClearScreen;

	//buffer management
	p_RenderContext->commandProcessing.bufferManagement.createBuffer = K15_GLCreateBuffer;
	//p_RenderContext->processRenderCommand = K15_GLProcessRenderCommand;

	glContext->vendorString = glGetString(GL_VENDOR);
	glContext->rendererString = glGetString(GL_RENDERER);
	
	glGetIntegerv(GL_MINOR_VERSION, &glContext->version.minor);
	glGetIntegerv(GL_MINOR_VERSION, &glContext->version.major);

	//glGetIntegerv fails for OpenGL ES 2.0
	GLenum versionRetrieveError = glGetError();

	if(versionRetrieveError == GL_INVALID_ENUM)
	{
		const GLubyte* versionString = glGetString(GL_VERSION);
		char* token = strtok((char*)versionString, ".");
		
		glContext->version.major = atoi(token);

		token = strtok(0, ".");

		glContext->version.minor = atoi(token);

	}
	

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

	p_RenderContext->userData = (void*)glContext;

	K15_Window* window = p_OSLayerContext->window.window;

	//set initiale viewport
	glViewport(0, 0, window->width, window->height);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_OFFSET_FILL);

	glClearColor(0.f, 0.f, 0.f, 1.f);

	return K15_SUCCESS;
}
/*********************************************************************************/