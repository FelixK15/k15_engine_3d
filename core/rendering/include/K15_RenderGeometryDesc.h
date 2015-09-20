#ifndef _K15_Rendering_RenderGeometryDesc_h_
#define _K15_Rendering_RenderGeometryDesc_h_

#include "K15_RenderPrerequisites.h"
#include "K15_Matrix4.h"

enum K15_RenderTopology
{
	K15_RENDER_TOPOLOGY_TRIANGLES = 0,
	K15_RENDER_TOPOLOGY_TRIANGLE_STRIP,
	K15_RENDER_TOPOLOGY_TRIANGLE_FAN,
	K15_RENDER_TOPOLOGY_POINTS,
	K15_RENDER_TOPOLOGY_LINES
};

struct K15_RenderGeometryDesc
{
	K15_RenderTopology topology;
	K15_RenderVertexData* vertexData;
	K15_Matrix4 worldMatrix;
};

#endif //_K15_Rendering_RenderGeometryDesc_h_