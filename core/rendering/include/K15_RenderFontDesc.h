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
	K15_Rectangle glyphTextureArea;
	uint16 character;
};

struct K15_RenderFontDesc
{
	K15_RenderKerningDesc* kernDescs;
	K15_RenderGlyphDesc* glyphDescs;

	K15_RenderResourceHandle* textureHandle;
	K15_RenderResourceHandle* bla;
	uint32 startCharacter;
	uint32 endCharacter;
	uint32 fontNameHash;
};

#endif //_K15_Rendering_RenderFontDesc_h_