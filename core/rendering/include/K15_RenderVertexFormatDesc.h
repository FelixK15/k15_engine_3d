#ifndef _K15_Rendering_RenderVertexFormat_h_
#define _K15_Rendering_RenderVertexFormat_h_

#include "K15_RenderPrerequisites.h"

#define K15_MAX_RENDER_VERTEX_FORMAT_ATTRIBUTES 10

struct K15_RenderVertexFormatElementDesc
{
	int typeID;
	int semanticID;
	uint32 attributeInstance;
	uint32 numValues; 
};

struct K15_RenderVertexFormatDesc
{
	uint32 hash;
	uint32 stride;
	uint32 numAttributes;
	K15_RenderVertexFormatElementDesc* elements;
};

K15_RenderVertexFormatDesc K15_CreateRenderVertexFormatDesc(K15_RenderContext* p_RenderContext, uint32 p_NumAttributes, ...);
void K15_FreeRenderVertexFormatDesc(K15_RenderContext* p_RenderContext, K15_RenderVertexFormatDesc* p_RenderFormatDesc);

#endif //_K15_Rendering_RenderVertexFormat_h_