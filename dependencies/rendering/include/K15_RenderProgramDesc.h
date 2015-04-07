#ifndef _K15_Rendering_RenderProgramDesc_h_
#define _K15_Rendering_RenderProgramDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_RenderProgramType
{
	K15_RENDER_PROGRAM_TYPE_VERTEX = 0,
	K15_RENDER_PROGRAM_TYPE_FRAGMENT,
	K15_RENDER_PROGRAM_TYPE_GEOMETRY
};

enum K15_RenderProgramSource
{
	K15_RENDER_PROGRAM_SOURCE_FILE = 0,
	K15_RENDER_PROGRAM_SOURCE_CODE
};

enum K15_UniformType
{
	K15_UNIFORM_TYPE_INVALID = 0,

	K15_UNIFORM_TYPE_UINT32,
	K15_UNIFORM_TYPE_UINT16,
	K15_UNIFORM_TYPE_UINT8,
	K15_UNIFORM_TYPE_INT32,
	K15_UNIFORM_TYPE_INT16,
	K15_UNIFORM_TYPE_INT8,

	K15_UNIFORM_TYPE_FLOAT,
	
	K15_UNIFORM_TYPE_FLOAT2,
	K15_UNIFORM_TYPE_FLOAT3,
	K15_UNIFORM_TYPE_FLOAT4,

	K15_UNIFORM_TYPE_FLOAT2x2,
	K15_UNIFORM_TYPE_FLOAT3x3,
	K15_UNIFORM_TYPE_FLOAT4x4
};

struct K15_RenderProgramDesc
{
	const char* file;
	const char* code;
	K15_RenderProgramType type;
	K15_RenderProgramSource source;
};

struct K15_RenderUniformUpdateDesc
{
	const char* name;
	K15_UniformType type;
	byte* data;
	uint32 size;
	uint32 flags;

	//auto free data?
};

#endif //_K15_Rendering_RenderProgramDesc_h_