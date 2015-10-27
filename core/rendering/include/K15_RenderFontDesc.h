#ifndef _K15_Rendering_RenderFontDesc_h_
#define _K15_Rendering_RenderFontDesc_h_

#include "K15_RenderPrerequisites.h"
#include "K15_Rectangle.h"

struct K15_RenderKerningDesc
{
	uint32 shiftedCharacters;
	float kerning;
};

struct K15_RenderGlyphDesc
{
	float x;
	float y;
	float width;
	float height;
  float advance;
	uint16 character;
};

struct K15_RenderFontDesc
{
	K15_RenderKerningDesc* kernDescs;
	K15_RenderGlyphDesc* glyphDescs;

	K15_RenderResourceHandle* textureHandle;
	K15_RenderResourceHandle* bla;
	float textureWidth;
	float textureHeight;
	float scaleFactor;
  float baseLine;
  float lineGap;
	uint32 startCharacter;
	uint32 endCharacter;
	uint32 fontNameHash;
};

#endif //_K15_Rendering_RenderFontDesc_h_