#ifndef _K15_Rendering_RenderMesh_h_
#define _K15_Rendering_RenderMesh_h_

#include "K15_RenderPrerequisites.h"

struct K15_RenderMeshDesc
{
	uint32 submeshCount;
	K15_RenderResourceHandle* vertexBufferHandles;
	K15_RenderResourceHandle* indexBufferHandles;
};

#endif //_K15_Rendering_RenderMesh_h_