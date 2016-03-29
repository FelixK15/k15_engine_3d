#include "OpenGL/K15_RenderGLContext.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"
#elif defined K15_OS_ANDROID
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#endif 

#define K15_OPENGL_ERROR_MISSING_EXTENSION 10

#define K15_CHECK_AND_SET_EXTENSIONS(c, s, e) if (!c->extensions.e.supported && strstr(s, #e)) { K15_InternalGetExtensionBoard(s, c->extensions.e.board); c->extensions.e.supported = K15_TRUE; }
#define K15_CHECK_AND_SET_EXTENSIONS_BIT(c, s, e, b, m) if ((c->extensions.m & b) == 0 && strstr(s, #e)) { c->extensions.m |= b; }

#include "K15_RenderContext.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderViewportDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderTargetDesc.h"
#include "K15_RenderGeometryDesc.h"

#include <K15_String.h>
#include <K15_Logging.h>

#include <K15_DefaultCLibraries.h>
#include <K15_OSContext.h>
#include <K15_Window.h>
#include <K15_FileSystem.h>

#include "OpenGL/K15_RenderGLTypes.cpp"

/*********************************************************************************/
intern inline void K15_InternalGenerateGLSLVersionString(K15_GLRenderContext* p_GLContext)
{
	GLint major = p_GLContext->version.major;
	GLint minor = p_GLContext->version.minor;
	char* glslVersionIdentifier = "#version 100";
	//https://www.opengl.org/wiki/Detecting_the_Shader_Model

	if (major == 2)
	{
		if (minor == 0)
		{
			glslVersionIdentifier = "#version 110";
		}
		else if (minor == 1)
		{
			glslVersionIdentifier = "#version 120";
		}
	}
	else if (major == 3 && minor < 3)
	{
		if (minor == 0)
		{
			glslVersionIdentifier = "#version 130";
		}
		else if (minor == 1)
		{
			glslVersionIdentifier = "#version 140";;
		}
		else if (minor == 2)
		{
			glslVersionIdentifier = "#version 150";
		}
	}
	else
	{
		//glsl version = gl version
		int glslVersion = major*100+minor*10;
		glslVersionIdentifier = (char*)malloc(13);
		sprintf(glslVersionIdentifier, "#version %d", glslVersion);
	}

	p_GLContext->glslVersionString = glslVersionIdentifier;
}
/*********************************************************************************/
intern inline K15_RenderResourceHandle K15_InternalAddGLObject(K15_GLRenderContext* p_GLContext, void* p_GLObjectData, uint32 p_GLObjectDataSize, uint32 p_GLObjectHash, K15_GLObjectType p_GLObjectType)
{
	K15_ASSERT(p_GLContext);
	K15_ASSERT(p_GLObjectData);

	uint32 index = p_GLObjectHash % K15_RENDER_GL_MAX_OBJECTS;
	//uint32 nextIndex = p_GLContext->lastFreeGLObjectIndex + 1;
	uint32 indexFound = 0;

	if (p_GLContext->glObjects[index].type == K15_GL_TYPE_INVALID)
	{
		indexFound = index;
	}

	if (!indexFound)
	{
		for (uint32 glObjectIndex = 0;
			glObjectIndex < K15_RENDER_GL_MAX_OBJECTS;
			++glObjectIndex)
		{
			indexFound = glObjectIndex;

			if (p_GLContext->glObjects[glObjectIndex].type == K15_GL_TYPE_INVALID)
			{
				break;
			}
		}
	}

	K15_ASSERT(indexFound != K15_RENDER_GL_MAX_OBJECTS);

	K15_GLObjectHeader* currentObjectHeader = &p_GLContext->glObjects[indexFound];
	currentObjectHeader->type = p_GLObjectType;
	//currentObjectHeader->offsetNextEntry = p_GLObjectDataSize + sizeof(K15_GLObjectHeader);
	currentObjectHeader->glObjectData = (byte*)K15_AllocateFromMemoryAllocator(p_GLContext->memoryAllocator, p_GLObjectDataSize);
	memcpy(currentObjectHeader->glObjectData, p_GLObjectData, p_GLObjectDataSize);

	//p_GLContext->lastFreeGLObjectIndex = indexFound;

	return (K15_RenderResourceHandle)indexFound;
}
/*********************************************************************************/
intern inline byte* K15_InternalGetGLObjectData(K15_GLRenderContext* p_GLContext, K15_RenderResourceHandle p_ResourceHandle, K15_GLObjectType p_GLObjectType)
{
	K15_ASSERT(p_GLContext);
	K15_ASSERT(p_ResourceHandle != K15_INVALID_GPU_RESOURCE_HANDLE);

	K15_GLObjectHeader* currentObjectHeader = &p_GLContext->glObjects[p_ResourceHandle];

	K15_ASSERT(currentObjectHeader->type == p_GLObjectType);

	return currentObjectHeader->glObjectData;
}
/*********************************************************************************/
intern inline bool8 K15_InternalRemoveGLObject(K15_GLRenderContext* p_GLContext, K15_RenderResourceHandle p_ResourceHandle, K15_GLObjectType p_GLObjectType)
{
	K15_ASSERT(p_GLContext);
	K15_ASSERT(p_ResourceHandle);
	K15_ASSERT(p_ResourceHandle != K15_INVALID_GPU_RESOURCE_HANDLE);

	K15_GLObjectHeader* objectHeader = &p_GLContext->glObjects[p_ResourceHandle];

	K15_ASSERT(objectHeader->type == p_GLObjectType);

	K15_FreeFromMemoryAllocator(p_GLContext->memoryAllocator, objectHeader->glObjectData);

	objectHeader->glObjectData = 0;
	//objectHeader->offsetNextEntry = sizeof(K15_GLObjectHeader);
	objectHeader->type = K15_GL_TYPE_INVALID;

	return K15_TRUE;
}
/*********************************************************************************/
intern inline void K15_InternalGetExtensionBoard(char* p_Extension, char* p_Buffer)
{
	//skip first 3 chars as they're most probably "GL_"
	char* board = p_Extension + 3;
	K15_CopyStringIntoBuffer(board, p_Buffer, 3);
}
/*********************************************************************************/
intern void K15_InternalGLMatchInputLayoutWithVertexFormat(K15_ShaderProcessorContext* p_ShaderProcessorContext, 
	K15_RenderVertexFormatDesc* p_VertexFormat, K15_GLInputLayout* p_InputLayout)
{
	for (uint32 vertexElementIndex = 0;
	vertexElementIndex < p_VertexFormat->numAttributes;
		++vertexElementIndex)
	{
		K15_RenderVertexFormatElementDesc* vertexElement = p_VertexFormat->elements + vertexElementIndex;
		bool inputElementMatch = false;

		for (uint32 inputIndex = 0;
		inputIndex < p_InputLayout->numInputElements;
			++inputIndex)
		{
			K15_GLInputLayoutElement* inputElement = p_InputLayout->inputElements + inputIndex;

			if (inputElement->semanticID == vertexElement->semanticID &&
				inputElement->typeID == vertexElement->typeID)
			{
				inputElementMatch = true;
				break;
			}
		}

		K15_ASSERT_TEXT(inputElementMatch, "Could not match vertex shader input layout with vertex format. "
			"(First mismatch: Vertex Element Index '%d' | Semantic: '%s' | Type: '%s')", vertexElementIndex, 
			K15_GetSemanticName(p_ShaderProcessorContext, vertexElement->semanticID),
			K15_GetTypeName(p_ShaderProcessorContext, vertexElement->typeID));
	}
}
/*********************************************************************************/
intern void K15_InternalGLEvaluatePipeline(K15_RenderBackEnd* p_RenderBackEnd, K15_GLRenderContext* p_GLContext)
{
	K15_GLProgram* glBoundVertexShader = p_GLContext->glBoundObjects.boundPrograms[K15_RENDER_PROGRAM_TYPE_VERTEX];
	K15_RenderVertexFormatDesc* vertexFormat = 0;
	uint32 vertexFormatHash = p_GLContext->glBoundObjects.boundVertexFormatHash;

	if (vertexFormatHash != 0)
	{
		vertexFormat = K15_GetCachedRenderVertexFormatDesc(&p_RenderBackEnd->vertexFormatCache, vertexFormatHash);
	}

	if (vertexFormat && glBoundVertexShader)
	{
		K15_InternalGLMatchInputLayoutWithVertexFormat(p_RenderBackEnd->shaderProcessorContext, 
			vertexFormat, &glBoundVertexShader->inputLayout);
	}
}
/*********************************************************************************/



#include "OpenGL/K15_RenderGLConversion.cpp"
#include "OpenGL/K15_RenderGLBuffer.cpp"
#include "OpenGL/K15_RenderGLFrame.cpp"
#include "OpenGL/K15_RenderGLState.cpp"
#include "OpenGL/K15_RenderGLTexture.cpp"
#include "OpenGL/K15_RenderGLProgram.cpp"
#include "OpenGL/K15_RenderGLVertexFormat.cpp"
#include "OpenGL/K15_RenderGLSampler.cpp"
// #include "OpenGL/K15_RenderGLRenderTarget.cpp"
#include "OpenGL/K15_RenderGLMaterial.cpp"
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
	p_RenderBackEnd->renderInterface.clearScreen = K15_GLClearScreen;
	p_RenderBackEnd->renderInterface.swapFrameBuffer = K15_GLSwapFrameBuffer;
	p_RenderBackEnd->renderInterface.resizeViewport = K15_GLResizeViewport;

	//state management
	p_RenderBackEnd->renderInterface.setDepthState = K15_GLSetDepthStateDesc;
	p_RenderBackEnd->renderInterface.setBlendState = K15_GLSetBlendStateDesc;
	p_RenderBackEnd->renderInterface.setRasterizerState = K15_GLSetRasterizerStateDesc;
	p_RenderBackEnd->renderInterface.setStencilState = K15_GLSetStencilStateDesc;

	//texture management
	p_RenderBackEnd->renderInterface.createTextureFromTextureDesc = K15_GLCreateTextureFromTextureDesc;
	p_RenderBackEnd->renderInterface.createSamplerFromSamplerDesc = K15_GLCreateSamplerFromSamplerDesc;
	p_RenderBackEnd->renderInterface.deleteTexture = K15_GLDeleteTexture;
	p_RenderBackEnd->renderInterface.deleteSampler = K15_GLDeleteSampler;

	//program management
	p_RenderBackEnd->renderInterface.createProgram = K15_GLCreateProgram;
	p_RenderBackEnd->renderInterface.deleteProgram = K15_GLDeleteProgram;

	//buffer management
	p_RenderBackEnd->renderInterface.updateVertexData = K15_GLUpdateVertexData;
	p_RenderBackEnd->renderInterface.freeVertexData = K15_GLFreeVertexData;

	//draw management
	p_RenderBackEnd->renderInterface.drawGeometry = K15_GLDrawGeometry;
}
/*********************************************************************************/
intern int K15_CmpStringsSort(const void* a, const void* b)
{
	const char* const *stringA = (char**)a;
	const char* const *stringB = (char**)b;

	return strcmp(*stringA, *stringB);
}
/*********************************************************************************/
intern char* K15_CreateGLExtensionString(const char* p_ExtensionsPointerString, char* p_Board, char* p_Buffer)
{
	size_t extensionStringLength = strlen(p_ExtensionsPointerString);
	memcpy(p_Buffer, p_ExtensionsPointerString, extensionStringLength);

	K15_ConvertToUpperIntoBuffer(p_Board, p_Buffer + extensionStringLength);

	return p_Buffer;
}
/*********************************************************************************/
intern void K15_GLGetExtensionStrings(K15_GLRenderContext* p_GLRenderContext, K15_CustomMemoryAllocator* p_MemoryAllocator)
{
	//try to get extensions via glGetStringi...
	if (kglGetStringi)
	{
		GLint numExtensions = 0;
		kglGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		p_GLRenderContext->extensions.count = numExtensions;
		p_GLRenderContext->extensions.names = (char**)(K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(char*) * numExtensions));

		for(int i = 0; i < numExtensions; ++i)
		{
			char* extensionName = (char*)kglGetStringi(GL_EXTENSIONS, i);
			uint32 extensionNameLength= (uint32)strlen(extensionName);
		
			p_GLRenderContext->extensions.names[i] = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator,extensionNameLength) + 1; // +1 for 0 terminator
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

		char** extensions = (char**)K15_AllocateFromMemoryAllocator(p_MemoryAllocator,sizeof(char*) * numExtensions);

		/*********************************************************************************/
		//get extension
		char* currentExtension = strtok(extensionStringBuffer, separator);
		numExtensions = 0;

		while (currentExtension)
		{
			uint32 extensionLength= (uint32)strlen(currentExtension); 
			char* currentExtensionBuffer = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator,extensionLength + 1); //+1 for 0 terminator
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
	qsort(p_GLRenderContext->extensions.names, p_GLRenderContext->extensions.count, sizeof(char*), K15_CmpStringsSort);
}
/*********************************************************************************/
intern void* K15_GLGetExtensionFunction(const char* p_ExtensionFunctionName, char* p_BoardAbbreveration)
{
	void* functionPointer = kglGetProcAddress(p_ExtensionFunctionName);

	if (!functionPointer)
	{
		char* functionName = K15_CreateGLExtensionString(p_ExtensionFunctionName, p_BoardAbbreveration, (char*)alloca(512));
		functionPointer = kglGetProcAddress(functionName);
	}

	return functionPointer;
}
/*********************************************************************************/
intern bool8 K15_GLGetExtensions(K15_GLRenderContext* p_GLRenderContext)
{
	char** extensions = p_GLRenderContext->extensions.names;
	uint32 numExtensions = p_GLRenderContext->extensions.count;

	char* extensionBuffer = (char*)alloca(512);

	for (uint32 extensionIndex = 0;
		extensionIndex < numExtensions;
		++extensionIndex)
	{
		char* extension = K15_ConvertToLowerIntoBuffer(extensions[extensionIndex], extensionBuffer);

		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, vertex_array_object);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, sampler_objects);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, separate_shader_objects);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, framebuffer_object);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, direct_state_access);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, debug);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, sparse_buffer);
		K15_CHECK_AND_SET_EXTENSIONS(p_GLRenderContext, extension, sparse_texture);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_pvrtc,	K15_RENDER_GL_PVRTC_FLAG,	compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, compressed_atc_texture,		K15_RENDER_GL_ATC_FLAG,		compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_atitc,	K15_RENDER_GL_ATC_FLAG,		compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_latc,	K15_RENDER_GL_ATC_FLAG,		compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_dxt3,	K15_RENDER_GL_DXT_3_FLAG,	compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_s3tc,	K15_RENDER_GL_DXT_3_FLAG,	compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_dxt1,	K15_RENDER_GL_DXT_1_FLAG,	compression.supportedCompressionMask);
		K15_CHECK_AND_SET_EXTENSIONS_BIT(p_GLRenderContext, extension, texture_compression_dxt5,	K15_RENDER_GL_DXT_5_FLAG,	compression.supportedCompressionMask);
	}

	return K15_TRUE;
}
/*********************************************************************************/
intern uint8 K15_GLLoadExtensionFunctions(K15_GLRenderContext* p_GLRenderContext)
{
	uint8 result = K15_SUCCESS;
	char* tempBuffer = (char*)alloca(256);

	K15_CHECK_ASSIGNMENT(kglCreateShader, (PFNGLCREATESHADERPROC)kglGetProcAddress("glCreateShader"));
	K15_CHECK_ASSIGNMENT(kglShaderSource, (PFNGLSHADERSOURCEPROC)kglGetProcAddress("glShaderSource"));				
	K15_CHECK_ASSIGNMENT(kglCompileShader, (PFNGLCOMPILESHADERPROC)kglGetProcAddress("glCompileShader"));
	K15_CHECK_ASSIGNMENT(kglCreateProgram, (PFNGLCREATEPROGRAMPROC)kglGetProcAddress("glCreateProgram"));
	K15_CHECK_ASSIGNMENT(kglGetShaderiv, (PFNGLGETSHADERIVPROC)kglGetProcAddress("glGetShaderiv"));
	K15_CHECK_ASSIGNMENT(kglAttachShader, (PFNGLATTACHSHADERPROC)kglGetProcAddress("glAttachShader"));
	K15_CHECK_ASSIGNMENT(kglLinkProgram, (PFNGLLINKPROGRAMPROC)kglGetProcAddress("glLinkProgram"));
	K15_CHECK_ASSIGNMENT(kglDetachShader, (PFNGLDETACHSHADERPROC)kglGetProcAddress("glDetachShader"));
	K15_CHECK_ASSIGNMENT(kglGetShaderInfoLog, (PFNGLGETSHADERINFOLOGPROC)kglGetProcAddress("glGetShaderInfoLog"));
	K15_CHECK_ASSIGNMENT(kglGenBuffers, (PFNGLGENBUFFERSPROC)kglGetProcAddress("glGenBuffers"));
	K15_CHECK_ASSIGNMENT(kglVertexAttribPointer, (PFNGLVERTEXATTRIBPOINTERPROC)kglGetProcAddress("glVertexAttribPointer"));
	K15_CHECK_ASSIGNMENT(kglEnableVertexAttribArray, (PFNGLENABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glEnableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglDisableVertexAttribArray, (PFNGLDISABLEVERTEXATTRIBARRAYPROC)kglGetProcAddress("glDisableVertexAttribArray"));
	K15_CHECK_ASSIGNMENT(kglDrawElementsBaseVertex, (PFNGLDRAWELEMENTSBASEVERTEXPROC)kglGetProcAddress("glDrawElementsBaseVertex"));
	K15_CHECK_ASSIGNMENT(kglBindBuffer, (PFNGLBINDBUFFERPROC)kglGetProcAddress("glBindBuffer"));
	K15_CHECK_ASSIGNMENT(kglBufferData, (PFNGLBUFFERDATAPROC)kglGetProcAddress("glBufferData"));
	K15_CHECK_ASSIGNMENT(kglBufferSubData, (PFNGLBUFFERSUBDATAPROC)kglGetProcAddress("glBufferSubData"));
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
	K15_CHECK_ASSIGNMENT(kglTexImage3D, (PFNGLTEXIMAGE3DPROC)kglGetProcAddress("glTexImage3D"));
	K15_CHECK_ASSIGNMENT(kglCompressedTexImage1D, (PFNGLCOMPRESSEDTEXIMAGE1DPROC)kglGetProcAddress("glCompressedTexImage1D"));
	K15_CHECK_ASSIGNMENT(kglCompressedTexImage2D, (PFNGLCOMPRESSEDTEXIMAGE2DPROC)kglGetProcAddress("glCompressedTexImage2D"));
	K15_CHECK_ASSIGNMENT(kglCompressedTexImage3D, (PFNGLCOMPRESSEDTEXIMAGE3DPROC)kglGetProcAddress("glCompressedTexImage3D"));
	K15_CHECK_ASSIGNMENT(kglGenerateMipmap, (PFNGLGENERATEMIPMAPPROC)kglGetProcAddress("glGenerateMipmap"));
	K15_CHECK_ASSIGNMENT(kglActiveTexture, (PFNGLACTIVETEXTUREPROC)kglGetProcAddress("glActiveTexture"));

	if(p_GLRenderContext->extensions.debug.supported)
	{
		K15_CHECK_ASSIGNMENT(kglDebugMessageCallback, 
			(PFNGLDEBUGMESSAGECALLBACKARBPROC)kglGetProcAddress(K15_CreateGLExtensionString("glDebugMessageCallback", p_GLRenderContext->extensions.debug.board, tempBuffer)));

		kglDebugMessageCallback(K15_DebugProcARB, 0);
	}

	if(p_GLRenderContext->extensions.vertex_array_object.supported)
	{
		K15_CHECK_ASSIGNMENT(kglGenVertexArrays, (PFNGLGENVERTEXARRAYSPROC)K15_GLGetExtensionFunction("glGenVertexArrays", p_GLRenderContext->extensions.vertex_array_object.board));
		K15_CHECK_ASSIGNMENT(kglBindVertexArray, (PFNGLBINDVERTEXARRAYPROC)K15_GLGetExtensionFunction("glBindVertexArray", p_GLRenderContext->extensions.vertex_array_object.board));
		K15_CHECK_ASSIGNMENT(kglDeleteVertexArrays, (PFNGLDELETEVERTEXARRAYSPROC)K15_GLGetExtensionFunction("glDeleteVertexArrays", p_GLRenderContext->extensions.vertex_array_object.board));
	}

	if(p_GLRenderContext->extensions.direct_state_access.supported)
	{
		K15_CHECK_ASSIGNMENT(kglTextureStorage1D, (PFNGLTEXTURESTORAGE1DPROC)K15_GLGetExtensionFunction("glTextureStorage1D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglTextureStorage2D, (PFNGLTEXTURESTORAGE2DPROC)K15_GLGetExtensionFunction("glTextureStorage2D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglTextureStorage3D, (PFNGLTEXTURESTORAGE3DPROC)K15_GLGetExtensionFunction("glTextureStorage3D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage1D, (PFNGLTEXTURESUBIMAGE1DPROC)K15_GLGetExtensionFunction("glTextureSubImage1D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage2D, (PFNGLTEXTURESUBIMAGE2DPROC)K15_GLGetExtensionFunction("glTextureSubImage2D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglTextureSubImage3D, (PFNGLTEXTURESUBIMAGE3DPROC)K15_GLGetExtensionFunction("glTextureSubImage3D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage1D, (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)K15_GLGetExtensionFunction("glCompressedTextureSubImage1D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage2D, (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)K15_GLGetExtensionFunction("glCompressedTextureSubImage2D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglCompressedTextureSubImage3D, (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)K15_GLGetExtensionFunction("glCompressedTextureSubImage3D", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglGenerateTextureMipmap, (PFNGLGENERATETEXTUREMIPMAPPROC)K15_GLGetExtensionFunction("glGenerateTextureMipmap", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglNamedBufferData, (PFNGLNAMEDBUFFERDATAPROC)K15_GLGetExtensionFunction("glNamedBufferData", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglNamedBufferSubData, (PFNGLNAMEDBUFFERSUBDATAPROC)K15_GLGetExtensionFunction("glNamedBufferSubData", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglMapNamedBuffer, (PFNGLMAPNAMEDBUFFERPROC)K15_GLGetExtensionFunction("glMapNamedBuffer", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglMapNamedBufferRange, (PFNGLMAPNAMEDBUFFERRANGEPROC)K15_GLGetExtensionFunction("glMapNamedBufferRange", p_GLRenderContext->extensions.direct_state_access.board));
		K15_CHECK_ASSIGNMENT(kglUnmapNamedBuffer, (PFNGLUNMAPNAMEDBUFFERPROC)K15_GLGetExtensionFunction("glUnmapNamedBuffer", p_GLRenderContext->extensions.direct_state_access.board));
	}

	if(p_GLRenderContext->extensions.sampler_objects.supported)
	{
		K15_CHECK_ASSIGNMENT(kglGenSamplers, (PFNGLGENSAMPLERSPROC)K15_GLGetExtensionFunction("glGenSamplers", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglDeleteSamplers, (PFNGLDELETESAMPLERSPROC)K15_GLGetExtensionFunction("glDeleteSamplers", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglBindSampler, (PFNGLBINDSAMPLERPROC)K15_GLGetExtensionFunction("glBindSampler", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameteri, (PFNGLSAMPLERPARAMETERIPROC)K15_GLGetExtensionFunction("glSamplerParameteri", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterf, (PFNGLSAMPLERPARAMETERFPROC)K15_GLGetExtensionFunction("glSamplerParameterf", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameteriv, (PFNGLSAMPLERPARAMETERIVPROC)K15_GLGetExtensionFunction("glSamplerParameteriv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterfv, (PFNGLSAMPLERPARAMETERFVPROC)K15_GLGetExtensionFunction("glSamplerParameterfv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterIiv, (PFNGLSAMPLERPARAMETERIIVPROC)K15_GLGetExtensionFunction("glSamplerParameterIiv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglSamplerParameterIuiv, (PFNGLSAMPLERPARAMETERIUIVPROC)K15_GLGetExtensionFunction("glSamplerParameterIuiv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterIiv, (PFNGLGETSAMPLERPARAMETERIIVPROC)K15_GLGetExtensionFunction("glGetSamplerParameterIiv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameteriv, (PFNGLGETSAMPLERPARAMETERIVPROC)K15_GLGetExtensionFunction("glGetSamplerParameteriv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterfv, (PFNGLGETSAMPLERPARAMETERFVPROC)K15_GLGetExtensionFunction("glGetSamplerParameterfv", p_GLRenderContext->extensions.sampler_objects.board));
		K15_CHECK_ASSIGNMENT(kglGetSamplerParameterIuiv, (PFNGLGETSAMPLERPARAMETERIUIVPROC)K15_GLGetExtensionFunction("glGetSamplerParameterIuiv", p_GLRenderContext->extensions.sampler_objects.board));
	}

	if(p_GLRenderContext->extensions.separate_shader_objects.supported)
	{
		K15_CHECK_ASSIGNMENT(kglGenProgramPipelines, (PFNGLGENPROGRAMPIPELINESPROC)K15_GLGetExtensionFunction("glGenProgramPipelines", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglBindProgramPipeline, (PFNGLBINDPROGRAMPIPELINEPROC)K15_GLGetExtensionFunction("glBindProgramPipeline", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglDeleteProgramPipelines, (PFNGLDELETEPROGRAMPIPELINESPROC)K15_GLGetExtensionFunction("glDeleteProgramPipelines", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglUseProgramStages, (PFNGLUSEPROGRAMSTAGESPROC)K15_GLGetExtensionFunction("glUseProgramStages", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglActiveShaderProgram, (PFNGLACTIVESHADERPROGRAMPROC)K15_GLGetExtensionFunction("glActiveShaderProgram", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglCreateShaderProgramv, (PFNGLCREATESHADERPROGRAMVPROC)K15_GLGetExtensionFunction("glCreateShaderProgramv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramParameteri, (PFNGLPROGRAMPARAMETERIPROC)K15_GLGetExtensionFunction("glProgramParameteri", p_GLRenderContext->extensions.separate_shader_objects.board));
	
		K15_CHECK_ASSIGNMENT(kglProgramUniform1i, (PFNGLPROGRAMUNIFORM1IPROC)K15_GLGetExtensionFunction("glProgramUniform1i", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2i, (PFNGLPROGRAMUNIFORM2IPROC)K15_GLGetExtensionFunction("glProgramUniform2i", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3i, (PFNGLPROGRAMUNIFORM3IPROC)K15_GLGetExtensionFunction("glProgramUniform3i", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4i, (PFNGLPROGRAMUNIFORM4IPROC)K15_GLGetExtensionFunction("glProgramUniform4i", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1ui, (PFNGLPROGRAMUNIFORM1UIPROC)K15_GLGetExtensionFunction("glProgramUniform1ui", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2ui, (PFNGLPROGRAMUNIFORM2UIPROC)K15_GLGetExtensionFunction("glProgramUniform2ui", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3ui, (PFNGLPROGRAMUNIFORM3UIPROC)K15_GLGetExtensionFunction("glProgramUniform3ui", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4ui, (PFNGLPROGRAMUNIFORM4UIPROC)K15_GLGetExtensionFunction("glProgramUniform4ui", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1f, (PFNGLPROGRAMUNIFORM1FPROC)K15_GLGetExtensionFunction("glProgramUniform1f", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2f, (PFNGLPROGRAMUNIFORM2FPROC)K15_GLGetExtensionFunction("glProgramUniform2f", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3f, (PFNGLPROGRAMUNIFORM3FPROC)K15_GLGetExtensionFunction("glProgramUniform3f", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4f, (PFNGLPROGRAMUNIFORM4FPROC)K15_GLGetExtensionFunction("glProgramUniform4f", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1d, (PFNGLPROGRAMUNIFORM1DPROC)K15_GLGetExtensionFunction("glProgramUniform1d", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2d, (PFNGLPROGRAMUNIFORM2DPROC)K15_GLGetExtensionFunction("glProgramUniform2d", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3d, (PFNGLPROGRAMUNIFORM3DPROC)K15_GLGetExtensionFunction("glProgramUniform3d", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4d, (PFNGLPROGRAMUNIFORM4DPROC)K15_GLGetExtensionFunction("glProgramUniform4d", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1iv, (PFNGLPROGRAMUNIFORM1IVPROC)K15_GLGetExtensionFunction("glProgramUniform1iv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2iv, (PFNGLPROGRAMUNIFORM2IVPROC)K15_GLGetExtensionFunction("glProgramUniform2iv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3iv, (PFNGLPROGRAMUNIFORM3IVPROC)K15_GLGetExtensionFunction("glProgramUniform3iv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4iv, (PFNGLPROGRAMUNIFORM4IVPROC)K15_GLGetExtensionFunction("glProgramUniform4iv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1uiv, (PFNGLPROGRAMUNIFORM1UIVPROC)K15_GLGetExtensionFunction("glProgramUniform1uiv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2uiv, (PFNGLPROGRAMUNIFORM2UIVPROC)K15_GLGetExtensionFunction("glProgramUniform2uiv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3uiv, (PFNGLPROGRAMUNIFORM3UIVPROC)K15_GLGetExtensionFunction("glProgramUniform3uiv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4uiv, (PFNGLPROGRAMUNIFORM4UIVPROC)K15_GLGetExtensionFunction("glProgramUniform4uiv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1fv, (PFNGLPROGRAMUNIFORM1FVPROC)K15_GLGetExtensionFunction("glProgramUniform1fv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2fv, (PFNGLPROGRAMUNIFORM2FVPROC)K15_GLGetExtensionFunction("glProgramUniform2fv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3fv, (PFNGLPROGRAMUNIFORM3FVPROC)K15_GLGetExtensionFunction("glProgramUniform3fv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4fv, (PFNGLPROGRAMUNIFORM4FVPROC)K15_GLGetExtensionFunction("glProgramUniform4fv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniform1dv, (PFNGLPROGRAMUNIFORM1DVPROC)K15_GLGetExtensionFunction("glProgramUniform1dv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform2dv, (PFNGLPROGRAMUNIFORM2DVPROC)K15_GLGetExtensionFunction("glProgramUniform2dv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform3dv, (PFNGLPROGRAMUNIFORM3DVPROC)K15_GLGetExtensionFunction("glProgramUniform3dv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniform4dv, (PFNGLPROGRAMUNIFORM4DVPROC)K15_GLGetExtensionFunction("glProgramUniform4dv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix2fv, (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix2fv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix3fv, (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix3fv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix4fv, (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix4fv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix2dv, (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix2dv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix3dv, (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix3dv", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglProgramUniformMatrix4dv, (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)K15_GLGetExtensionFunction("glProgramUniformMatrix4dv", p_GLRenderContext->extensions.separate_shader_objects.board));

		K15_CHECK_ASSIGNMENT(kglValidateProgramPipeline, (PFNGLVALIDATEPROGRAMPIPELINEPROC)K15_GLGetExtensionFunction("glValidateProgramPipeline", p_GLRenderContext->extensions.separate_shader_objects.board));
		K15_CHECK_ASSIGNMENT(kglGetProgramPipelineInfoLog, (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)K15_GLGetExtensionFunction("glGetProgramPipelineInfoLog", p_GLRenderContext->extensions.separate_shader_objects.board));
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
	GLint glMaxTextureUnits = 0;

	K15_OPENGL_CALL(kglGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glMaxAnistropy));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &glMaxColorAttachments));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_SAMPLES, &glMaxSamples));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize));
	K15_OPENGL_CALL(kglGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &glMaxTextureUnits));

	p_RenderFeatures->maxAnisotropy = (float)glMaxAnistropy;
	p_RenderFeatures->maxRenderTargets = (uint32)glMaxColorAttachments;
	p_RenderFeatures->maxSamples = (uint32)glMaxSamples;
	p_RenderFeatures->maxTextureDimension = (uint32)glMaxTextureSize;
	p_RenderFeatures->maxTextureUnits = (uint32)glMaxTextureUnits;
}
/*********************************************************************************/
intern void K15_InternalInitializeExtensions(K15_GLRenderContext* p_GLContext)
{
	if (p_GLContext->extensions.vertex_array_object.supported)
	{
		GLuint glVertexArrayObject = 0;
		K15_OPENGL_CALL(kglGenVertexArrays(1, &glVertexArrayObject));
		K15_OPENGL_CALL(kglBindVertexArray(glVertexArrayObject));
	}

	if (p_GLContext->extensions.separate_shader_objects.supported)
	{
		GLuint glProgramPipelineObject = 0;
		K15_OPENGL_CALL(kglGenProgramPipelines(1, &glProgramPipelineObject));
		K15_OPENGL_CALL(kglBindProgramPipeline(glProgramPipelineObject));
		p_GLContext->extensions.separate_shader_objects.programPipelineHandle = glProgramPipelineObject;
	}
}
/*********************************************************************************/
result8 K15_GLCreateRenderContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSLayerContext)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_GLRenderContext));

	if (!glContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	glContext->memoryAllocator = p_MemoryAllocator;
	glContext->platformContextData = 0;

	if (!K15_CreatePlatformSpecificContext)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	glContext->extensions.debug.supported = K15_FALSE;
	glContext->extensions.framebuffer_object.supported = K15_FALSE;
	glContext->extensions.sampler_objects.supported = K15_FALSE;
	glContext->extensions.direct_state_access.supported = K15_FALSE;
	glContext->extensions.separate_shader_objects.supported = K15_FALSE;
	glContext->extensions.vertex_array_object.supported = K15_FALSE;
	glContext->extensions.sparse_buffer.supported = K15_FALSE;
	glContext->extensions.sparse_texture.supported = K15_FALSE;

	glContext->extensions.compression.supportedCompressionMask = 0;
	glContext->extensions.separate_shader_objects.programPipelineHandle = 0;

	uint8 result = K15_CreatePlatformSpecificContext(p_MemoryAllocator, glContext, p_OSLayerContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//glContext->lastFreeGLObjectIndex = 0;

	//clear glObject array
	memset(glContext->glObjects, 0, sizeof(glContext->glObjects));

	//clear bound objects arrays
	memset(glContext->glBoundObjects.boundBuffers, 0, sizeof(glContext->glBoundObjects.boundBuffers));
	memset(glContext->glBoundObjects.boundPrograms, 0, sizeof(glContext->glBoundObjects.boundPrograms));

	memset(glContext->vertexCache.cacheEntries, 0, sizeof(glContext->vertexCache.cacheEntries));
	memset(glContext->pendingBufferUpdateManager.pendingUpdate, 0, sizeof(glContext->pendingBufferUpdateManager.pendingUpdate));

	glContext->glBoundObjects.boundMaterialPass = 0;
	glContext->glBoundObjects.boundVertexFormatHash = 0;

	glContext->pendingBufferUpdateManager.numPendingUpdates = 0;
	glContext->vertexCache.numEntries = 0;

// 	TODO
 	glContext->vendorString = (const char*)kglGetString(GL_VENDOR);
 	glContext->rendererString = (const char*)kglGetString(GL_RENDERER);

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

	K15_InternalGenerateGLSLVersionString(glContext);

	//1st get extension strings from API
	K15_GLGetExtensionStrings(glContext, p_MemoryAllocator);
	
	//2nd check if the extensions we *could* use are supported
	bool8 minSpecExtensionsSupported = K15_GLGetExtensions(glContext);

	//3rd check if the extensions we need at minimum are supported
	//TODO

	//4th load extensions
	result = K15_GLLoadExtensionFunctions(glContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	K15_InternalInitializeExtensions(glContext);

	//5th get interface function poiunter based on available extensions
	K15_InternalSetGLInterface(p_RenderBackEnd);

	kglClearColor(1.f, 1.f, 1.f, 1.f);

	//set pointer to gl context
	p_RenderBackEnd->specificRenderPlatform = (void*)glContext;

	//ask the renderer api what features it has
	K15_InternalGLGetRenderCapabilities(&p_RenderBackEnd->features);

	//initialize texture manager
	glContext->textureManager.numTextureSlots = 0;
	glContext->textureManager.maxTextureSlots = p_RenderBackEnd->features.maxTextureUnits;
	glContext->textureManager.textureSlots = (K15_GLTextureSlot*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_GLTextureSlot) * p_RenderBackEnd->features.maxTextureUnits);
	memset(glContext->textureManager.textureSlots, 0, sizeof(K15_GLTextureSlot) * p_RenderBackEnd->features.maxTextureUnits);

	//set viewport dimension
	K15_GLResizeViewport(p_RenderBackEnd, p_RenderBackEnd->viewportWidth, p_RenderBackEnd->viewportHeight);

	return K15_SUCCESS;
}
/*********************************************************************************/