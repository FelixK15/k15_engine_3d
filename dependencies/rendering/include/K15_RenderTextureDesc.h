#ifndef _K15_Rendering_RenderTextureDesc_h_
#define _K15_Rendering_RenderTextureDesc_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderFormats.h"

enum K15_RenderTextureType : uint32
{
	K15_RENDER_TEXTURE_TYPE_1D = 0,
	K15_RENDER_TEXTURE_TYPE_2D,
	K15_RENDER_TEXTURE_TYPE_3D,
	K15_RENDER_TEXTURE_TYPE_CUBE
};

struct K15_RenderTextureDesc
{
	K15_RenderTextureType type;
	K15_RenderFormat format;
	
	struct  
	{
		uint32 width;
		uint32 height;
		uint32 depth;
	} dimension;

	byte* data;
	uint32 flags;

	uint8 createMipChain;
};

struct K15_RenderTextureUpdateDesc
{
	byte* data;

	struct 
	{
		uint32 x;
		uint32 y;
		uint32 z;
	} offset;

	struct 
	{
		uint32 width;
		uint32 height;
		uint32 depth;
	} dimension;

	uint32 flags;
};

#endif //_K15_Rendering_RenderTextureDesc_h_
