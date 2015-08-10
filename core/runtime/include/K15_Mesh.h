#ifndef _K15_Runtime_Mesh_h_
#define _K15_Runtime_Mesh_h_

#include "K15_RuntimePrerequisites.h"
#include "K15_RenderMeshDesc.h"

struct K15_Mesh
{
	K15_RenderMeshDesc renderMeshDesc;
//	K15_RenderMeshHandle renderMeshHandle;
	//K15_Transformation transformation;
	//material
};

K15_Mesh* K15_LoadMesh(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue, const char* p_ResourcePath);

#endif //_K15_Runtime_Mesh_h_