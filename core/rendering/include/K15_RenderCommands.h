#ifndef _K15_Rendering_RenderCommands_h_
#define _K15_Rendering_RenderCommands_h_

#include "K15_RenderCommand.h"
#include "K15_RenderPrerequisites.h"
#include "K15_Rectangle.h"

void K15_RenderCommandDraw2DTexture(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_RenderMaterialDesc* p_RenderMaterialDesc, 
									K15_Rectangle p_DestinationRect, K15_Rectangle p_SourceRect);

void K15_RenderCommandCreateTextureFromTextureFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_TextureHandle, K15_TextureFormat* p_TextureFormat);
void K15_RenderCommandCreateMaterialFromMaterialFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderMaterialDesc* p_MaterialDesc, K15_MaterialFormat* p_MaterialFormat);
void K15_RenderCommandCreateSamplerFromSamplerFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_SamplerHandle , K15_SamplerFormat* p_SamplerFormat);
void K15_RenderCommandCreateFontFromFontFormat(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_FontHandle , K15_FontFormat* p_FontFormat);
void K15_RenderCommandCreateProgram(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderResourceHandle* p_ProgramHandle, K15_RenderProgramDesc* p_ProgramDesc);

const char* K15_ConvertRenderCommandToString(K15_RenderCommandType p_RenderCommandType);

#endif //_K15_Rendering_RenderCommands_h_