#ifndef _K15_Rendering_RenderProgramDesc_h_
#define _K15_Rendering_RenderProgramDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_RenderProgramType : uint32
{
	K15_RENDER_PROGRAM_TYPE_VERTEX = 0,
	K15_RENDER_PROGRAM_TYPE_FRAGMENT,
	K15_RENDER_PROGRAM_TYPE_GEOMETRY,

	K15_RENDER_PROGRAM_TYPE_COUNT
};

enum K15_RenderProgramSource : uint32
{
	K15_RENDER_PROGRAM_SOURCE_FILE = 0,
	K15_RENDER_PROGRAM_SOURCE_CODE
};

struct K15_RenderProgramUniform
{
	char* name;
	uint32 semanticID;
	uint32 semanticGroupID;
	uint32 typeID;
};

struct K15_RenderUniformUpdateDesc
{
	uint32 nameHash;
	uint32 typeID;
	uint32 sizeInBytes;
	uint32 flags;

	union 
	{
		byte* rawData;
		uint32 textureSlot;
	} data;
};

struct K15_RenderProgramDesc
{
	const char* file;
	char* code;
	K15_RenderProgramType type;
	K15_RenderProgramSource source;

	uint32 nameHash;
	uint32 numUniforms;
	K15_RenderProgramUniform uniforms[K15_RENDER_MAX_UNIFORMS];
};

//uint32 K15_ConvertToUniformByteSize(K15_UniformType p_UniformType);

const char* K15_GetMainFunctionIdentifierForProgramType(K15_RenderProgramType p_ProgramType);

#endif //_K15_Rendering_RenderProgramDesc_h_