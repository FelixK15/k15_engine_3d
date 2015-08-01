#ifndef _K15_Rendering_RenderMaterialDesc_h_
#define _K15_Rendering_RenderMaterialDesc_h_

#include "K15_RenderPrerequisites.h"

struct K15_RenderMaterialPassDesc
{
	uint32 numSamplerHandles;
	K15_RenderSamplerHandle* samplerHandles;
	K15_RenderProgramHandle vertexShaderHandle;
	K15_RenderProgramHandle fragmentShaderHandle;
};

struct K15_RenderMaterialDesc
{
	uint32 numMaterialPasses;
	K15_RenderMaterialPassDesc* materialPasses;
};

#endif //_K15_Rendering_RenderMaterialDesc_h_