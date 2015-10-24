#ifndef _K15_ResourceFormat_FontFormat_h_
#define _K15_ResourceFormat_FontFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_KerningFormat
{
	uint32 shiftedCharacters;
	int32 kerning;
};

struct K15_GlyphFormat
{
	uint32 posX;
	uint32 posY;
	uint32 width;
	uint32 height;
	uint32 xOffset;
	uint32 yOffset;

	uint16 character;
};

struct K15_FontFormat
{
	K15_KerningFormat* kernFormats;
	K15_GlyphFormat* glyphFormats;
	//char* fontName;

	struct  
	{
		uint32 width;
		uint32 height;
		uint32 dataSize;
		byte* data;
	} texture;

	float fontSize;

	uint32 fontNameHash;
	uint32 startCharacter;
	uint32 endCharacter;
};

uint8 K15_SetFontName(K15_FontFormat* p_FontFormat, char* p_FontName);
uint8 K15_SetFontGlyphRange(K15_FontFormat* p_FontFormat, uint32 p_StartCharacter, uint32 p_EndCharacter);

uint8 K15_SetFontTexture(K15_FontFormat* p_FontFormat, byte* p_ImageData, uint32 p_Width, uint32 p_Height);
byte* K15_GetFontTexture(K15_FontFormat* p_FontFormat);

uint32 K15_GetFontTextureSize(K15_FontFormat* p_FontFormat);

uint8 K15_AddFontGlyphData(K15_FontFormat* p_FontFormat, K15_GlyphFormat* p_GlyphFormat, uint32 p_Character);
uint8 K15_AddFontKerningData(K15_FontFormat* p_FontFormat, uint16 p_Character1, uint16 p_Character2, int32 p_Kerning);
uint32 K15_GetKerningDataCount(K15_FontFormat* p_FontFormat);
int32 K15_GetFontKerningData(K15_FontFormat* p_FontFormat, uint16 p_Character1, uint16 p_Characeter2);

K15_GlyphFormat* K15_GetFontGlyphData(K15_FontFormat* p_FontFormat, uint32 p_Character);

uint8 K15_SaveFontFormatToFile(K15_FontFormat* p_FontFormat, const char* p_Path, uint32 p_SaveFlags);

uint8 K15_LoadFontFormatFromFile(K15_FontFormat* p_FontFormat, const char* p_Path);
uint8 K15_LoadFontFormatFromMemory(K15_FontFormat* p_FontFormat, byte* p_Memory, uint32 p_MemorySize);

void K15_FreeFontFormat(K15_FontFormat p_FontFormat);
#endif //_K15_ResourceFormat_FontFormat_h_