#ifndef _K15_Rendering_RenderMaterialDesc_h_
#define _K15_Rendering_RenderMaterialDesc_h_

#include "K15_RenderPrerequisites.h"
#include "generated/K15_RenderMaterialDataDescStretchBuffer.h"

struct K15_RenderMaterialDataDesc
{
	byte* data;
	//char* name;
	uint32 nameHash;
	uint32 dataSizeInBytes;
	uint32 typeID;
	uint32 semanticID;
	uint32 semanticGroupID;
};

struct K15_RenderMaterialPassDesc
{
	K15_RenderResourceHandle* vertexShaderHandle;
	K15_RenderResourceHandle* fragmentShaderHandle;
	K15_RenderMaterialDataDescStretchBuffer materialData;
};

struct K15_RenderMaterialDesc
{
	uint32 numMaterialPasses;
	K15_RenderMaterialPassDesc* materialPasses;
};

//bool8 K15_RenderMaterialDescStringCompare(K15_RenderMaterialDataDesc* p_Element, void* p_UserData);
bool8 K15_RenderMaterialDescHashCompare(K15_RenderMaterialDataDesc* p_Element, void* p_UserData);

void K15_SetRenderMaterialDataAsString(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, const char* p_Value);
void K15_SetRenderMaterialDataAsInt(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, int p_Value);
void K15_SetRenderMaterialDataAsFloat(K15_RenderMaterialPassDesc* p_MaterialPass, const char* p_DataValueName, float p_Value);

#endif //_K15_Rendering_RenderMaterialDesc_h_