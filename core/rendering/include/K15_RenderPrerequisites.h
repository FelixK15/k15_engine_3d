#ifndef K15_Render_Prerequisties_h_
#define K15_Render_Prerequisties_h_

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "K15_Common.h"
#include "K15_RendererConfig.h"
#include "K15_ErrorCodes.h"

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"	
	#define K15_DESKTOP_GL
#elif defined K15_OS_LINUX
	#define K15_DESKTOP_GL
#elif defined K15_OS_ANDROID || defined K15_OS_IOS
	#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
	#define K15_EMBEDDED_GL
#endif //K15_OS_WINDOWS

#ifdef K15_DESKTOP_GL
	#define K15_MIN_GL_VERSION_MAJOR 4
	#define K15_MIN_GL_VERSION_MINOR 5
#elif defined K15_EMBEDDED_GL
	#define K15_MIN_GL_VERSION_MAJOR 2
	#define K15_MIN_GL_VERSION_MINOR 0
#endif

#define K15_INVALID_GPU_RESOURCE_HANDLE 0xffffffff

#define K15_RENDER_MAX_GPU_BUFFER								1024
#define K15_RENDER_MAX_GPU_PROGRAMS								1024
#define K15_RENDER_MAX_GPU_TEXTURES								1024
#define K15_RENDER_MAX_GPU_MESHES								1024
#define K15_RENDER_MAX_GPU_MATERIALS							512
#define K15_RENDER_MAX_GPU_SAMPLERS								32
#define K15_RENDER_MAX_GPU_RENDER_TARGETS						32
#define K15_RENDER_MAX_GPU_FONTS								32
#define K15_RENDER_MAX_CAMERAS									32
#define K15_RENDER_MAX_UNIFORMS									64

#define K15_RENDER_IMMEDIATE_BUFFER_SIZE						size_kilobyte(512)

#define K15_IS_POW2(x) ((x != 0) && !(x & (x - 1))
#define K15_CHECK_ASSIGNMENT(variable, value) {variable = value; K15_ASSERT_TEXT(variable, "Could not assign to variable \'%s\'.", #variable);}

enum K15_RenderDescFlags
{
	K15_RENDER_DESC_NO_FLAGS		  = 0x000,
	K15_RENDER_DESC_AUTO_CLEANUP_FLAG = 0x001
};

typedef uint32 K15_RenderResourceHandle;

struct K15_CustomMemoryAllocator;
struct K15_OSContext;

struct K15_RenderProgramDesc;
struct K15_RenderBufferDesc;
struct K15_RenderTextureDesc;
struct K15_RenderSamplerDesc;
struct K15_RenderTargetDesc;
struct K15_RenderMaterialDesc;
struct K15_RenderBufferUpdateDesc;
struct K15_RenderCommandQueue;
struct K15_RenderUniformUpdateDesc;
struct K15_RenderGeometryDesc;
struct K15_RenderCommand;
struct K15_RenderBackEnd;
struct K15_RenderFrontEnd;
struct K15_RenderContext;
struct K15_RenderTextureUpdateDesc;
struct K15_RenderVertexFormatDesc;
struct K15_RenderVertexData;

struct K15_ShaderProcessorContext;

struct K15_Thread;


struct K15_TextureFormat;

#endif //K15_Render_Prerequisties_h_
