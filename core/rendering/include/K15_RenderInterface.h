#ifndef _K15_Render_Interface_h_
#define _K15_Render_Interface_h_

#include "K15_RenderPrerequisites.h"
#include "K15_DefaultCLibraries.h"

K15_FUNCTION_VARIABLE(uint8, K15_ClearScreenCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd));

//buffer
K15_FUNCTION_VARIABLE(uint8, K15_CreateBufferCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderResourceHandle* p_RenderBufferHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_UpdateBufferCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferUpdateDesc* p_RenderBufferDesc, K15_RenderResourceHandle* p_RenderBufferHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_BindBufferCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderBufferHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_DeleteBufferCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderBufferHandlePtr));

//programs
K15_FUNCTION_VARIABLE(uint8, K15_CreateProgramCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderResourceHandle* p_RenderProgramHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_DeleteProgramCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderProgramHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_BindProgramCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderProgramHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_UpdateUniformCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, K15_RenderResourceHandle* p_RenderProgramHandlePtr));
K15_FUNCTION_VARIABLE(uint8, K15_UpdateDirtyUniformsCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd));

//states
K15_FUNCTION_VARIABLE(uint8, K15_SetDepthStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderDepthStateDesc* p_RenderDepthStateDesc));
K15_FUNCTION_VARIABLE(uint8, K15_SetStencilStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderStencilStateDesc* p_RenderStencilStateDesc));
K15_FUNCTION_VARIABLE(uint8, K15_SetRasterizerStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderRasterizerStateDesc* p_RenderRasterizerStateDesc));
K15_FUNCTION_VARIABLE(uint8, K15_SetBlendStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBlendStateDesc* p_RenderDepthStateDesc));

//sampler
K15_FUNCTION_VARIABLE(uint8, K15_CreateSamplerCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderSamplerDesc* p_RenderSamplerDesc, K15_RenderResourceHandle* p_RenderSamplerHandle));
K15_FUNCTION_VARIABLE(uint8, K15_DeleteSamplerCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderSamplerHandle));

//render target
K15_FUNCTION_VARIABLE(uint8, K15_CreateRenderTargetCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTargetDesc* p_RenderTargetDesc, K15_RenderResourceHandle* p_RenderTargetHandle));
K15_FUNCTION_VARIABLE(uint8, K15_BindRenderTargetCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderTargetHandle));
K15_FUNCTION_VARIABLE(uint8, K15_UnbindRenderTargetCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd));
K15_FUNCTION_VARIABLE(uint8, K15_DeleteRenderTargetCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderTargetHandle));

//textures
K15_FUNCTION_VARIABLE(uint8, K15_CreateTextureCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderResourceHandle* p_RenderTextureHandle));
K15_FUNCTION_VARIABLE(uint8, K15_UpdateTextureCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureUpdateDesc* p_RenderTextureDesc, K15_RenderResourceHandle* p_RenderTextureHandle));
K15_FUNCTION_VARIABLE(uint8, K15_DeleteTextureCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderTextureHandle));
K15_FUNCTION_VARIABLE(uint8, K15_BindTextureCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderTextureHandle));

//drawing
K15_FUNCTION_VARIABLE(uint8, K15_DrawFullscreenQuadCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderProgramHandle));
K15_FUNCTION_VARIABLE(uint8, K15_DrawIndexedCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, uint32 p_IndexCount, uint32 p_IndexOffset));

#endif //_K15_Render_Interface_h_


