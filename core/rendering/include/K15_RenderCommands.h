#ifndef _K15_Rendering_RenderCommands_h_
#define _K15_Rendering_RenderCommands_h_

#include "K15_RenderCommand.h"
#include "K15_RenderPrerequisites.h"
#include "K15_Rectangle.h"

void K15_RenderCommandDraw2DTexture(K15_RenderCommandQueue* p_RenderCommandQueue, 
									K15_RenderResourceHandle* p_TextureHandle,
									float p_PosX, float p_PosY,
									float p_Width, float p_Height);

void K15_RenderCommandDraw2DTextureEX(K15_RenderCommandQueue* p_RenderCommandQueue,
									  K15_RenderResourceHandle* p_TextureHandle,
									  float p_PosX, float p_PosY,
									  float p_Width, float p_Height,
									  float p_TexU, float p_TexV,
									  float p_TexWidth, float p_TexHeight);

void K15_RenderCommandDraw2DGUI(K15_RenderCommandQueue* p_RenderCommandQueue,
								K15_GUIContext* p_GUIContext);

void K15_RenderCommandDraw2DText(K15_RenderCommandQueue* p_RenderCommandQueue,
	K15_RenderFontDesc* p_FontDesc,
	const char* p_Text, K15_Vector4 p_Color,
	K15_Vector2 p_Position);

void K15_RenderCommandDrawClipped2DText(K15_RenderCommandQueue* p_RenderCommandQueue, 
										K15_RenderFontDesc* p_FontDesc, 
										const char* p_Text, K15_Vector4 p_Color,
										K15_Rectangle p_Rect);

void K15_RenderCommandCreateTextureFromTextureFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_TextureFormat* p_TextureFormat);
void K15_RenderCommandCreateSamplerFromSamplerFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_SamplerHandle , K15_SamplerFormat* p_SamplerFormat);
void K15_RenderCommandCreateTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_RenderTextureDesc* p_RenderTextureDesc);
void K15_RenderCommandCreateProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_ProgramHandle, K15_RenderProgramDesc* p_ProgramDesc);
void K15_RenderCommandWindowResized(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_Width, uint32 p_Height);

void K15_RenderCommandDeleteTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_TextureHandle);
void K15_RenderCommandDeleteSampler(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_SamplerHandle);
void K15_RenderCommandDeleteProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle p_ProgramHandle);

const char* K15_ConvertRenderCommandToString(K15_RenderCommandType p_RenderCommandType);

#endif //_K15_Rendering_RenderCommands_h_