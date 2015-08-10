#ifndef _K15_ResourceFormat_SamplerFormat_h_
#define _K15_ResourceFormat_SamplerFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

#define K15_FILTER_MODE_LINEAR 0
#define K15_FILTER_MODE_NEAREST 1

#define K15_ADDRESS_MODE_REPEAT 0
#define K15_ADDRESS_MODE_CLAMP 1
#define K15_ADDRESS_MODE_MIRROR 2

struct K15_SamplerFormat
{
	uint32 samplerNameHash;

	uint8 minificationFilter;
	uint8 magnificationFilter;

	uint8 addressModeU;
	uint8 addressModeV;
	uint8 addressModeW;
};

uint8 K15_SetSamplerFormatName(K15_SamplerFormat* p_SamplerFormat, const char* p_Name);

uint8 K15_SetSamplerFormatMinificationFilter(K15_SamplerFormat* p_SamplerFormat, uint8 p_MinificationFilter);
uint8 K15_SetSamplerFormatMagnificationFilter(K15_SamplerFormat* p_SamplerFormat, uint8 p_MagnificationFilter);

uint8 K15_SetSamplerFormatAdressModeU(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeU);
uint8 K15_SetSamplerFormatAdressModeV(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeV);
uint8 K15_SetSamplerFormatAdressModeW(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeW);

uint8 K15_SaveSamplerFormatToFile(K15_SamplerFormat* p_SamplerFormat, const char* p_Path, uint32 p_SaveFlags);
uint8 K15_LoadSamplerFormatFromFile(K15_SamplerFormat* p_SamplerFormat, const char* p_Path);
uint8 K15_LoadSamplerFormatFromMemory(K15_SamplerFormat* p_SamplerFormat, byte* p_Memory, uint32 p_MemorySize);

#endif //_K15_ResourceFormat_SamplerFormat_h_