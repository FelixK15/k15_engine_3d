#include "K15_Mesh.h"
#include "K15_ResourceContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_MeshFormat.h"
#include "K15_RenderContext.h"

#include "K15_RenderCommands.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderMeshDesc.h"

#include "K15_Logging.h"

#include "K15_DefaultCLibraries.h"

/*********************************************************************************/
K15_Mesh* K15_LoadMesh(K15_ResourceContext* p_ResourceContext, K15_RenderCommandBuffer* p_RenderCommandBuffer, const char* p_ResourcePath)
{
	//TODO: Instead of several submeshes, load one big mesh
// 	K15_Resource* resource = K15_LoadResource(p_ResourceContext, p_ResourcePath, 0);
// 	K15_CustomMemoryAllocator* memoryAllocator = &p_ResourceContext->memoryAllocator;
// 
	K15_Mesh* mesh = 0;
// 	K15_MeshFormat meshFormat = {};
// 
// 	if (!resource)
// 	{
// 		return NULL;
// 	}
// 
// 	uint8 result = K15_LoadMeshFormatFromMemory(&meshFormat, resource->data, resource->dataSizeInBytes);
// 
// 	if (result != K15_SUCCESS)
// 	{
// 		K15_LOG_ERROR_MESSAGE("The format of '%s' is not compatible with the .k15mesh format.");
// 		return NULL;
// 	}
// 
// 	mesh = (K15_Mesh*)K15_AllocateFromMemoryAllocator(memoryAllocator, sizeof(K15_Mesh));

	//TODO
	/*K15_RenderMeshHandle meshHandle = K15_INVALID_GPU_RESOURCE_HANDLE;
	if (K15_RenderCommandCreateMesh(p_RenderCommandBuffer, &meshFormat, &mesh->renderMeshHandle) != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Error creating render commands for mesh '%s'.", p_ResourcePath);
	}*/

	return mesh;
}
/*********************************************************************************/