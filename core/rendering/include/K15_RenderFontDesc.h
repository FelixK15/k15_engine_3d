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
	float glyphLeft;
	float glyphRight;
	float glyphTop;
	float glyphBottom;
	float advance;
	uint16 character;
};

struct K15_RenderFontDesc
{
	K15_RenderKerningDesc* kernDescs;
	K15_RenderGlyphDesc* glyphDescs;

	K15_RenderResourceHandle* textureHandle;
	float textureWidth;
	float textureHeight;
	float scaleFactor;
	float lineGap;
	float ascent;
	float descent;
	uint32 startCharacter;
	uint32 endCharacter;
	uint32 fontNameHash;
};

void K15_GetFontCharacterInfo(K15_RenderFontDesc* p_RenderFontDesc, const char* p_Text, uint32 p_TextLength, uint32 p_CharacterIndex, float* p_GlyphX, float* p_GlyphY, float* p_GlyphWidth, float* p_GlyphHeight, float* p_AdvanceX, float* p_AdvanceY, bool8* p_Renderable);
void K15_GetTextSizeInPixels(K15_RenderFontDesc* p_RenderFontDesc, float* p_OutWidth, float* p_OutHeight, const char* p_Text, unsigned int p_TextLength = 0);
K15_RenderFontDesc* K15_CreateRenderFontDescFromFontFormat(K15_FontFormat* p_FontFormat, K15_RenderCommandQueue* p_RenderCommandQueue, K15_CustomMemoryAllocator* p_MemoryAllocator);
uint32 K15_GetTextVertexCount(K15_RenderFontDesc* p_RenderFontDesc, const char* p_Text, uint32 p_TextLength);

#endif //_K15_Rendering_RenderFontDesc_h_