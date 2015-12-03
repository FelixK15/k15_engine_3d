#include "K15_RenderFontDesc.h"

/*********************************************************************************/
void K15_GetFontCharacterInfo(K15_RenderFontDesc* p_RenderFontDesc, const char* p_Text, uint32 p_TextLength, uint32 p_CharacterIndex, float* p_GlyphX, float* p_GlyphY, float* p_GlyphWidth, float* p_GlyphHeight, float* p_AdvanceX, float* p_AdvanceY, bool8* p_Renderable)
{
	K15_ASSERT(p_Text);

	unsigned char currentChar = p_Text[p_CharacterIndex];

	float glyphX = 0.f;
	float glyphY = 0.f;
	float glyphWidth = 0.f;
	float glyphHeight = 0.f;
	float advanceX = 0.f;
	float advanceY = 0.f;
	bool8 renderable = K15_TRUE;

	if (currentChar >= p_RenderFontDesc->startCharacter &&
		currentChar <= p_RenderFontDesc->endCharacter)
	{
		K15_RenderGlyphDesc* glyphDesc = &p_RenderFontDesc->glyphDescs[currentChar - p_RenderFontDesc->startCharacter];

		float kerningX = 0.f;

		glyphX = glyphDesc->x;
		glyphY = glyphDesc->y;
		glyphWidth = glyphDesc->width;
		glyphHeight = glyphDesc->height;

		//get kerning
		if (p_CharacterIndex+1 < p_TextLength)
		{
			const uint32 numCharacterInFont = p_RenderFontDesc->endCharacter - p_RenderFontDesc->startCharacter;
			const uint32 numKerningElements = numCharacterInFont * numCharacterInFont;
			unsigned int shiftedCharacter = (currentChar << 16) | p_Text[p_CharacterIndex+1];
			K15_RenderKerningDesc* kerningDesc = (K15_RenderKerningDesc*)bsearch(&shiftedCharacter, p_RenderFontDesc->kernDescs, numKerningElements, sizeof(K15_RenderKerningDesc), K15_InternalCompareKerning);

			if (kerningDesc)
			{
				kerningX = kerningDesc->kerning;
			}
		}

		if (currentChar == '\n')
		{
			advanceY -= p_RenderFontDesc->ascent - p_RenderFontDesc->descent + p_RenderFontDesc->lineGap;
		}
		else
		{
			advanceX += kerningX + glyphWidth/*- glyphDesc->glyphLeft*/;
			advanceY = glyphDesc->glyphTop;
		}

		if (isspace(currentChar))
		{
			renderable = K15_FALSE;
		}
	}

	if (p_AdvanceX)
	{
		*p_AdvanceX = advanceX;
	}

	if (p_AdvanceY)
	{
		*p_AdvanceY = advanceY;
	}

	if (p_GlyphHeight)
	{
		*p_GlyphHeight = glyphHeight;
	}

	if (p_GlyphWidth)
	{
		*p_GlyphWidth = glyphWidth;
	}

	if (p_GlyphX)
	{
		*p_GlyphX = glyphX;
	}

	if (p_GlyphY)
	{
		*p_GlyphY = glyphY;
	}

	if (p_Renderable)
	{
		*p_Renderable = renderable;
	}
}
/*********************************************************************************/
void K15_GetTextSizeInPixels(K15_RenderFontDesc* p_RenderFontDesc, float* p_OutWidth, float* p_OutHeight, const char* p_Text, unsigned int p_TextLength)
{
	float textWidth = 0.f;
	float textHeight = 0.f;

	if (p_TextLength == 0)
	{
		p_TextLength = (uint32)strlen(p_Text);
	
		if (p_TextLength == 0)
		{
			return;
		}
	}

	float baseHeight = 0.f;

	for (uint32 charIndex = 0;
		charIndex < p_TextLength;
		++charIndex)
	{
		float glyphWidth = 0.f;
		float advanceX = 0.f;
		float advanceY = 0.f;
		float glyphHeight = 0.f;
	
		K15_GetFontCharacterInfo(p_RenderFontDesc, p_Text, p_TextLength, charIndex, 0, 0, &glyphWidth, &glyphHeight, &advanceX, &advanceY, 0);

		baseHeight = K15_MAX(baseHeight, glyphHeight);

		textWidth += glyphWidth;
		textHeight += advanceY;
	}

	if (p_OutWidth)
	{
		*p_OutWidth = textWidth;
	}

	if (p_OutHeight)
	{
		*p_OutHeight = textHeight + baseHeight;
	}
}
/*********************************************************************************/
K15_RenderFontDesc* K15_CreateRenderFontDescFromFontFormat(K15_FontFormat* p_FontFormat, K15_RenderCommandQueue* p_RenderCommandQueue, K15_CustomMemoryAllocator* p_MemoryAllocator)
{
	K15_ASSERT(p_FontFormat);
	K15_ASSERT(p_RenderCommandQueue);
	K15_ASSERT(p_MemoryAllocator);

	K15_RenderFontDesc* renderFontDesc = (K15_RenderFontDesc*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_RenderFontDesc));
	renderFontDesc->textureHandle = (K15_RenderResourceHandle*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_RenderResourceHandle));

	uint32 fontTextureSize = K15_GetFontTextureSize(p_FontFormat);
	byte* fontTextureData = K15_GetFontTexture(p_FontFormat);

	K15_RenderTextureDesc fontTextureDesc = {};

	fontTextureDesc.createMipChain = K15_TRUE;
	fontTextureDesc.dimension.width = p_FontFormat->texture.width;
	fontTextureDesc.dimension.height = p_FontFormat->texture.height;
	fontTextureDesc.format = K15_RENDER_FORMAT_R8_UINT;
	fontTextureDesc.mipmaps.count = 1;
	fontTextureDesc.mipmaps.data[0] = fontTextureData;
	fontTextureDesc.mipmaps.dataSize[0] = fontTextureSize;
	fontTextureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;

	K15_RenderCommandCreateTexture(p_RenderCommandQueue, renderFontDesc->textureHandle, &fontTextureDesc);

	renderFontDesc->startCharacter = p_FontFormat->startCharacter;
	renderFontDesc->endCharacter = p_FontFormat->endCharacter;
	renderFontDesc->fontNameHash = p_FontFormat->fontNameHash;
	renderFontDesc->scaleFactor = p_FontFormat->scaleFactor;
	renderFontDesc->ascent = p_FontFormat->ascent;
	renderFontDesc->descent = p_FontFormat->descent;
	renderFontDesc->lineGap = p_FontFormat->lineGap;

	uint32 numGlyphs = p_FontFormat->endCharacter - p_FontFormat->startCharacter;
	uint32 numKerning = numGlyphs * numGlyphs;

	renderFontDesc->glyphDescs = (K15_RenderGlyphDesc*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_RenderGlyphDesc) * numGlyphs);
	renderFontDesc->kernDescs = (K15_RenderKerningDesc*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_RenderKerningDesc) * numKerning);
	renderFontDesc->textureHeight = p_FontFormat->texture.height;
	renderFontDesc->textureWidth = p_FontFormat->texture.width;

	//transfer glyph data
	for (uint32 glyphIndex = p_FontFormat->startCharacter;
		glyphIndex < p_FontFormat->endCharacter;
		++glyphIndex)
	{
		uint32 arrayGlyphIndex = glyphIndex - p_FontFormat->startCharacter;
		K15_GlyphFormat* glyphFormat = K15_GetFontGlyphData(p_FontFormat, glyphIndex);
		K15_RenderGlyphDesc* renderGlyphDesc = &renderFontDesc->glyphDescs[arrayGlyphIndex];

		float x = (float)glyphFormat->posX;
		float y = (float)glyphFormat->posY;
		float w = (float)glyphFormat->width;
		float h = (float)glyphFormat->height;
		float gl = (float)glyphFormat->glyphLeft;
		float gr = (float)glyphFormat->glyphRight;
		float gt = (float)glyphFormat->glyphTop;
		float gb = (float)glyphFormat->glyphBottom;

		renderGlyphDesc->character = glyphIndex;
		renderGlyphDesc->x = x;
		renderGlyphDesc->y = y;
		renderGlyphDesc->glyphLeft = gl;
		renderGlyphDesc->glyphRight = gr;
		renderGlyphDesc->glyphBottom = gb;
		renderGlyphDesc->glyphTop = gt;
		renderGlyphDesc->width = w;
		renderGlyphDesc->height = h;
		renderGlyphDesc->advance = glyphFormat->advance;
	}

	//transfer kerning data (literally the same structure)
	memcpy(renderFontDesc->kernDescs, p_FontFormat->kernFormats, sizeof(K15_RenderKerningDesc) * numKerning);

	return renderFontDesc;
}
/*********************************************************************************/
uint32 K15_GetTextVertexCount(K15_RenderFontDesc* p_RenderFontDesc, const char* p_Text, uint32 p_TextLength)
{
	uint32 numVertices = 0;
	bool8 renderable = K15_TRUE;
	for (uint32 textIndex = 0;
		textIndex < p_TextLength;
		++textIndex)
	{
		K15_GetFontCharacterInfo(p_RenderFontDesc, p_Text, p_TextLength, textIndex, 0, 0, 0, 0, 0, 0, &renderable);
	
		if (renderable)
		{
			numVertices += 6; //quad of two triangles
		}
	}

	return numVertices;
}
/*********************************************************************************/