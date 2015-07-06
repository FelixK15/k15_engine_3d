#ifndef _K15_Rendering_RenderFontDesc_h_
#define _K15_Rendering_RenderFontDesc_h_

#include "K15_RenderPrerequisites.h"

struct K15_RenderGlyphDesc
{
	real32 texturePosX;
	real32 texturePosY;

	int character;
};

struct K15_RenderFontDesc
{
	real32 fontSize;
	K15_RenderTextureHandle fontTextureHandle;
};

#endif //_K15_Rendering_RenderFontDesc_h_