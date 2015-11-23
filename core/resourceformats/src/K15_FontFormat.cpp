#include "K15_FontFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"

/*********************************************************************************/
intern uint32 K15_InternalCalculateKerningBufferSize(K15_FontFormat* p_FontFormat)
{
	return K15_GetKerningDataCount(p_FontFormat) * sizeof(K15_KerningFormat);
}
/*********************************************************************************/
intern void K15_InternalInitializeKernFormatBuffer(K15_KerningFormat* p_KernFormatBuffer, uint16 p_StartCharacter, uint16 p_EndCharacter)
{
	uint32 numCharacter = p_EndCharacter - p_StartCharacter;
	uint32 kernFormatBufferIndex = 0;

	for (uint32 characterIndex = 0;
		characterIndex < numCharacter;
		++characterIndex)
	{
		for(uint32 characterIndex2 = 0;
			characterIndex2 < numCharacter;
			++characterIndex2)
		{
			uint32 charIndex = characterIndex + p_StartCharacter;
			uint32 charIndex2 = characterIndex2 + p_StartCharacter;

			K15_KerningFormat* currentKernFormat = &p_KernFormatBuffer[kernFormatBufferIndex++];

			currentKernFormat->shiftedCharacters = (charIndex << 16) | charIndex2;
			currentKernFormat->kerning = 0.f;
		}
	}
}
/*********************************************************************************/
intern uint8 K15_InternalSaveFontFormat(K15_DataAccessContext* p_DataAccessContext, K15_FontFormat* p_FontFormat, uint32 p_SaveFlags)
{
	assert(p_FontFormat);
	assert(p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_FONT);

	//write header
	K15_WriteData(p_DataAccessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//write font name hash
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->fontNameHash);

	//write font size
	K15_WriteData(p_DataAccessContext, sizeof(float), &p_FontFormat->fontSize);

	//write scale factor
	K15_WriteData(p_DataAccessContext, sizeof(float), &p_FontFormat->scaleFactor);

	//write ascent
	K15_WriteData(p_DataAccessContext, sizeof(float), &p_FontFormat->ascent);

	//write descent
	K15_WriteData(p_DataAccessContext, sizeof(float), &p_FontFormat->descent);

	//write line gap
	K15_WriteData(p_DataAccessContext, sizeof(float), &p_FontFormat->lineGap);

	//write font texture width
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.width);

	//write font texture height
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.height);

	//write font texture data size
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.dataSize);

	//write font texture data
	K15_WriteData(p_DataAccessContext, p_FontFormat->texture.dataSize, p_FontFormat->texture.data);

	//write starting character
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->startCharacter);

	//write ending character
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->endCharacter);

	uint32 numGlyphs = p_FontFormat->endCharacter - p_FontFormat->startCharacter;
	uint32 glyphBufferSize = numGlyphs * sizeof(K15_GlyphFormat);

	//write glyph buffer size
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &glyphBufferSize);

	//write glyph buffer
	K15_WriteData(p_DataAccessContext, glyphBufferSize, p_FontFormat->glyphFormats);

	uint32 kerningBufferSize = K15_InternalCalculateKerningBufferSize(p_FontFormat);

	//write kerning buffer
	K15_WriteData(p_DataAccessContext, kerningBufferSize, p_FontFormat->kernFormats);

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeFontFormat(*p_FontFormat);
	}

	return K15_SUCCESS;

}
/*********************************************************************************/
intern uint8 K15_InternalLoadFontFormat(K15_DataAccessContext* p_DataAccessContext, K15_FontFormat* p_FontFormat)
{
	assert(p_FontFormat);
	assert(p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	uint8 headerResult = K15_ReadHeaderFormat(p_DataAccessContext, &headerFormat, K15_RESOURCE_FORMAT_FONT);

	if(headerResult != K15_SUCCESS)
	{
		return headerResult;
	}

	//read font name hash
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->fontNameHash);

	//read font size
	K15_ReadData(p_DataAccessContext, sizeof(float), &p_FontFormat->fontSize);

	//read scale factor
	K15_ReadData(p_DataAccessContext, sizeof(float), &p_FontFormat->scaleFactor);

	//read ascent
	K15_ReadData(p_DataAccessContext, sizeof(float), &p_FontFormat->ascent);

	//read descent
	K15_ReadData(p_DataAccessContext, sizeof(float), &p_FontFormat->descent);

	//read line gap
	K15_ReadData(p_DataAccessContext, sizeof(float), &p_FontFormat->lineGap);

	//read font texture width
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.width);

	//read font texture height
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.height);

	//read font texture data size
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->texture.dataSize);

	p_FontFormat->texture.data = (byte*)K15_RF_MALLOC(p_FontFormat->texture.dataSize);

	//read font texture data
	K15_ReadData(p_DataAccessContext, p_FontFormat->texture.dataSize, p_FontFormat->texture.data);

	//read starting character
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->startCharacter);

	//read ending character
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_FontFormat->endCharacter);

	uint32 glyphBufferSize = 0;
	
	//read glyph buffer size
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &glyphBufferSize);

	byte* glyphBuffer = (byte*)K15_RF_MALLOC(glyphBufferSize);

	K15_ReadData(p_DataAccessContext, glyphBufferSize, glyphBuffer);

	uint32 kerningBufferSize = K15_InternalCalculateKerningBufferSize(p_FontFormat);

	byte* kerningBuffer = (byte*)K15_RF_MALLOC(kerningBufferSize);

	//read kerning buffer
	K15_ReadData(p_DataAccessContext, kerningBufferSize, kerningBuffer);

	p_FontFormat->glyphFormats = (K15_GlyphFormat*)glyphBuffer;
	p_FontFormat->kernFormats = (K15_KerningFormat*)kerningBuffer;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern K15_KerningFormat* K15_InternalSearchKerning(K15_KerningFormat* p_KerningFormatBuffer, uint32 p_KerningFormatBufferLength, uint32 p_ShiftedCharacters)
{
	K15_KerningFormat* kerningFormatBufferElement = 0;

	uint32 shiftedCharacters = 0;
	uint32 left = 0;
	uint32 index = 0;
	uint32 right = p_KerningFormatBufferLength;

	while (left <= right)
	{
		index = left + ((right - left) / 2);

		kerningFormatBufferElement = &p_KerningFormatBuffer[index];
		
		shiftedCharacters = kerningFormatBufferElement->shiftedCharacters;

		if (p_ShiftedCharacters == shiftedCharacters)
		{
			return kerningFormatBufferElement;
		}
		if (p_ShiftedCharacters < shiftedCharacters)
		{
			right = index - 1;
		}
		else
		{
			left = index + 1;
		}
	}

	return 0;
}
/*********************************************************************************/
uint32 K15_GetKerningDataCount(K15_FontFormat* p_FontFormat)
{
	uint32 numCharacters = p_FontFormat->endCharacter - p_FontFormat->startCharacter;
	return (numCharacters * numCharacters);
}
/*********************************************************************************/
uint8 K15_SetFontName(K15_FontFormat* p_FontFormat, char* p_FontName)
{
	assert(p_FontFormat);
	assert(p_FontName);

	/*uint32 nameLength = (uint32)strlen(p_FontName);
	char* nameBuffer = (char*)K15_RF_MALLOC(nameLength);

	if (!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memcpy(nameBuffer, p_FontName, nameLength);

	p_FontFormat->fontNameLength = nameLength;
	p_FontFormat->fontName = nameBuffer;*/

	p_FontFormat->fontNameHash = K15_CreateHash(p_FontName);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetFontGlyphRange(K15_FontFormat* p_FontFormat, uint32 p_StartCharacter, uint32 p_EndCharacter)
{
	assert(p_FontFormat);
	
	if (p_StartCharacter > p_EndCharacter)
	{
		return K15_ERROR_START_GREATER_THAN_END;
	}

	uint32 numCharacter = p_EndCharacter - p_StartCharacter;

	if (numCharacter == 0)
	{
		return K15_ERROR_START_EQUAL_END;
	}

	p_FontFormat->startCharacter = p_StartCharacter;
	p_FontFormat->endCharacter = p_EndCharacter;

	uint32 numCharacters = p_EndCharacter - p_StartCharacter;

	uint32 kernFormatBufferSizeInByte = K15_InternalCalculateKerningBufferSize(p_FontFormat);

	p_FontFormat->kernFormats = (K15_KerningFormat*)K15_RF_MALLOC(kernFormatBufferSizeInByte);
	p_FontFormat->glyphFormats = (K15_GlyphFormat*)K15_RF_MALLOC(numCharacter * sizeof(K15_GlyphFormat));

	if (!p_FontFormat->kernFormats)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	if (!p_FontFormat->glyphFormats)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	K15_InternalInitializeKernFormatBuffer(p_FontFormat->kernFormats, p_StartCharacter, p_EndCharacter);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetFontTexture(K15_FontFormat* p_FontFormat, byte* p_ImageData, uint32 p_Width, uint32 p_Height)
{
	assert(p_FontFormat);
	assert(p_ImageData);

	uint32 imageDataSize = p_Width * p_Height;
	byte* imageData = p_ImageData;

	p_FontFormat->texture.width = p_Width;
	p_FontFormat->texture.height = p_Height;
	p_FontFormat->texture.dataSize = imageDataSize;
	p_FontFormat->texture.data = (byte*)K15_RF_MALLOC(imageDataSize);

	memcpy(p_FontFormat->texture.data, imageData, imageDataSize);

	return K15_SUCCESS;
}
/*********************************************************************************/
byte* K15_GetFontTexture(K15_FontFormat* p_FontFormat)
{
	assert(p_FontFormat);

	uint32 imageDataSize = p_FontFormat->texture.dataSize;
	byte* imageData = p_FontFormat->texture.data;
	byte* returnTextureData = (byte*)K15_RF_MALLOC(imageDataSize);

	memcpy(returnTextureData, imageData, imageDataSize);
	return returnTextureData;
}
/*********************************************************************************/
uint32 K15_GetFontTextureSize(K15_FontFormat* p_FontFormat)
{
	assert(p_FontFormat);

	return p_FontFormat->texture.width * p_FontFormat->texture.height;
}
/*********************************************************************************/
uint8 K15_AddFontGlyphData(K15_FontFormat* p_FontFormat, K15_GlyphFormat* p_GlyphFormat, uint32 p_Character)
{
	assert(p_FontFormat);
	assert(p_GlyphFormat);

	uint32 startCharacter = p_FontFormat->startCharacter;
	uint32 endCharacter = p_FontFormat->endCharacter;

	if (p_Character < startCharacter || p_Character > endCharacter)
	{
		return K15_ERROR_CHARACTER_OUT_OF_BOUNDS;
	}

	uint32 characterIndex = p_Character - startCharacter;

	p_FontFormat->glyphFormats[characterIndex] = *p_GlyphFormat;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddFontKerningData(K15_FontFormat* p_FontFormat, uint16 p_Character1, uint16 p_Character2, float p_Kerning)
{
	assert(p_FontFormat);
	assert(p_Character1 >= p_FontFormat->startCharacter && p_Character1 <= p_FontFormat->endCharacter);
	assert(p_Character2 >= p_FontFormat->startCharacter && p_Character2 <= p_FontFormat->endCharacter);

	uint32 shiftedCharacters = (p_Character1 << 16) | p_Character2;
	uint32 kernFormatBufferLength = K15_GetKerningDataCount(p_FontFormat);

	K15_KerningFormat* kernFormat = K15_InternalSearchKerning(p_FontFormat->kernFormats, kernFormatBufferLength, shiftedCharacters);

	if (kernFormat)
	{
		kernFormat->kerning = p_Kerning;
		return K15_SUCCESS;
	}

	return K15_ERROR_CHARACTER_OUT_OF_BOUNDS;
}
/*********************************************************************************/
float K15_GetFontKerningData(K15_FontFormat* p_FontFormat, uint16 p_Character1, uint16 p_Character2)
{
	assert(p_FontFormat);
	assert(p_Character1 >= p_FontFormat->startCharacter && p_Character1 <= p_FontFormat->endCharacter);
	assert(p_Character2 >= p_FontFormat->startCharacter && p_Character2 <= p_FontFormat->endCharacter);

	uint32 shiftedCharacters = (p_Character1 << 16) | p_Character1;
	uint32 kernFormatBufferLength = K15_GetKerningDataCount(p_FontFormat);

	K15_KerningFormat* kernFormat = K15_InternalSearchKerning(p_FontFormat->kernFormats, kernFormatBufferLength, shiftedCharacters);

	if (kernFormat)
	{
		return kernFormat->kerning;
	}

	return 0;
}
/*********************************************************************************/
K15_GlyphFormat* K15_GetFontGlyphData(K15_FontFormat* p_FontFormat, uint32 p_Character)
{
	assert(p_FontFormat);

	uint32 startCharacter = p_FontFormat->startCharacter;
	uint32 endCharacter = p_FontFormat->endCharacter;

	if (p_Character < startCharacter || p_Character > endCharacter)
	{
		return 0;
	}

	uint32 characterIndex = p_Character - startCharacter;

	return &p_FontFormat->glyphFormats[characterIndex];;
}
/*********************************************************************************/
uint8 K15_SaveFontFormatToFile(K15_FontFormat* p_FontFormat, const char* p_Path, uint32 p_SaveFlags)
{
	assert(p_FontFormat);
	assert(p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_SAVE);

	if (result == K15_SUCCESS)
	{
		result = K15_InternalSaveFontFormat(&accessContext, p_FontFormat, p_SaveFlags);
		K15_CloseDataAccessContext(&accessContext);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_LoadFontFormatFromFile(K15_FontFormat* p_FontFormat, const char* p_Path)
{
	assert(p_FontFormat);
	assert(p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_LOAD);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalLoadFontFormat(&accessContext, p_FontFormat);

		K15_CloseDataAccessContext(&accessContext);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_LoadFontFormatFromMemory(K15_FontFormat* p_FontFormat, byte* p_Memory, uint32 p_MemorySize)
{
	assert(p_FontFormat);
	assert(p_Memory);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory, p_MemorySize, K15_DATA_ACCESS_USAGE_LOAD);

	if(result == K15_SUCCESS)
	{
		result = K15_InternalLoadFontFormat(&accessContext, p_FontFormat);
		K15_CloseDataAccessContext(&accessContext);
	}

	return result;
}
/*********************************************************************************/
void K15_FreeFontFormat(K15_FontFormat p_FontFormat)
{
	//K15_RF_FREE(p_FontFormat.fontName);
	K15_RF_FREE(p_FontFormat.texture.data);
	K15_RF_FREE(p_FontFormat.glyphFormats);
	K15_RF_FREE(p_FontFormat.kernFormats);
}
/*********************************************************************************/