#include "OpenGL/K15_RenderGLContext.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"
#elif defined K15_OS_ANDROID
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#endif 

#define K15_OPENGL_ERROR_MISSING_EXTENSION 10

#include "K15_RenderContext.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderTargetDesc.h"

#include <K15_Logging.h>

#include <K15_DefaultCLibraries.h>
#include <K15_OSContext.h>
#include <K15_Window.h>
#include <K15_FileSystem.h>

#include "OpenGL/GL3/K15_RenderGL3Conversion.cpp"
#include "OpenGL/GL3/K15_RenderGL3Buffer.cpp"
#include "OpenGL/GL3/K15_RenderGL3Frame.cpp"
#include "OpenGL/GL3/K15_RenderGL3Program.cpp"
#include "OpenGL/GL3/K15_RenderGL3State.cpp"
#include "OpenGL/GL3/K15_RenderGL3Texture.cpp"
#include "OpenGL/GL3/K15_RenderGL3Sampler.cpp"
#include "OpenGL/GL3/K15_RenderGL3RenderTarget.cpp"
#include "OpenGL/GL3/K15_RenderGL3Draw.cpp"

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_GLRenderContext*, K15Context*);

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
intern inline void K15_InternalGLSetFunctionPointers(K15_RenderContext* p_RenderContext)
{
	//screen management
	p_RenderContext->commandProcessing.screenManagement.clearScreen = K15_GLClearScreen;

	//buffer management
	p_RenderContext->commandProcessing.bufferManagement.createBuffer = K15_GLCreateBuffer;
	p_RenderContext->commandProcessing.bufferManagement.updateBuffer = K15_GLUpdateBuffer;
	p_RenderContext->commandProcessing.bufferManagement.deleteBuffer = K15_GLDeleteBuffer;

	//program management
	p_RenderContext->commandProcessing.programManagement.createProgram = K15_GLCreateProgram;
	p_RenderContext->commandProcessing.programManagement.deleteProgram = K15_GLDeleteProgram;
	p_RenderContext->commandProcessing.programManagement.updateUniform = K15_GLUpdateUniform;

	//texture management
	p_RenderContext->commandProcessing.textureManagement.createTexture = K15_GLCreateTexture;
	p_RenderContext->commandProcessing.textureManagement.updateTexture = K15_GLUpdateTexture;
	p_RenderContext->commandProcessing.textureManagement.deleteTexture = K15_GLDeleteTexture;

	//sampler management
	p_RenderContext->commandProcessing.samplerManagement.createSampler = K15_GLCreateSampler;
	p_RenderContext->commandProcessing.samplerManagement.deleteSampler = K15_GLDeleteSampler;

	//state management
	p_RenderContext->commandProcessing.stateManagement.setDepthState = K15_GLSetDepthStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setBlendState = K15_GLSetBlendStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setRasterizerState = K15_GLSetRasterizerStateDesc;
	p_RenderContext->commandProcessing.stateManagement.setStencilState = K15_GLSetStencilStateDesc;

	//render target management
	p_RenderContext->commandProcessing.renderTargetManagement.createRenderTarget = K15_GLCreateRenderTarget;
	p_RenderContext->commandProcessing.renderTargetManagement.bindRenderTarget = K15_GLBindRenderTarget;
	p_RenderContext->commandProcessing.renderTargetManagement.unbindRenderTarget = K15_GLUnbindRenderTarget;
	p_RenderContext->commandProcessing.renderTargetManagement.deleteRenderTarget = K15_GLDeleteRenderTarget;

	//drawing
	p_RenderContext->commandProcessing.drawManagement.drawFullscreenQuad = K15_GLDrawFullscreenQuad;
}
/*********************************************************************************/
intern int K15_CmpStrings(const void* a, const void* b)
{
	const char* stringA = (char*)a;
	const char* const *stringB = (char**)b;

	return strcmp(stringA, *stringB);
}
/*********************************************************************************/
intern int K15_CmpStringsSort(const void* a, const void* b)
{
	const char* const *stringA = (char**)a;
	const char* const *stringB = (char**)b;

	return strcmp(*stringA, *stringB);
}
/*********************************************************************************/
intern void K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	//try to get extensions via glGetStringi...
	if (kglGetStringi)
	{
		GLint numExtensions = 0;
		kglGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

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
		const GLubyte* extensionsString = kglGetString(GL_EXTENSIONS);

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
		p_GLRenderContext->extensions.count = numExtensions;
		p_GLRenderContext->extensions.names = extensions;
	}

	K15_Sort(p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStringsSort);
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	uint8 result = K15_SUCCESS;

	K15_CHECK_ASSIGNMENT(kglGenBuffers, (PFNGLGENBUFFERSPROC)kglGetProcAddress("glGenBuffers"));
	K15_CHECK_ASSIGNMENT(kglVertexAttribPointer, (PFNGLVERTEXATTRIBPOINTERPROC)kglGetProcAddress("glVertexAttribPointer"));
	K15_CHECK_ASSIGNMENT(kglEnableVertexAttribArray, (PFNGLENABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glEnableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglDisableVertexAttribArray, (PFNGLDISABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glDisableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglBindBuffer, (PFNGLBINDBUFFERPROC)kglGetProcAddress("glBindBuffer"));
	K15_CHECK_ASSIGNMENT(kglDeleteBuffers, (PFNGLDELETEBUFFERSPROC)kglGetProcAddress("glDeleteBuffers"));
	K15_CHECK_ASSIGNMENT(kglGetProgramiv, (PFNGLGETPROGRAMIVPROC)kglGetProcAddress("glGetProgramiv"));
	K15_CHECK_ASSIGNMENT(kglDeleteProgram, (PFNGLDELETEPROGRAMPROC)kglGetProcAddress("glDeleteProgram"));
	K15_CHECK_ASSIGNMENT(kglGetActiveUniform, (PFNGLGETACTIVEUNIFORMPROC)kglGetProcAddress("glGetActiveUniform"));
	K15_CHECK_ASSIGNMENT(kglGetActiveAttrib, (PFNGLGETACTIVEATTRIBPROC)kglGetProcAddress("glGetActiveAttrib"));
	K15_CHECK_ASSIGNMENT(kglGetUniformLocation, (PFNGLGETUNIFORMLOCATIONPROC)kglGetProcAddress("glGetUniformLocation"));
	K15_CHECK_ASSIGNMENT(kglGetAttribLocation, (PFNGLGETATTRIBLOCATIONPROC)kglGetProcAddress("glGetAttribLocation"));
	K15_CHECK_ASSIGNMENT(kglBlendEquationSeparate, (PFNGLBLENDEQUATIONSEPARATEPROC)kglGetProcAddress("glBlendEquationSeparate"));
	K15_CHECK_ASSIGNMENT(kglBindFragDataLocation, (PFNGLBINDFRAGDATALOCATIONPROC)kglGetProcAddress("glBindFragDataLocation"));
	K15_CHECK_ASSIGNMENT(kglGetProgramInfoLog, (PFNGLGETPROGRAMINFOLOGPROC)kglGetProcAddress("glGetProgramInfoLog"));
	K15_CHECK_ASSIGNMENT(kglBlendFuncSeparate, (PFNGLBLENDFUNCSEPARATEPROC)kglGetProcAddress("glBlendFuncSeparate"));
	K15_CHECK_ASSIGNMENT(kglGenFramebuffers, (PFNGLGENFRAMEBUFFERSPROC)kglGetProcAddress("glGenFramebuffers"));
	K15_CHECK_ASSIGNMENT(kglBindFramebuffer, (PFNGLBINDFRAMEBUFFERPROC)kglGetProcAddress("glBindFramebuffer"));
	K15_CHECK_ASSIGNMENT(kglDeleteFramebuffers, (PFNGLDELETEFRAMEBUFFERSPROC)kglGetProcAddress("glDeleteFramebuffers"));
	K15_CHECK_ASSIGNMENT(kglGenRenderbuffers, (PFNGLGENRENDERBUFFERSPROC)kglGetProcAddress("glGenRenderbuffers"));
	K15_CHECK_ASSIGNMENT(kglBindRenderbuffer, (PFNGLBINDRENDERBUFFERPROC)kglGetProcAddress("glBindRenderbuffer"));
	K15_CHECK_ASSIGNMENT(kglDeleteRenderbuffers, (PFNGLDELETERENDERBUFFERSPROC)kglGetProcAddress("glDeleteRenderbuffers"));
	K15_CHECK_ASSIGNMENT(kglRenderbufferStorage, (PFNGLRENDERBUFFERSTORAGEPROC)kglGetProcAddress("glRenderbufferStorage"));
	K15_CHECK_ASSIGNMENT(kglFramebufferRenderbuffer, (PFNGLFRAMEBUFFERRENDERBUFFERPROC)kglGetProcAddress("glFramebufferRenderbuffer"));
	K15_CHECK_ASSIGNMENT(kglFramebufferTexture, (PFNGLFRAMEBUFFERTEXTUREPROC)kglGetProcAddress("glFramebufferTexture"));
	K15_CHECK_ASSIGNMENT(kglCheckFramebufferStatus, (PFNGLCHECKFRAMEBUFFERSTATUSPROC)kglGetProcAddress("glCheckFramebufferStatus"));
	K15_CHECK_ASSIGNMENT(kglRenderbufferStorageMultisample, (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)kglGetProcAddress("glRenderbufferStorageMultisample"));

	if(K15_Search("GL_AMD_debug_output", p_GLRenderContext->extensions.names, 
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglDebugMessageCallbackAMD, (PFNGLDEBUGMESSAGECALLBACKAMDPROC)kglGetProcAddress("glDebugMessageCallbackAMD"));

		kglDebugMessageCallbackAMD(K15_DebugProcAMD, 0);
	}
	else if(K15_Search("GL_ARB_debug_output", p_GLRenderContext->extensions.names, 
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglDebugMessageCallback, (PFNGLDEBUGMESSAGECALLBACKPROC)kglGetProcAddress("glDebugMessageCallback"));

		kglDebugMessageCallback(K15_DebugProcARB, 0);
	}
	else
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	if(K15_Search("GL_ARB_vertex_array_object", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglGenVertexArrays, (PFNGLGENVERTEXARRAYSPROC)kglGetProcAddress("glGenVertexArrays"));
		K15_CHECK_ASSIGNMENT(kglBindVertexArray, (PFNGLBINDVERTEXARRAYPROC)kglGetProcAddress("glBindVertexArray"));
		K15_CHECK_ASSIGNMENT(kglDeleteVertexArrays, (PFNGLDELETEVERTEXARRAYSPROC)kglGetProcAddress("glDeleteVertexArrays"));
	}
	else
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	if(K15_Search("GL_EXT_direct_state_access", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglTextureImage1DEXT, (PFNGLTEXTUREIMAGE1DEXTPROC)kglGetProcAddress("glTextureImage1DEXT"));
		K15_CHECK_ASSIGNMENT(kglTextureImage2DEXT, (PFNGLTEXTUREIMAGE2DEXTPROC)kglGetProcAddress("glTextureImage2DEXT"));
		K15_CHECK_ASSIGNMENT(kglTextureImage3DEXT, (PFNGLTEXTUREIMAGE3DEXTPROC)kglGetProcAddress("glTextureImage3DEXT"));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage1DEXT, (PFNGLTEXTURESUBIMAGE1DEXTPROC)kglGetProcAddress("glTextureSubImage1DEXT"));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage2DEXT, (PFNGLTEXTURESUBIMAGE2DEXTPROC)kglGetProcAddress("glTextureSubImage2DEXT"));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage3DEXT, (PFNGLTEXTURESUBIMAGE3DEXTPROC)kglGetProcAddress("glTextureSubImage3DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureImage1DEXT, (PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC)kglGetProcAddress("glCompressedTextureImage1DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureImage2DEXT, (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC)kglGetProcAddress("glCompressedTextureImage2DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureImage3DEXT, (PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC)kglGetProcAddress("glCompressedTextureImage3DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage1DEXT, (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC)kglGetProcAddress("glCompressedTextureSubImage1DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage2DEXT, (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC)kglGetProcAddress("glCompressedTextureSubImage2DEXT"));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage3DEXT, (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC)kglGetProcAddress("glCompressedTextureSubImage3DEXT"));
		K15_CHECK_ASSIGNMENT(kglGenerateTextureMipmapEXT, (PFNGLGENERATETEXTUREMIPMAPEXTPROC)kglGetProcAddress("glGenerateTextureMipmapEXT"));
		K15_CHECK_ASSIGNMENT(kglNamedBufferDataEXT, (PFNGLNAMEDBUFFERDATAEXTPROC)kglGetProcAddress("glNamedBufferDataEXT"));
		K15_CHECK_ASSIGNMENT(kglNamedBufferSubDataEXT, (PFNGLNAMEDBUFFERSUBDATAEXTPROC)kglGetProcAddress("glNamedBufferSubDataEXT"));
		K15_CHECK_ASSIGNMENT(kglMapNamedBufferEXT, (PFNGLMAPNAMEDBUFFEREXTPROC)kglGetProcAddress("glMapNamedBufferEXT"));
		K15_CHECK_ASSIGNMENT(kglMapNamedBufferRangeEXT, (PFNGLMAPNAMEDBUFFERRANGEEXTPROC)kglGetProcAddress("glMapNamedBufferRangeEXT"));
		K15_CHECK_ASSIGNMENT(kglUnmapNamedBufferEXT, (PFNGLUNMAPNAMEDBUFFEREXTPROC)kglGetProcAddress("glUnmapNamedBufferEXT"));
	}
	else
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	if(K15_Search("GL_ARB_sampler_objects", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglGenSamplers, (PFNGLGENSAMPLERSPROC)kglGetProcAddress("glGenSamplers"));
		K15_CHECK_ASSIGNMENT(kglDeleteSamplers, (PFNGLDELETESAMPLERSPROC)kglGetProcAddress("glDeleteSamplers"));
		K15_CHECK_ASSIGNMENT(kglBindSampler, (PFNGLBINDSAMPLERPROC)kglGetProcAddress("glBindSamplers"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameteri, (PFNGLSAMPLERPARAMETERIPROC)kglGetProcAddress("glSamplerParameteri"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterf, (PFNGLSAMPLERPARAMETERFPROC)kglGetProcAddress("glSamplerParameterf"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameteriv, (PFNGLSAMPLERPARAMETERIVPROC)kglGetProcAddress("glSamplerParameteriv"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterfv, (PFNGLSAMPLERPARAMETERFVPROC)kglGetProcAddress("glSamplerParameterfv"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterIiv, (PFNGLSAMPLERPARAMETERIIVPROC)kglGetProcAddress("glSamplerParameterIiv"));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterIuiv, (PFNGLSAMPLERPARAMETERIUIVPROC)kglGetProcAddress("glSamplerParameterIuiv"));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterIiv, (PFNGLGETSAMPLERPARAMETERIIVPROC)kglGetProcAddress("glGetSamplerParameterIiv"));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameteriv, (PFNGLGETSAMPLERPARAMETERIVPROC)kglGetProcAddress("glGetSamplerParameteriv"));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterfv, (PFNGLGETSAMPLERPARAMETERFVPROC)kglGetProcAddress("glGetSamplerParameterfv"));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterIuiv, (PFNGLGETSAMPLERPARAMETERIUIVPROC)kglGetProcAddress("glGetSamplerParameterIuiv"));
	}
	else
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	if (K15_Search("GL_EXT_texture_compression_s3tc", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) == 0)
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	if(K15_Search("GL_ARB_separate_shader_objects", p_GLRenderContext->extensions.names,
		p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStrings) != 0)
	{
		K15_CHECK_ASSIGNMENT(kglGenProgramPipelines, (PFNGLGENPROGRAMPIPELINESPROC)kglGetProcAddress("glGenProgramPipelines"));
		K15_CHECK_ASSIGNMENT(kglBindProgramPipeline, (PFNGLBINDPROGRAMPIPELINEPROC)kglGetProcAddress("glBindProgramPipeline"));
		K15_CHECK_ASSIGNMENT(kglDeleteProgramPipelines, (PFNGLDELETEPROGRAMPIPELINESPROC)kglGetProcAddress("glDeleteProgramPipelines"));
		K15_CHECK_ASSIGNMENT(kglUseProgramStages, (PFNGLUSEPROGRAMSTAGESPROC)kglGetProcAddress("glUseProgramStages"));
		K15_CHECK_ASSIGNMENT(kglActiveShaderProgram, (PFNGLACTIVESHADERPROGRAMPROC)kglGetProcAddress("glActiveShaderProgram"));
		K15_CHECK_ASSIGNMENT(kglCreateShaderProgramv, (PFNGLCREATESHADERPROGRAMVPROC)kglGetProcAddress("glCreateShaderProgramv"));
		K15_CHECK_ASSIGNMENT(kglProgramParameteri, (PFNGLPROGRAMPARAMETERIPROC)kglGetProcAddress("glProgramParameteri"));
	
		K15_CHECK_ASSIGNMENT(kglProgramUniform1i, (PFNGLPROGRAMUNIFORM1IPROC)kglGetProcAddress("glProgramUniform1i"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2i, (PFNGLPROGRAMUNIFORM2IPROC)kglGetProcAddress("glProgramUniform2i"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3i, (PFNGLPROGRAMUNIFORM3IPROC)kglGetProcAddress("glProgramUniform3i"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4i, (PFNGLPROGRAMUNIFORM4IPROC)kglGetProcAddress("glProgramUniform4i"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1ui, (PFNGLPROGRAMUNIFORM1UIPROC)kglGetProcAddress("glProgramUniform1ui"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2ui, (PFNGLPROGRAMUNIFORM2UIPROC)kglGetProcAddress("glProgramUniform2ui"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3ui, (PFNGLPROGRAMUNIFORM3UIPROC)kglGetProcAddress("glProgramUniform3ui"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4ui, (PFNGLPROGRAMUNIFORM4UIPROC)kglGetProcAddress("glProgramUniform4ui"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1f, (PFNGLPROGRAMUNIFORM1FPROC)kglGetProcAddress("glProgramUniform1f"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2f, (PFNGLPROGRAMUNIFORM2FPROC)kglGetProcAddress("glProgramUniform2f"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3f, (PFNGLPROGRAMUNIFORM3FPROC)kglGetProcAddress("glProgramUniform3f"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4f, (PFNGLPROGRAMUNIFORM4FPROC)kglGetProcAddress("glProgramUniform4f"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1d, (PFNGLPROGRAMUNIFORM1DPROC)kglGetProcAddress("glProgramUniform1d"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2d, (PFNGLPROGRAMUNIFORM2DPROC)kglGetProcAddress("glProgramUniform2d"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3d, (PFNGLPROGRAMUNIFORM3DPROC)kglGetProcAddress("glProgramUniform3d"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4d, (PFNGLPROGRAMUNIFORM4DPROC)kglGetProcAddress("glProgramUniform4d"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1iv, (PFNGLPROGRAMUNIFORM1IVPROC)kglGetProcAddress("glProgramUniform1iv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2iv, (PFNGLPROGRAMUNIFORM2IVPROC)kglGetProcAddress("glProgramUniform2iv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3iv, (PFNGLPROGRAMUNIFORM3IVPROC)kglGetProcAddress("glProgramUniform3iv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4iv, (PFNGLPROGRAMUNIFORM4IVPROC)kglGetProcAddress("glProgramUniform4iv"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1uiv, (PFNGLPROGRAMUNIFORM1UIVPROC)kglGetProcAddress("glProgramUniform1uiv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2uiv, (PFNGLPROGRAMUNIFORM2UIVPROC)kglGetProcAddress("glProgramUniform2uiv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3uiv, (PFNGLPROGRAMUNIFORM3UIVPROC)kglGetProcAddress("glProgramUniform3uiv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4uiv, (PFNGLPROGRAMUNIFORM4UIVPROC)kglGetProcAddress("glProgramUniform4uiv"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1fv, (PFNGLPROGRAMUNIFORM1FVPROC)kglGetProcAddress("glProgramUniform1fv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2fv, (PFNGLPROGRAMUNIFORM2FVPROC)kglGetProcAddress("glProgramUniform2fv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3fv, (PFNGLPROGRAMUNIFORM3FVPROC)kglGetProcAddress("glProgramUniform3fv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4fv, (PFNGLPROGRAMUNIFORM4FVPROC)kglGetProcAddress("glProgramUniform4fv"));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1dv, (PFNGLPROGRAMUNIFORM1DVPROC)kglGetProcAddress("glProgramUniform1dv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2dv, (PFNGLPROGRAMUNIFORM2DVPROC)kglGetProcAddress("glProgramUniform2dv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3dv, (PFNGLPROGRAMUNIFORM3DVPROC)kglGetProcAddress("glProgramUniform3dv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4dv, (PFNGLPROGRAMUNIFORM4DVPROC)kglGetProcAddress("glProgramUniform4dv"));

		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix2fv, (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)kglGetProcAddress("glProgramUniformMatrix2fv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix3fv, (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)kglGetProcAddress("glProgramUniformMatrix3fv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix4fv, (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)kglGetProcAddress("glProgramUniformMatrix4fv"));

		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix2dv, (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)kglGetProcAddress("glProgramUniformMatrix2dv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix3dv, (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)kglGetProcAddress("glProgramUniformMatrix3dv"));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix4dv, (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)kglGetProcAddress("glProgramUniformMatrix4dv"));

		K15_CHECK_ASSIGNMENT(kglValidateProgramPipeline, (PFNGLVALIDATEPROGRAMPIPELINEPROC)kglGetProcAddress("glValidateProgramPipeline"));
		K15_CHECK_ASSIGNMENT(kglGetProgramPipelineInfoLog, (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)kglGetProcAddress("glGetProgramPipelineInfoLog"));
	}
	else
	{
		result = K15_OPENGL_ERROR_MISSING_EXTENSION;
	}

	return result;
}
/*********************************************************************************/
intern void K15_InternalGLGetRenderCapabilities(K15_RenderContext* p_RenderContext)
{
	K15_RenderCapabilities* capabilities = &p_RenderContext->capabilities;

	GLfloat glMaxAnistropy = 1.0f;
	GLint glMaxColorAttachments = 1;
	GLint glMaxSamples = 0;

	K15_OPENGL_CALL(kglGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glMaxAnistropy));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &glMaxColorAttachments));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_SAMPLES, &glMaxSamples));

	capabilities->maxAnisotropy = (float)glMaxAnistropy;
	capabilities->maxRenderTargets = (uint32)glMaxColorAttachments;
	capabilities->maxSamples = (uint32)glMaxSamples;
}
/*********************************************************************************/
intern inline void K15_InternalGLCreateFullscreenQuadVBO(K15_GLRenderContext* p_GlContext)
{
	GLfloat glQuadVertices[] = {
		//pos vertex 1
		-1.f, -1.f, 1.f,
		//UV vertex 1
		0.f, 0.f,

		//pos vertex 2
		-1.f, 1.f, 1.f,
		//UV vertex 2
		0.f, 1.f,

		//pos vertex 3
		1.f, 1.f, 1.f,
		//UV vertex 3
		1.f, 1.f,

		//pos vertex 4
		1.f, -1.f, 1.f,
		//UV vertex 4
		1.f, 0.f
	};

	GLshort glQuadIndices[] = {
		0, 1, 2,

		2, 3, 0
	};

	GLuint glVBO = 0;
	GLuint glIBO = 0;
	GLuint glVAO = 0;
	GLuint glPPL = 0;
	GLuint glVS = 0;

	const char* glVertexShaderCode[] = {
		K15_GLGenerateGLSLHeaderCode(GL_VERTEX_SHADER),
		"in vec3 position;\n"
		"in vec2 uv;\n"
		"out vec2 v_Color;\n"
		"void main(void) {\n"
		"v_Color = uv;\n"
		"gl_Position = vec4(position, 1.0f); }\n"
	};

	GLint activeAttributes = 0;
	GLint maxActiveAttributesNameLength = 0;

	GLint vsPositionRegister = 0;
	GLint vsTexcoordRegister = 0;

	K15_OPENGL_CALL(glVS = kglCreateShaderProgramv(GL_VERTEX_SHADER, 2, glVertexShaderCode));

	K15_OPENGL_CALL(kglGetProgramiv(glVS, GL_ACTIVE_ATTRIBUTES, &activeAttributes));
	K15_OPENGL_CALL(kglGetProgramiv(glVS, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxActiveAttributesNameLength));

	//ensure uv & position are present
	assert(activeAttributes == 2);

	K15_OPENGL_CALL(vsPositionRegister = kglGetAttribLocation(glVS, "position"));
	K15_OPENGL_CALL(vsTexcoordRegister = kglGetAttribLocation(glVS, "uv"));

	K15_OPENGL_CALL(kglGenProgramPipelines(1, &glPPL));
	K15_OPENGL_CALL(kglGenVertexArrays(1, &glVAO));
	K15_OPENGL_CALL(kglGenBuffers(1, &glVBO));
	K15_OPENGL_CALL(kglGenBuffers(1, &glIBO));

	K15_OPENGL_CALL(kglBindBuffer(GL_ARRAY_BUFFER, glVBO));
	K15_OPENGL_CALL(kglBindVertexArray(glVAO));
	K15_OPENGL_CALL(kglBindProgramPipeline(glPPL));


	K15_OPENGL_CALL(kglUseProgramStages(glPPL, GL_VERTEX_SHADER_BIT, glVS));
	K15_OPENGL_CALL(kglNamedBufferDataEXT(glVBO, sizeof(glQuadVertices), glQuadVertices, GL_STATIC_DRAW));
	K15_OPENGL_CALL(kglNamedBufferDataEXT(glIBO, sizeof(glQuadIndices), glQuadIndices, GL_STATIC_DRAW));

	//Enable attributes (0 = position, 1 = UV)
	K15_OPENGL_CALL(kglEnableVertexAttribArray(vsPositionRegister));
	K15_OPENGL_CALL(kglEnableVertexAttribArray(vsTexcoordRegister));
	K15_OPENGL_CALL(kglVertexAttribPointer(vsPositionRegister, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0));
	K15_OPENGL_CALL(kglVertexAttribPointer(vsTexcoordRegister, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const void*)(sizeof(GLfloat) * 3)));

	K15_OPENGL_CALL(kglBindProgramPipeline(0));
	K15_OPENGL_CALL(kglBindBuffer(GL_ARRAY_BUFFER, 0));
	K15_OPENGL_CALL(kglBindVertexArray(0));


	p_GlContext->gl3.fullscreenQuadVBO = glVBO;
	p_GlContext->gl3.fullscreenQuadIBO = glIBO;
	p_GlContext->gl3.fullscreenQuadPPL = glPPL;
	p_GlContext->gl3.fullscreenQuadVAO = glVAO;
}
/*********************************************************************************/
uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15Context* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)malloc(sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	if (!K15_CreateContext)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	glContext->gl3.programPipelineHandle = 0;

	uint8 result = K15_CreateContext(glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	K15_InternalGLSetFunctionPointers(p_RenderContext);

	glContext->vendorString = kglGetString(GL_VENDOR);
	glContext->rendererString = kglGetString(GL_RENDERER);
	
	kglGetIntegerv(GL_MINOR_VERSION, &glContext->version.minor);
	kglGetIntegerv(GL_MAJOR_VERSION, &glContext->version.major);

	//glGetIntegerv fails for OpenGL ES 2.0
	GLenum versionRetrieveError = kglGetError();

	if(versionRetrieveError == GL_INVALID_ENUM)
	{
		const GLubyte* versionString = kglGetString(GL_VERSION);
		char* token = strtok((char*)versionString, ".");
		
		glContext->version.major = atoi(token);

		token = strtok(0, ".");

		glContext->version.minor = atoi(token);
	}
	

	if (glContext->version.major != K15_MIN_GL_VERSION_MAJOR ||
		glContext->version.minor != K15_MIN_GL_VERSION_MINOR)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_GLGetExtensions(glContext);
	
	result = K15_GLLoadExtensions(glContext);

	if (result != K15_SUCCESS)
	{
		const char* errorMessage = "Missing OpenGL Extensions.";
		uint32 errorMessageLength = (uint32)strlen(errorMessage);
		K15_SetRenderContextError(p_RenderContext, errorMessage, errorMessageLength);

		return result;
	}

	p_RenderContext->userData = (void*)glContext;

	K15_GLCreateBuffers(glContext);
	K15_GLInitPrograms(glContext);

	K15_InternalGLGetRenderCapabilities(p_RenderContext);
	K15_InternalGLCreateFullscreenQuadVBO(glContext);

	K15_Window* window = p_OSLayerContext->window.window;

	//set initiale viewport
	kglViewport(0, 0, window->width, window->height);

	kglEnable(GL_STENCIL_TEST);
	kglEnable(GL_DEPTH_TEST);
	kglEnable(GL_BLEND);
	kglEnable(GL_POLYGON_OFFSET_FILL);

	kglClearColor(0.f, 0.f, 0.f, 1.f);

	//Create program pipeline
	GLuint programPipeline;
	K15_OPENGL_CALL(kglGenProgramPipelines(1, &programPipeline));
	K15_OPENGL_CALL(kglBindProgramPipeline(programPipeline));

	//Create vertex array object
	GLuint vertexArrayObject;
	K15_OPENGL_CALL(kglGenVertexArrays(1, &vertexArrayObject));
	K15_OPENGL_CALL(kglBindVertexArray(vertexArrayObject));

	glContext->gl3.vertexArrayHandle = vertexArrayObject;
	glContext->gl3.programPipelineHandle = programPipeline;
	glContext->gl3.framebufferHandle = 0; //no framebuffer currently bound

	return K15_SUCCESS;
}
/*********************************************************************************/