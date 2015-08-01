#ifndef _K15_Rendering_RenderFontDesc_h_
#define _K15_Rendering_RenderFontDesc_h_

#include "K15_RenderPrerequisites.h"

struct K15_RenderKerningDesc
{
	uint32 shiftedCharacters;
	real32 kerning;
};

struct K15_RenderGlyphDesc
{
	real32 texturePosX;
	real32 texturePosY;
	real32 glyphWidth;
	real32 glyphHeight;

	int character;
};

struct K15_RenderFontDesc
{
	real32 fontSize;
	K15_RenderTextureHandle fontTextureHandle;

	uint32 numGlyphs;

	K15_RenderGlyphDesc* renderGlyphDescs;
	K15_RenderKerningDesc* renderKerningDescs;
};

#endif //_K15_Rendering_RenderFontDesc_h_