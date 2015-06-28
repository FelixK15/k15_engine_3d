#ifndef _K15_Rendering_RenderMesh_h_
#define _K15_Rendering_RenderMesh_h_

#include "K15_RenderPrerequisites.h"

struct K15_RenderMeshDesc
{
	uint32 submeshCount;
	K15_RenderBufferHandle* vertexBufferHandles;
	K15_RenderBufferHandle* indexBufferHandles;
};

#endif //_K15_Rendering_RenderMesh_h_