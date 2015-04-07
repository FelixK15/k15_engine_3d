#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

#include <K15_RenderContext.h>
#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <K15_OSLayer_SystemEvents.h>
#include <K15_OSLayer_Thread.h>
#include <K15_OSLayer_System.h>
#include <K15_Logging.h>


#include <K15_RenderBufferDesc.h>
#include <K15_RenderProgramDesc.h>

#ifdef K15_OS_WINDOWS

#include <win32/K15_EnvironmentWin32.h>

void K15_InternalCreateTriangleBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferHandle* p_BufferHandle)
{
	float triangle[] = {
		1.f, -.5f, 0.f,
		-1.f, -.5f, 0.f,
		0.5f, .5f, 0.f
	};

	K15_RenderBufferDesc desc;
	desc.access = K15_RENDER_BUFFER_ACCESS_ALL;
	desc.data = (byte*)triangle;
	desc.size = sizeof(float) * 9;
	desc.type = K15_RENDER_BUFFER_TYPE_VERTEX;
	desc.usage = K15_RENDER_BUFFER_USAGE_STATIC_DRAW;
	desc.flags = K15_RENDER_DESC_AUTO_CLEANUP_FLAG;
	
	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_BUFFER);
	K15_AddRenderBufferHandleParameter(p_RenderCommandBuffer, p_BufferHandle);
	K15_AddRenderBufferDescParameter(p_RenderCommandBuffer, &desc);
	K15_EndRenderCommand(p_RenderCommandBuffer);
}

void K15_InternalCreateVertexShader(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramHandle* p_ProgramHandle)
{
	K15_RenderProgramDesc programDesc;
	programDesc.file = "../../../../test/res/default.vert";
	programDesc.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
	programDesc.source = K15_RENDER_PROGRAM_SOURCE_FILE;

	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_CREATE_PROGRAM);
	K15_AddRenderProgramHandleParameter(p_RenderCommandBuffer, p_ProgramHandle);
	K15_AddRenderProgramDescParameter(p_RenderCommandBuffer, &programDesc);
	K15_EndRenderCommand(p_RenderCommandBuffer);
}

void K15_InternalFillViewportUniform(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramHandle* p_ProgramHandle)
{
	K15_RenderUniformUpdateDesc uniformUpdateDesc;

	uniformUpdateDesc.name = "g_Viewport";
	uniformUpdateDesc.type = K15_UNIFORM_TYPE_FLOAT2;
	uniformUpdateDesc.data = (byte*)malloc(sizeof(float) * 2);
	uniformUpdateDesc.size = sizeof(float) * 2;
	uniformUpdateDesc.flags = K15_RENDER_DESC_AUTO_CLEANUP_FLAG;

	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	float width = (float)osContext->window.window->width;
	float height = (float)osContext->window.window->height;

	memcpy(uniformUpdateDesc.data, &width, sizeof(float));
	memcpy(uniformUpdateDesc.data + sizeof(float), &height, sizeof(float));

	K15_BeginRenderCommand(p_RenderCommandBuffer, K15_RENDER_COMMAND_UPDATE_UNIFORM);
	K15_AddRenderProgramHandleParameter(p_RenderCommandBuffer, p_ProgramHandle);
	K15_AddRenderUniformUpdateDescParameter(p_RenderCommandBuffer, &uniformUpdateDesc);
	K15_EndRenderCommand(p_RenderCommandBuffer);
}


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int returnValue = 0;

	uint8 result = K15_Win32InitializeOSLayer(hInstance);

	if (result == K15_ERROR_OUT_OF_MEMORY)
	{
		K15_LOG_ERROR_MESSAGE("Out of memory!");
	}
	else if(result == K15_ERROR_SYSTEM)
	{
		char* errorMessage = K15_Win32GetError();
		K15_LOG_ERROR_MESSAGE(errorMessage);
		free(errorMessage);
	}

	K15_Window* window = K15_CreateWindow(0, 0);
	K15_OSLayerContext* osContext = K15_GetOSLayerContext();

	K15_SetWindowDimension(window, 1024, 768);
	
	K15_RenderContext* renderContext = K15_CreateRenderContext(osContext);
	K15_RenderCommandBuffer* renderCommandBuffer = K15_CreateRenderCommandBuffer(renderContext);

	bool running = true;

	K15_SystemEvent event = {};


	//Test 1: Create Vertex Buffer
// 	K15_RenderBufferHandle triangleVertexBuffer = K15_INVALID_GPU_RESOURCE_HANDLE;
// 	K15_InternalCreateTriangleBuffer(renderCommandBuffer, &triangleVertexBuffer);

	// Test 2: Load Shader
	K15_RenderProgramHandle programHandle = K15_INVALID_GPU_RESOURCE_HANDLE;
	K15_InternalCreateVertexShader(renderCommandBuffer, &programHandle);

	// Test 3: Fill Uniforms
	K15_InternalFillViewportUniform(renderCommandBuffer, &programHandle);

	while (running)
	{
		K15_PumpSystemEvents();

		while (K15_GetSystemEventFromQueue(&event, K15_REMOVE_SYSTEM_EVENT_FLAG) != K15_SYSTEM_EVENT_QUEUE_EMPTY)
		{
			if (event.event == K15_APPLICATION_QUIT)
			{
				running = false;
			}
		}		
	
		K15_DispatchRenderCommandBuffer(renderCommandBuffer);

		K15_ProcessDispatchedRenderCommandBuffers(renderContext);

		K15_SleepThreadForSeconds(0.014f);
	}

	return 0;
}

#endif //K15_OS_WINDOWS