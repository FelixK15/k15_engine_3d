#ifndef _K15_Rendering_RenderFontDesc_h_
#define _K15_Rendering_RenderFontDesc_h_

#include "K15_RenderPrerequisites.h"
#include "K15_Rectangle.h"

struct K15_RenderKerningDesc
{
	uint32 shiftedCharacters;
	int32 kerning;
};

struct K15_RenderGlyphDesc
{
	float x;
	float y;
	float width;
	float height;
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
	uint32 startCharacter;
	uint32 endCharacter;
	uint32 fontNameHash;
};

#endif //_K15_Rendering_RenderFontDesc_h_