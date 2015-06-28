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

enum K15_UniformType : uint32
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

enum K15_AutoUniformType : uint32
{
	K15_AUTO_UNIFORM_TYPE_UNKNOWN = 0,
	K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX,
	K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX,
	K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX,

	K15_AUTO_UNIFORM_TYPE_COUNT
};

enum K15_AutoUniformTypeGroupFlag : uint32
{
	K15_AUTO_UNIFORM_TYPE_GROUP_PER_MESH		= 0x01,
	K15_AUTO_UNIFORM_TYPE_GROUP_PER_MATERIAL	= 0x02,
	K15_AUTO_UNIFORM_TYPE_GROUP_PER_CAMERA		= 0x04,

	K15_AUTO_UNIFORM_TYPE_GROUP_ALL = K15_AUTO_UNIFORM_TYPE_GROUP_PER_MESH | K15_AUTO_UNIFORM_TYPE_GROUP_PER_MATERIAL | K15_AUTO_UNIFORM_TYPE_GROUP_PER_CAMERA
};

struct K15_RenderProgramUniform
{
	char* name;
	K15_UniformType type;
	K15_AutoUniformType autoType;
	K15_AutoUniformTypeGroupFlag autoTypeGroup;
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

struct K15_RenderProgramDesc
{
	const char* file;
	char* code;
	K15_RenderProgramType type;
	K15_RenderProgramSource source;

	uint32 numUniforms;
	K15_RenderProgramUniform uniforms[K15_RENDER_MAX_UNIFORMS];
};

K15_AutoUniformType K15_ConvertToAutoType(char* p_UniformName);
K15_AutoUniformTypeGroupFlag K15_ConvertToAutoTypeGroup(K15_AutoUniformType p_UniformAutoType);
uint32 K15_ConvertToUniformByteSize(K15_UniformType p_UniformType);
#endif //_K15_Rendering_RenderProgramDesc_h_