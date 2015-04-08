#ifndef _K15_Rendering_RenderFormats_h_
#define _K15_Rendering_RenderFormats_h_

enum K15_RenderFormat
{
	K15_RENDER_FORMAT_R8G8B8_UINT = 0,
	K15_RENDER_FORMAT_R16G16B16_UINT,
	K15_RENDER_FORMAT_R32G32B32_UINT,
	
	K15_RENDER_FORMAT_R8G8B8A8_UINT,
	K15_RENDER_FORMAT_R16G16B16A16_UINT,
	K15_RENDER_FORMAT_R32G32B32A32_UINT,

	K15_RENDER_FORMAT_R8G8B8_INT,
	K15_RENDER_FORMAT_R16G16B16_INT,
	K15_RENDER_FORMAT_R32G32B32_INT,

	K15_RENDER_FORMAT_R8G8B8A8_INT,
	K15_RENDER_FORMAT_R16G16B16A16_INT,
	K15_RENDER_FORMAT_R32G32B32A32_INT,

	K15_RENDER_FORMAT_R16G16B16_FLOAT,
	K15_RENDER_FORMAT_R32G32B32_FLOAT,

	K15_RENDER_FORMAT_R16G16B16A16_FLOAT,
	K15_RENDER_FORMAT_R32G32B32A32_FLOAT,

	K15_RENDER_FORMAT_RGB_DXT_1,
	K15_RENDER_FORMAT_RGBA_DXT_1,
	K15_RENDER_FORMAT_RGBA_DXT_3,
	K15_RENDER_FORMAT_RGBA_DXT_5
};

#endif //_K15_Rendering_RenderFormats_h_