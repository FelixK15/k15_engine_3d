#include "K15_SamplerFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"

#include "K15_String.h"

/*********************************************************************************/
intern uint8 K15_InternalSaveSamplerFormat(K15_DataAccessContext* p_DataAccessContext, K15_SamplerFormat* p_SamplerFormat, uint32 p_SaveFlags)
{
	assert(p_SamplerFormat);
	assert(p_DataAccessContext);

	K15_HeaderFormat headerFormat;
	K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_SAMPLER);

	//write header
	K15_WriteData(p_DataAccessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//write sampler name length
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_SamplerFormat->samplerNameHash);

	//write minification filter
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->minificationFilter);

	//write magnification filter
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->magnificationFilter);

	//write address move (u)
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeU);

	//write address move (v)
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeV);

	//write address move (w)
	K15_WriteData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeW);

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalLoadSamplerFormat(K15_DataAccessContext* p_DataAccessContext, K15_SamplerFormat* p_SamplerFormat)
{
	assert(p_SamplerFormat);
	assert(p_DataAccessContext);

	K15_HeaderFormat headerFormat;

	uint8 headerResult = K15_ReadHeaderFormat(p_DataAccessContext, &headerFormat, K15_RESOURCE_FORMAT_SAMPLER);
	if(headerResult != K15_SUCCESS)
	{
		return headerResult;
	}

	//read sampler name length
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &p_SamplerFormat->samplerNameHash);

	//read minification filter
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->minificationFilter);

	//read magnification filter
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->magnificationFilter);

	//read address move (u)
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeU);

	//read address move (v)
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeV);

	//read address move (w)
	K15_ReadData(p_DataAccessContext, sizeof(uint8), &p_SamplerFormat->addressModeW);

	return K15_SUCCESS;
}
/*********************************************************************************/




/*********************************************************************************/
uint8 K15_SetSamplerFormatName(K15_SamplerFormat* p_SamplerFormat, const char* p_Name)
{
	assert(p_SamplerFormat);
	assert(p_Name);

	p_SamplerFormat->samplerNameHash = K15_CreateHash(p_Name);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSamplerFormatMinificationFilter(K15_SamplerFormat* p_SamplerFormat, uint8 p_MinificationFilter)
{
	assert(p_SamplerFormat);
	assert(p_MinificationFilter >= 0 && p_MinificationFilter <= K15_FILTER_MODE_NEAREST);

	p_SamplerFormat->minificationFilter = p_MinificationFilter;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSamplerFormatMagnificationFilter(K15_SamplerFormat* p_SamplerFormat, uint8 p_MagnificationFilter)
{
	assert(p_SamplerFormat);
	assert(p_MagnificationFilter >= 0 && p_MagnificationFilter <= K15_FILTER_MODE_NEAREST);

	p_SamplerFormat->magnificationFilter = p_MagnificationFilter;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSamplerFormatAdressModeU(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeU)
{
	assert(p_SamplerFormat);
	assert(p_AddressModeU >= 0 && p_AddressModeU <= K15_ADDRESS_MODE_MIRROR);

	p_SamplerFormat->addressModeU = p_AddressModeU;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSamplerFormatAdressModeV(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeV)
{
	assert(p_SamplerFormat);
	assert(p_AddressModeV >= 0 && p_AddressModeV <= K15_ADDRESS_MODE_MIRROR);

	p_SamplerFormat->addressModeV = p_AddressModeV;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSamplerFormatAdressModeW(K15_SamplerFormat* p_SamplerFormat, uint8 p_AddressModeW)
{
	assert(p_SamplerFormat);
	assert(p_AddressModeW >= 0 && p_AddressModeW <= K15_ADDRESS_MODE_MIRROR);

	p_SamplerFormat->addressModeW = p_AddressModeW;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SaveSamplerFormatToFile(K15_SamplerFormat* p_SamplerFormat, const char* p_Path, uint32 p_SaveFlags)
{
	assert(p_SamplerFormat);
	assert(p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_SAVE);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalSaveSamplerFormat(&accessContext, p_SamplerFormat, p_SaveFlags);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadSamplerFormatFromFile(K15_SamplerFormat* p_SamplerFormat, const char* p_Path)
{
	assert(p_SamplerFormat);
	assert(p_Path);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromFile(&accessContext, p_Path, K15_DATA_ACCESS_USAGE_LOAD);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalLoadSamplerFormat(&accessContext, p_SamplerFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadSamplerFormatFromMemory(K15_SamplerFormat* p_SamplerFormat, byte* p_Memory, uint32 p_MemorySize)
{
	assert(p_SamplerFormat);
	assert(p_Memory);
	assert(p_MemorySize);

	K15_DataAccessContext accessContext;

	uint8 result = K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory, p_MemorySize, K15_DATA_ACCESS_USAGE_LOAD);

	if(result != K15_SUCCESS)
	{
		//error
	}

	result = K15_InternalLoadSamplerFormat(&accessContext, p_SamplerFormat);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/