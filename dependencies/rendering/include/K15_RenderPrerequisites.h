#ifndef K15_Render_Prerequisties_h_
#define K15_Render_Prerequisties_h_

#include <K15_OSLayer_Prerequisites.h>

#ifdef K15_OS_WINDOWS
	#include "GL/WGL/K15_Win32RenderWGLContext.h"	
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

#define K15_INVALID_GPU_HANDLE 0xffffffff

#define K15_ERROR_MAX_RENDER_COMMAND_QUEUE_REACHED				210
#define K15_ERROR_MAX_RENDER_COMMANDS_REACHED					211
#define K15_ERROR_NO_RENDER_COMMAND								212
#define K15_ERROR_MAX_COMMAND_PARAMETER_SIZE_BUFFER_REACHED		213
#define K15_ERROR_RENDER_COMMAND_NOT_IMPLEMENTED				214
#define K15_ERROR_INVALID_PARAMETER_SIZE						215
#define K15_ERROR_INVALID_BUFFER_TYPE							216
#define K15_ERROR_LAST_COMMAND_NOT_FINISHED						217
#define K15_ERROR_RENDER_BUFFER_LOCKED							218

#define K15_CHECK_RENDER_CONTEXT_INITIALIZED(context) \
	while((context->flags & K15_RCF_INITIALIZED) == 0)

#define K15_CHECK_RENDER_BACK_BUFFER_UNLOCKED(renderQueue) \
	while((renderQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->flags & K15_CBF_LOCKED) == 0)

typedef uint32 K15_GpuBufferHandle;

struct K15_RenderContext;
struct K15_RenderCommandQueue;
struct K15_RenderCommandInstance;

#endif //K15_Render_Prerequisties_h_