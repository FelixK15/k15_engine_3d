#include "K15_RenderCommands.h"

#include "K15_Rectangle.h"

#include "K15_ErrorCodes.h"
#include "K15_Logging.h"

#include "K15_RenderMaterialDesc.h"

#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

/*********************************************************************************/
void K15_RenderCommandDraw2DTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_RenderMaterialDesc* p_RenderMaterialDesc, K15_Rectangle p_DestinationRect, K15_Rectangle p_SourceRect)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_TextureHandle, "Texture handle is NULL.");
	//K15_ASSERT_TEXT(*p_TextureHandle != K15_INVALID_GPU_RESOURCE_HANDLE, "Invalid texture handle.");
	K15_ASSERT_TEXT(p_RenderMaterialDesc, "Material Description is NULL.");
	K15_ASSERT_TEXT(K15_CalculateRectangleArea(p_SourceRect) > 0.f, "Source Rectangle area is 0.");
	K15_ASSERT_TEXT(K15_CalculateRectangleArea(p_DestinationRect) > 0.f, "Destination Rectangle area is 0.");

	result8 result = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_RENDER_2D_TEXTURE);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_RENDER_2D_TEXTURE), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_TextureHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_RenderMaterialDesc), p_RenderMaterialDesc));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_Rectangle), &p_DestinationRect));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_Rectangle), &p_SourceRect));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateTextureFromTextureFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_TextureFormat* p_TextureFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_TextureHandle, "Texture handle is NULL.");
	K15_ASSERT_TEXT(p_TextureFormat, "Texture format is NULL.");

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_TextureHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_TextureFormat), p_TextureFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateMaterialFromMaterialFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderMaterialDesc* p_MaterialDesc, K15_MaterialFormat* p_MaterialFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_MaterialDesc, "Material desc is NULL.");
	K15_ASSERT_TEXT(p_MaterialFormat, "Material format is NULL.");

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_MATERIAL_FROM_MATERIAL_FORMAT);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_CREATE_MATERIAL_FROM_MATERIAL_FORMAT), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_MaterialDesc));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_MaterialFormat), p_MaterialFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateSamplerFromSamplerFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_SamplerHandle , K15_SamplerFormat* p_SamplerFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_SamplerHandle, "sampler handle is NULL.");
	K15_ASSERT_TEXT(p_SamplerFormat, "Sampler format is NULL.");

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_SamplerHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_SamplerFormat), p_SamplerFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateFontFromFontFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_FontHandle , K15_FontFormat* p_FontFormat)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_FontHandle, "font handle is NULL.");
	K15_ASSERT_TEXT(p_FontFormat, "font format is NULL.");

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_FONT_FROM_FONT_FORMAT);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_CREATE_FONT_FROM_FONT_FORMAT), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, K15_PTR_SIZE, &p_FontHandle));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(K15_FontFormat), p_FontFormat));
	K15_CHECK_RESULT(K15_EndRenderCommand(p_RenderCommandQueue));
}
/*********************************************************************************/
void K15_RenderCommandCreateProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_ProgramHandle, K15_RenderProgramDesc* p_ProgramDesc)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_ProgramHandle, "Program handle is NULL.");
	K15_ASSERT_TEXT(p_ProgramDesc, "Program description is NULL.");

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_CREATE_PROGRAM);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_CREATE_PROGRAM), K15_GetErrorCodeString(result));
		return;
	}

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

	result8 result  = K15_BeginRenderCommand(p_RenderCommandQueue, K15_RENDER_COMMAND_WINDOW_RESIZED);

	if (result != K15_SUCCESS)
	{
		K15_LOG_WARNING_MESSAGE("Could not add render command '%s' (%s)", K15_ConvertRenderCommandToString(K15_RENDER_COMMAND_WINDOW_RESIZED), K15_GetErrorCodeString(result));
		return;
	}

	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(uint32), &p_Width));
	K15_CHECK_RESULT(K15_AddRenderCommandParameter(p_RenderCommandQueue, sizeof(uint32), &p_Height));
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

		case K15_RENDER_COMMAND_CREATE_MATERIAL_FROM_MATERIAL_FORMAT:
		{
			renderCommandString = "Create material from material format";
			break;
		}

		case K15_RENDER_COMMAND_CREATE_FONT_FROM_FONT_FORMAT:
		{
			renderCommandString = "Create font from font format";
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