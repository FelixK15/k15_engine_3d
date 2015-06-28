#ifndef K15_Render_Prerequisties_h_
#define K15_Render_Prerequisties_h_

#define _CRT_SECURE_NO_WARNINGS

#include <K15_Prerequisites.h>
#include "K15_RendererConfig.h"

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
	#define K15_MIN_GL_VERSION_MAJOR 3
	#define K15_MIN_GL_VERSION_MINOR 3
#elif defined K15_EMBEDDED_GL
	#define K15_MIN_GL_VERSION_MAJOR 2
	#define K15_MIN_GL_VERSION_MINOR 0
#endif

#define K15_INVALID_GPU_RESOURCE_HANDLE 0xffffffff

#define K15_ERROR_MAX_RENDER_COMMAND_QUEUE_REACHED				210
#define K15_ERROR_MAX_RENDER_COMMANDS_REACHED					211
#define K15_ERROR_NO_RENDER_COMMAND								212
#define K15_ERROR_MAX_COMMAND_PARAMETER_SIZE_BUFFER_REACHED		213
#define K15_ERROR_RENDER_COMMAND_NOT_IMPLEMENTED				214
#define K15_ERROR_INVALID_PARAMETER_SIZE						215
#define K15_ERROR_INVALID_BUFFER_TYPE							216
#define K15_ERROR_LAST_COMMAND_NOT_FINISHED						217
#define K15_ERROR_RENDER_BUFFER_LOCKED							218
#define K15_ERROR_RENDER_API									219
#define K15_ERROR_RENDER_PROGRAM_COMPILATION					220
#define K15_ERROR_RENDER_UNIFORM_NOT_FOUND						221

#define K15_RENDER_MAX_GPU_BUFFER								4096
#define K15_RENDER_MAX_GPU_PROGRAMS								4096
#define K15_RENDER_MAX_GPU_TEXTURES								4096
#define K15_RENDER_MAX_GPU_MESHES								4096
#define K15_RENDER_MAX_GPU_SAMPLERS								32
#define K15_RENDER_MAX_GPU_RENDER_TARGETS						32
#define K15_RENDER_MAX_CAMERAS									32
#define K15_RENDER_MAX_UNIFORMS									64

#define K15_IS_POW2(x) ((x != 0) && !(x & (x - 1))
#define K15_CHECK_ASSIGNMENT(variable, value) {variable = value; K15_ASSERT_TEXT(variable, "Could not assign to variable \'%s\'.", #variable);}

enum K15_RenderDescFlags
{
	K15_RENDER_DESC_NO_FLAGS		  = 0x000,
	K15_RENDER_DESC_AUTO_CLEANUP_FLAG = 0x001
};

typedef uint32 K15_RenderBufferHandle;
typedef uint32 K15_RenderTextureHandle;
typedef uint32 K15_RenderProgramHandle;
typedef uint32 K15_RenderSamplerHandle;
typedef uint32 K15_RenderTargetHandle;
typedef uint32 K15_RenderMeshHandle;
typedef uint32 K15_RenderCameraHandle;

enum K15_UniformType : uint32;
enum K15_RenderCommand : uint32;

struct K15_InternalRenderCameraDesc;
struct K15_RenderContext;
struct K15_RenderCommandBuffer;
struct K15_RenderCommandInstance;
struct K15_RenderBufferDesc;
struct K15_RenderBufferUpdateDesc;
struct K15_RenderProgramDesc;
struct K15_RenderTextureDesc;
struct K15_RenderCameraDesc;
struct K15_RenderCameraUpdateDesc;
struct K15_RenderTextureUpdateDesc;
struct K15_RenderUniformUpdateDesc;
struct K15_RenderBlendStateDesc;
struct K15_RenderStencilStateDesc;
struct K15_RenderDepthStateDesc;
struct K15_RenderViewportDesc;
struct K15_RenderRasterizerStateDesc;
struct K15_RenderSamplerDesc;
struct K15_RenderTargetDesc;
struct K15_RenderMeshDesc;
struct K15_MeshFormat;
struct K15_Matrix4;

#endif //K15_Render_Prerequisties_h_
