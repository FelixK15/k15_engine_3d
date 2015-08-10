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
#include "K15_RenderViewportDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderTargetDesc.h"

#include <K15_String.h>
#include <K15_Logging.h>

#include <K15_DefaultCLibraries.h>
#include <K15_OSContext.h>
#include <K15_Window.h>
#include <K15_FileSystem.h>

#include "OpenGL/K15_RenderGLTypes.cpp"

#include "OpenGL/K15_RenderGLConversion.cpp"
#include "OpenGL/K15_RenderGLBuffer.cpp"
#include "OpenGL/K15_RenderGLFrame.cpp"
#include "OpenGL/K15_RenderGLProgram.cpp"
#include "OpenGL/K15_RenderGLState.cpp"
#include "OpenGL/K15_RenderGLTexture.cpp"
#include "OpenGL/K15_RenderGLSampler.cpp"
#include "OpenGL/K15_RenderGLRenderTarget.cpp"
#include "OpenGL/K15_RenderGLDraw.cpp"

typedef uint8 (*K15_CreatePlatformContextFnc)(K15_CustomMemoryAllocator*, K15_GLRenderContext*, K15_OSContext*);

#ifdef K15_OS_WINDOWS
	intern K15_CreatePlatformContextFnc K15_CreatePlatformSpecificContext = K15_Win32CreateGLContext;
#elif defined K15_OS_ANDROID
	intern K15_CreatePlatformContextFnc K15_CreatePlatformSpecificContext = K15_AndroidCreateGLContext;
#else
	intern K15_CreatePlatformContextFnc K15_CreatePlatformSpecificContext = 0;
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
intern inline void K15_InternalSetGLInterface(K15_RenderBackEnd* p_RenderBackEnd)
{
	//screen management
	p_RenderBackEnd->interface.clearScreen = K15_GLClearScreen;

	//buffer management
	p_RenderBackEnd->interface.createBuffer = K15_GLCreateBuffer;
	p_RenderBackEnd->interface.updateBuffer = K15_GLUpdateBuffer;
	p_RenderBackEnd->interface.bindBuffer = K15_GLBindBuffer;
	p_RenderBackEnd->interface.deleteBuffer = K15_GLDeleteBuffer;

	//program management
	p_RenderBackEnd->interface.createProgram = K15_GLCreateProgram;
	p_RenderBackEnd->interface.deleteProgram = K15_GLDeleteProgram;
	p_RenderBackEnd->interface.bindProgram = K15_GLBindProgram;
	p_RenderBackEnd->interface.updateUniform = K15_GLUpdateUniform;
	p_RenderBackEnd->interface.updateDirtyUniforms = K15_GLUpdateDirtyUniforms;

	//texture management
	p_RenderBackEnd->interface.createTexture = K15_GLCreateTexture;
	p_RenderBackEnd->interface.updateTexture = K15_GLUpdateTexture;
	p_RenderBackEnd->interface.bindTexture = K15_GLBindTexture;
	p_RenderBackEnd->interface.deleteTexture = K15_GLDeleteTexture;

	//sampler management
	p_RenderBackEnd->interface.createSampler = K15_GLCreateSampler;
	p_RenderBackEnd->interface.deleteSampler = K15_GLDeleteSampler;

	//state management
	p_RenderBackEnd->interface.setDepthState = K15_GLSetDepthStateDesc;
	p_RenderBackEnd->interface.setBlendState = K15_GLSetBlendStateDesc;
	p_RenderBackEnd->interface.setRasterizerState = K15_GLSetRasterizerStateDesc;
	p_RenderBackEnd->interface.setStencilState = K15_GLSetStencilStateDesc;

	//render target management
	p_RenderBackEnd->interface.createRenderTarget = K15_GLCreateRenderTarget;
	p_RenderBackEnd->interface.bindRenderTarget = K15_GLBindRenderTarget;
	p_RenderBackEnd->interface.unbindRenderTarget = K15_GLUnbindRenderTarget;
	p_RenderBackEnd->interface.deleteRenderTarget = K15_GLDeleteRenderTarget;

	//drawing
	p_RenderBackEnd->interface.drawFullscreenQuad = K15_GLDrawFullscreenQuad;
	p_RenderBackEnd->interface.drawIndexed = K15_GLDrawIndexed;
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
			uint32 extensionNameLength= (uint32)strlen(extensionName);
		
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

		uint32 extensionStringLength= (uint32)strlen((const char*)extensionsString);
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
			uint32 extensionLength= (uint32)strlen(currentExtension); 
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

	//sort extensions so we can do a binary search on them
	K15_Sort(p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStringsSort);
}
/*********************************************************************************/
intern bool8 K15_GLCheckMinSpecExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	char** extensions = p_GLRenderContext->extensions.names;
	uint32 numExtensions = p_GLRenderContext->extensions.count;

	char* extensionBuffer = (char*)alloca(512);

	for (uint32 extensionIndex = 0;
		extensionIndex < numExtensions;
		++extensionIndex)
	{
		char* extension = K15_ConvertToLowerIntoBuffer(extensions[extensionIndex], extensionBuffer);

		if (strstr(extension, "vertex_array_object"))
		{
			p_GLRenderContext->extensions.vertex_array_object.supported = K15_TRUE;
		}
		else if (strstr(extension, "sampler_objects"))
		{
			p_GLRenderContext->extensions.sampler_objects.supported = K15_TRUE;
		}
		else if (strstr(extension, "separate_shader_objects"))
		{
			p_GLRenderContext->extensions.separate_shader_objects.supported = K15_TRUE;
		}
		else if (strstr(extension, "framebuffer_objects"))
		{
			p_GLRenderContext->extensions.framebuffer_object.supported = K15_TRUE;
		}
		else if (strstr(extension, "debug"))
		{
			p_GLRenderContext->extensions.debug_output.supported = K15_TRUE;
		}
		else if (strstr(extension, "texture_compression_pvrtc"))
		{
			p_GLRenderContext->extensions.compression.supportedCompressionMask |= K15_RENDER_GL_PVRTC_FLAG;
		}
		else if (strstr(extension, "compressed_atc_texture") ||
				 strstr(extension, "texture_compression_atitc") ||
				 strstr(extension, "texture_compression_latc"))
		{
			p_GLRenderContext->extensions.compression.supportedCompressionMask |= K15_RENDER_GL_ATC_FLAG;
		}
		else if (strstr(extension, "texture_compression_dxt3") ||
				 strstr(extension, "texture_compression_s3tc"))
		{
			p_GLRenderContext->extensions.compression.supportedCompressionMask |= K15_RENDER_GL_DXT_3_FLAG;
		}
		else if (strstr(extension, "texture_compression_dxt1"))
		{
			p_GLRenderContext->extensions.compression.supportedCompressionMask |= K15_RENDER_GL_DXT_1_FLAG;
		}
		else if (strstr(extension, "texture_compression_dxt5"))
		{
			p_GLRenderContext->extensions.compression.supportedCompressionMask |= K15_RENDER_GL_DXT_5_FLAG;
		}
	}

	return K15_TRUE;
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	uint8 result = K15_SUCCESS;

	K15_CHECK_ASSIGNMENT(kglGenBuffers, (PFNGLGENBUFFERSPROC)kglGetProcAddress("glGenBuffers"));
	K15_CHECK_ASSIGNMENT(kglVertexAttribPointer, (PFNGLVERTEXATTRIBPOINTERPROC)kglGetProcAddress("glVertexAttribPointer"));
	K15_CHECK_ASSIGNMENT(kglEnableVertexAttribArray, (PFNGLENABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glEnableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglDisableVertexAttribArray, (PFNGLDISABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glDisableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglDrawElementsBaseVertex, (PFNGLDRAWELEMENTSBASEVERTEXPROC)kglGetProcAddress("glDrawElementsBaseVertex"));
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
intern void K15_InternalGLGetRenderCapabilities(K15_RenderFeatures* p_RenderFeatures)
{
	GLfloat glMaxAnistropy = 1.0f;
	GLint glMaxColorAttachments = 1;
	GLint glMaxSamples = 0;
	GLint glMaxTextureSize = 0;

	K15_OPENGL_CALL(kglGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glMaxAnistropy));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &glMaxColorAttachments));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_SAMPLES, &glMaxSamples));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize));

	p_RenderFeatures->maxAnisotropy = (float)glMaxAnistropy;
	p_RenderFeatures->maxRenderTargets = (uint32)glMaxColorAttachments;
	p_RenderFeatures->maxSamples = (uint32)glMaxSamples;
	p_RenderFeatures->maxTextureDimension = (uint32)glMaxTextureSize;
}
/*********************************************************************************/
result8 K15_GLCreateRenderContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	glContext->platformContextData = 0;

	if (!K15_CreatePlatformSpecificContext)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	glContext->extensions.separate_shader_objects.programPipelineHandle = 0;

	uint8 result = K15_CreatePlatformSpecificContext(p_MemoryAllocator, glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//clear bound objects arrays
	memset(glContext->glBoundObjects.boundBuffers, 0, sizeof(glContext->glBoundObjects.boundBuffers));
	memset(glContext->glBoundObjects.boundTextures, 0, sizeof(glContext->glBoundObjects.boundTextures));
	memset(glContext->glBoundObjects.boundPrograms, 0, sizeof(glContext->glBoundObjects.boundPrograms));

// 	TODO
// 	glContext->vendorString = kglGetString(GL_VENDOR);
// 	glContext->rendererString = kglGetString(GL_RENDERER);

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

	//1st check extensions
	K15_GLGetExtensions(glContext);
	
	//check if the min extension set is supported
	bool8 minSpecExtensionsSupported = K15_GLCheckMinSpecExtensions(glContext);

	//2nd load extensions
	result = K15_GLLoadExtensions(glContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//3rd get interface based on available extensions
	K15_InternalSetGLInterface(p_RenderBackEnd);

	//set pointer to gl context
	p_RenderBackEnd->specificRenderPlatform = (void*)glContext;

	//initialize program objects (incl. uniforms)
	K15_GLInitPrograms(glContext);

	//ask the renderer api what features it has
	K15_InternalGLGetRenderCapabilities(&p_RenderBackEnd->features);

	//Create program pipeline
	GLuint programPipeline;
	K15_OPENGL_CALL(kglGenProgramPipelines(1, &programPipeline));
	K15_OPENGL_CALL(kglBindProgramPipeline(programPipeline));

	//Create vertex array object
	GLuint vertexArrayObject;
	K15_OPENGL_CALL(kglGenVertexArrays(1, &vertexArrayObject));
	K15_OPENGL_CALL(kglBindVertexArray(vertexArrayObject));

	glContext->extensions.vertex_array_object.vertexArrayHandle = vertexArrayObject;
	glContext->extensions.separate_shader_objects.programPipelineHandle = programPipeline;
	glContext->extensions.framebuffer_object.frameBufferHandle = 0; //no framebuffer currently bound

	return K15_SUCCESS;
}
/*********************************************************************************/