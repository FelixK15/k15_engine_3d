#include "K15_RenderCommands.h"

#include "K15_Rectangle.h"

#include "K15_ErrorCodes.h"
#include "K15_Logging.h"

#include "K15_RenderMaterialDesc.h"

#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

/*********************************************************************************/
intern result8 K15_InternalBeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommandType p_RenderCommand)
{
	result8 result = K15_BeginRenderCommand(p_RenderCommandQueue, p_RenderCommand);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(p_RenderCommand), K15_GetErrorCodeString(result));
	}

	return result;
}
/*********************************************************************************/
void K15_RenderCommandDraw2DTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_Rectangle p_DestinationRect, K15_Rectangle p_SourceRect)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_TextureHandle, "Texture handle is NULL.");
	//K15_ASSERT_TEXT(*p_TextureHandle != K15_INVALID_GPU_RESOURCE_HANDLE, "Invalid texture handle.");
	K15_ASSERT_TEXT(K15_CalculateRectangleArea(p_SourceRect) > 0.f, "Source Rectangle area is 0.");
	K15_ASSERT_TEXT(K15_CalculateRectangleArea(p_DestinationRect) > 0.f, "Destination Rectangle area is 0.");

	result8 result = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_RENDER_2D_TEXTURE);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_TextureHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_Rectangle), &p_DestinationRect));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_Rectangle), &p_SourceRect));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandDraw2DText(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderFontDesc* p_FontDesc, const char* p_Text, K15_Vector2 p_Position)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_FontDesc, "Font desc is NULL.");
	K15_ASSERT_TEXT(p_Text, "Text is NULL.");

	result8 result = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_RENDER_2D_TEXT);

	uint32 textLength = (uint32)strlen(p_Text);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderFontDesc), p_FontDesc));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(uint32), &textLength));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, textLength, (char*)p_Text));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_Vector2), &p_Position));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateTextureFromTextureFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_TextureFormat* p_TextureFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_TextureHandle, "Texture handle is NULL.");
	K15_ASSERT_TEXT(p_TextureFormat, "Texture format is NULL.");

	*p_TextureHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	result8 result  = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_TextureHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_TextureFormat), p_TextureFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateSamplerFromSamplerFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_SamplerHandle , K15_SamplerFormat* p_SamplerFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_SamplerHandle, "sampler handle is NULL.");
	K15_ASSERT_TEXT(p_SamplerFormat, "Sampler format is NULL.");

	*p_SamplerHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	result8 result  = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_SamplerHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_SamplerFormat), p_SamplerFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_TextureHandle, "texture handle is NULL.");
	K15_ASSERT_TEXT(p_RenderTextureDesc, "Texture desc is NULL.");

	*p_TextureHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	result8 result  = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_TEXTURE);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_TextureHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderTextureDesc), p_RenderTextureDesc));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_ProgramHandle, K15_RenderProgramDesc* p_ProgramDesc)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_ProgramHandle, "Program handle is NULL.");
	K15_ASSERT_TEXT(p_ProgramDesc, "Program description is NULL.");

	*p_ProgramHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	result8 result  = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_PROGRAM);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_ProgramHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderProgramDesc), p_ProgramDesc));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandWindowResized(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_Width, uint32 p_Height)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_Width > 0, "Width is 0");
	K15_ASSERT_TEXT(p_Height > 0, "Height is 0");

	result8 result  = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_WINDOW_RESIZED);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(uint32), &p_Width));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(uint32), &p_Height));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandDeleteTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_TextureHandle)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	
	if (p_TextureHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return;
	}
	
	K15_ASSERT_TEXT(p_TextureHandle != K15_INVALID_GPU_RESOURCE_HANDLE, "Texture handle is invalid");

	result8 result = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_DELETE_TEXTURE);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderResourceHandle), &p_TextureHandle));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandDeleteSampler(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_SamplerHandle)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_SamplerHandle != K15_INVALID_GPU_RESOURCE_HANDLE, "Sampler handle is invalid");

	result8 result = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_DELETE_SAMPLER);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderResourceHandle), &p_SamplerHandle));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandDeleteProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_ProgramHandle)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_ProgramHandle != K15_INVALID_GPU_RESOURCE_HANDLE, "Program handle is invalid");

	result8 result = K15_InternalBeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_DELETE_PROGRAM);

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderResourceHandle), &p_ProgramHandle));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
const char* K15_ConvertRenderCommandToString(K15_RenderCommandType p_RenderCommandType)
{
	const char* renderCommandString = 0;

	switch(p_RenderCommandType)
	{
		case K15_RENDER_COMMAND_RENDER_2D_TEXTURE:
		{
			renderCommandString = "Render 2D Texture";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT:
		{
			renderCommandString = "Create texture from texture format";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT:
		{
			renderCommandString = "Create sampler from sampler format";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_PROGRAM:
		{
			renderCommandString = "Create Program";
			break;
		}

		case K15_RENDER_COMMAND_WINDOW_RESIZED:
		{
			renderCommandString = "Window Resized";
			break;
		}

		default:
		{
			renderCommandString = "Unsupported Command";
			break;
		}
	}

	return renderCommandString;
}
/*********************************************************************************/