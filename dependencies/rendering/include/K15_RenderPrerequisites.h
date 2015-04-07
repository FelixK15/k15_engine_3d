#ifndef K15_Render_Prerequisties_h_
#define K15_Render_Prerequisties_h_

#include <K15_OSLayer_Prerequisites.h>

#ifdef K15_OS_WINDOWS
	#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"	
	#define K15_DESKTOP_GL
#elif defined K15_OS_LINUX
	#define K15_DESKTOP_GL
#elif defined K15_OS_ANDROID || defined K15_OS_IOS
	#include "GL/EGL/K15_AndroidRenderEGLContext.h"
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

enum K15_RenderDescFlags
{
	K15_RENDER_DESC_NO_FLAGS		  = 0x000,
	K15_RENDER_DESC_AUTO_CLEANUP_FLAG = 0x001
};

typedef uint32 K15_RenderBufferHandle;
typedef uint32 K15_RenderProgramHandle;

struct K15_RenderContext;
struct K15_RenderCommandBuffer;
struct K15_RenderCommandInstance;
struct K15_RenderBufferDesc;
struct K15_RenderBufferUpdateDesc;
struct K15_RenderProgramDesc;
struct K15_RenderUniformUpdateDesc;
struct K15_RenderBlendStateDesc;
struct K15_RenderStencilStateDesc;
struct K15_RenderDepthStateDesc;
struct K15_RenderRasterizerStateDesc;

#endif //K15_Render_Prerequisties_h_