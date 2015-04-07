#ifndef _K15_Rendering_RenderBufferDesc_h_
#define _K15_Rendering_RenderBufferDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_RenderBufferUsage
{
	K15_RENDER_BUFFER_USAGE_STATIC_DRAW = 0,
	K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW,
	K15_RENDER_BUFFER_USAGE_STREAM_DRAW
};

enum K15_RenderBufferType
{
	K15_RENDER_BUFFER_TYPE_VERTEX = 0,
	K15_RENDER_BUFFER_TYPE_INDEX,
	K15_RENDER_BUFFER_TYPE_UNIFORM,
	K15_RENDER_BUFFER_TYPE_TEXTURE
};

enum K15_RenderBufferAccess : uint32
{
	K15_RENDER_BUFFER_ACCESS_READ	=	0x01,
	K15_RENDER_BUFFER_ACCESS_WRITE	=	0x02,
	K15_RENDER_BUFFER_ACCESS_ALL	=	K15_RENDER_BUFFER_ACCESS_READ | K15_RENDER_BUFFER_ACCESS_WRITE
};

struct K15_RenderBufferDesc
{
	byte* data;
	void* userData;
	uint32 size;

	K15_RenderBufferUsage usage;
	K15_RenderBufferType type;
	K15_RenderBufferAccess access;

	uint32 flags;
};

struct K15_RenderBufferUpdateDesc
{
	byte* data;
	uint32 size;
	uint32 offset;
	uint32 flags;
};

#endif //_K15_Rendering_RenderBufferDesc_h_