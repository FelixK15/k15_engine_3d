#ifndef _K15_Rendering_RenderSamplerDesc_h_
#define _K15_Rendering_RenderSamplerDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_RenderFilterMode : uint32
{
	K15_RENDER_FILTER_MODE_LINEAR = 0,
	K15_RENDER_FILTER_MODE_NEAREST,

	K15_RENDER_FILTER_MODE_NEAREST_MIPMAP,
	K15_RENDER_FILTER_MODE_LINEAR_MIPMAP,

	K15_RENDER_FILTER_MODE_ANISOTROPIC
};

enum K15_RenderFilterAddressMode: uint32
{
	K15_RENDER_FILTER_ADDRESS_MODE_REPEAT = 0,
	K15_RENDER_FILTER_ADDRESS_MODE_CLAMP,
	K15_RENDER_FILTER_ADDRESS_MODE_MIRROR
};

struct K15_RenderSamplerDesc
{
	struct  
	{
		K15_RenderFilterMode minification;
		K15_RenderFilterMode magnification;
	} filtering;

	struct  
	{
		K15_RenderFilterAddressMode u;
		K15_RenderFilterAddressMode v;
		K15_RenderFilterAddressMode w;
	} address;
};

#endif //_K15_Rendering_RenderSamplerDesc_h_