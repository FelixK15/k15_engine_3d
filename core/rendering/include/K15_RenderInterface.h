#ifndef _K15_Render_Interface_h_
#define _K15_Render_Interface_h_

#include "K15_RenderPrerequisites.h"
#include "K15_DefaultCLibraries.h"


K15_FUNCTION_VARIABLE(result8, K15_ClearScreenCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd));
K15_FUNCTION_VARIABLE(result8, K15_SwapFrameBufferCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd));

//states
K15_FUNCTION_VARIABLE(result8, K15_SetDepthStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderDepthStateDesc* p_RenderDepthStateDesc));
K15_FUNCTION_VARIABLE(result8, K15_SetStencilStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderStencilStateDesc* p_RenderStencilStateDesc));
K15_FUNCTION_VARIABLE(result8, K15_SetRasterizerStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderRasterizerStateDesc* p_RenderRasterizerStateDesc));
K15_FUNCTION_VARIABLE(result8, K15_SetBlendStateCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBlendStateDesc* p_RenderDepthStateDesc));
K15_FUNCTION_VARIABLE(result8, K15_CreateMaterialFromMaterialFormatCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_MaterialFormat* p_MaterialFormat, K15_RenderResourceHandle* p_RenderResourceHandle));

//texture
K15_FUNCTION_VARIABLE(result8, K15_CreateTextureFromTextureDescCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderResourceHandle* p_RenderResourceHandle));
K15_FUNCTION_VARIABLE(result8, K15_CreateSamplerFromSamplerDescCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderSamplerDesc* p_RenderSamplerDesc, K15_RenderResourceHandle* p_RenderResourceHandle));

//vertex format
K15_FUNCTION_VARIABLE(result8, K15_CreateVertexFormatFromVertexFormatDescCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc, K15_RenderResourceHandle* p_RenderResourceHandle));

//program
K15_FUNCTION_VARIABLE(result8, K15_CreateProgramCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderResourceHandle* p_RenderResourceHandle));

//draw
K15_FUNCTION_VARIABLE(result8, K15_DrawGeometryCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderGeometryDesc* p_RenderGeometry));

//buffer
K15_FUNCTION_VARIABLE(K15_RenderVertexData*, K15_UpdateVertexDataCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, void* p_VertexData, uint32 p_NumVertices, K15_RenderVertexFormatDesc* p_VertexFormat));
K15_FUNCTION_VARIABLE(void, K15_FreeVertexDataCommandFnc, (K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexData* p_RenderVertexData));

#endif //_K15_Render_Interface_h_


