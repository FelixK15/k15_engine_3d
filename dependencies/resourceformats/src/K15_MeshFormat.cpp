#include "K15_MeshFormat.h"
#include "K15_HeaderFormat.h"
#include "K15_DataAccessHelper.h"

#include "enums/K15_ReturnValues.h"
#include "enums/K15_FormatValues.h"

#define K15_RESOURCE_DESC_NUM_POSITION_VALUES	3
#define K15_RESOURCE_DESC_NUM_NORMAL_VALUES		3
#define K15_RESOURCE_DESC_NUM_TANGENT_VALUES	3
#define K15_RESOURCE_DESC_NUM_BITANENT_VALUES	3
#define K15_RESOURCE_DESC_NUM_COLOR_VALUES		4
#define K15_RESOURCE_DESC_NUM_TEXCOORD_VALUES	2


#define K15_RESOURCE_DESC_SIZE_POSITION		sizeof(float) * K15_RESOURCE_DESC_NUM_POSITION_VALUES
#define K15_RESOURCE_DESC_SIZE_NORMAL		sizeof(float) * K15_RESOURCE_DESC_NUM_NORMAL_VALUES
#define K15_RESOURCE_DESC_SIZE_TANGENT 		sizeof(float) * K15_RESOURCE_DESC_NUM_TANGENT_VALUES
#define K15_RESOURCE_DESC_SIZE_BITANGENT	sizeof(float) * K15_RESOURCE_DESC_NUM_BITANENT_VALUES
#define K15_RESOURCE_DESC_SIZE_COLOR		sizeof(float) * K15_RESOURCE_DESC_NUM_COLOR_VALUES
#define K15_RESOURCE_DESC_SIZE_TEXCOORD		sizeof(float) * K15_RESOURCE_DESC_NUM_TEXCOORD_VALUES
#define K15_RESOURCE_DESC_TRIANGLE_INDICES	sizeof(uint16) * 3

/*********************************************************************************/
uint8 K15_SetMeshSubmeshCount(K15_MeshFormat* p_MeshFormat, uint32 p_SubmeshCount)
{
	assert(p_SubmeshCount && p_MeshFormat);

	K15_SubMeshFormat* submeshes = (K15_SubMeshFormat*)malloc(sizeof(K15_SubMeshFormat) * p_SubmeshCount);

	if(!submeshes)
	{
		// log error
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memset(submeshes, 0, sizeof(K15_SubMeshFormat) * p_SubmeshCount);

	p_MeshFormat->submeshCount = p_SubmeshCount;
	p_MeshFormat->submeshes = submeshes;

	// initialize data
	for(uint32 submeshIndex = 0;
		submeshIndex < p_SubmeshCount;
		++submeshIndex)
	{
		memset(&submeshes[submeshIndex].helper, 0, sizeof(K15_SubmeshFormatHelper));

		submeshes[submeshIndex].colorChannelCount = 0;
		submeshes[submeshIndex].materialNameLength = 0;
		submeshes[submeshIndex].materialName = 0;
		submeshes[submeshIndex].textureChannelCount = 0;
		submeshes[submeshIndex].triangleCount = 0;
		submeshes[submeshIndex].triangleData = 0;
		submeshes[submeshIndex].vertexCount = 0;
		submeshes[submeshIndex].vertexData = 0;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetMeshName(K15_MeshFormat* p_MeshFormat, const char* p_Name)
{
	assert(p_MeshFormat && p_Name);

	
	uint32 nameLength = strlen(p_Name);
	char* nameBuffer = (char*)malloc(nameLength);

	if(!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	memcpy(nameBuffer, p_Name, nameLength);

	p_MeshFormat->meshNameLength = nameLength;
	p_MeshFormat->meshName = nameBuffer;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshVertexCount(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_VertexCount)
{
	assert(p_SubmeshFormat && p_VertexCount);

	p_VertexCount -= p_VertexCount % 3;

	// fill data
	p_SubmeshFormat->vertexCount = p_VertexCount;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshTriangleCount(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_TriangleCount)
{
	assert(p_SubmeshFormat && p_TriangleCount);

	K15_TriangleFormat* triangleData = (K15_TriangleFormat*)malloc(sizeof(K15_TriangleFormat) * p_TriangleCount);

	if(!triangleData)
	{
		// log error
		return K15_ERROR_OUT_OF_MEMORY;
	}

	p_SubmeshFormat->triangleCount = p_TriangleCount;
	p_SubmeshFormat->triangleData = triangleData;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshTriangleData(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_FaceIndex, uint32* p_Indices)
{
	assert(p_Indices && p_SubmeshFormat && p_FaceIndex <= p_SubmeshFormat->triangleCount);

	p_SubmeshFormat->triangleData[p_FaceIndex].indices[0] = (uint16)p_Indices[0];
	p_SubmeshFormat->triangleData[p_FaceIndex].indices[1] = (uint16)p_Indices[1];
	p_SubmeshFormat->triangleData[p_FaceIndex].indices[2] = (uint16)p_Indices[2];

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshData(K15_SubMeshFormat* p_SubmeshFormat, float* p_Data, uint8 p_DataIdentifier)
{
	assert(p_Data && p_SubmeshFormat && p_SubmeshFormat->vertexCount && p_DataIdentifier <= K15_MESH_DATA_TEXCOORD);

	uint32 vertexCount = p_SubmeshFormat->vertexCount;
	uint8 componentCount = 0;

	float** memorySlot = 0;

	// get amount of float component current data 
	// consists per vertex
	switch(p_DataIdentifier)
	{
	case K15_MESH_DATA_POSITION:
		{
			memorySlot = &p_SubmeshFormat->helper.positions;
			componentCount = 3;
			break;
		}

	case K15_MESH_DATA_NORMAL:
		{
			memorySlot = &p_SubmeshFormat->helper.normals;
			componentCount = 3;
			break;
		}

	case K15_MESH_DATA_TANGENT:
		{
			memorySlot = &p_SubmeshFormat->helper.tangents;
			componentCount = 3;
			break;
		}
	case K15_MESH_DATA_BITANGENT:
		{
			memorySlot = &p_SubmeshFormat->helper.bitangents;
			componentCount = 3;
			break;
		}
	default:
		{
			// log error

			return K15_ERROR_WRONG_DATA_IDENTIFIER;
		}
	}

	// calculate data size for current submesh data
	uint32 dataSize = componentCount * sizeof(float) * vertexCount;

	// allocate memory
	byte* memory = (byte*)malloc(dataSize);

	if(!memory)
	{
		// log error
		return K15_ERROR_OUT_OF_MEMORY;
	}

	// copy vertex data into memory of the helper struct
	memcpy(memory, p_Data, dataSize);
	*memorySlot = (float*)memory;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshDataWithIndex(uint32 p_Index, K15_SubMeshFormat* p_SubmeshFormat, float* p_Data, uint8 p_DataIdentifier)
{
	assert(p_Data && 
		p_SubmeshFormat && 
		p_SubmeshFormat->vertexCount && 
		p_DataIdentifier <= K15_MESH_DATA_TEXCOORD);

	uint32 vertexCount = p_SubmeshFormat->vertexCount;
	uint8 componentCount = 0;

	float** memorySlot = 0;

	switch(p_DataIdentifier)
	{
	case K15_MESH_DATA_TEXCOORD:
		{
			memorySlot = &p_SubmeshFormat->helper.texcoordChannels[p_Index];
			componentCount = K15_RESOURCE_DESC_NUM_TEXCOORD_VALUES;
			break;
		}

	case K15_MESH_DATA_COLOR:
		{
			memorySlot = &p_SubmeshFormat->helper.colorChannels[p_Index];
			componentCount = K15_RESOURCE_DESC_NUM_COLOR_VALUES;
			break;
		}

	default:
		{
			//log error
			return K15_ERROR_WRONG_DATA_IDENTIFIER;
		}
	}

	// calculate data size for current submesh data
	uint32 dataSize = componentCount * sizeof(float) * vertexCount;

	// copy vertex data into memory of the helper struct
	memcpy(*memorySlot, p_Data, dataSize);

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshMaterial(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_MaterialIndex)
{
	assert(p_SubmeshFormat);

	p_SubmeshFormat->materialIndex = p_MaterialIndex;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SetSubmeshChannelCount(K15_SubMeshFormat* p_SubmeshFormat, uint8 p_ChannelCount, uint8 p_DataIdentifier)
{
	assert(p_SubmeshFormat &&
		p_DataIdentifier);

	if(p_ChannelCount == 0)
	{
		return K15_SUCCESS;
	}

	float** memory = 0;
	float*** memorySlot = 0;
	uint32 componentCount = 0;
	uint32 vertexCount = p_SubmeshFormat->vertexCount;

	switch(p_DataIdentifier)
	{
	case K15_MESH_DATA_COLOR:
		{
			memorySlot = &p_SubmeshFormat->helper.colorChannels;
			componentCount = K15_RESOURCE_DESC_NUM_COLOR_VALUES;
			p_SubmeshFormat->colorChannelCount = p_ChannelCount;
			//memory = (float**)malloc(p_ChannelCount * K15_RESOURCE_DESC_NUM_COLOR_VALUES);
			break;
		}

	case K15_MESH_DATA_TEXCOORD:
		{
			memorySlot = &p_SubmeshFormat->helper.texcoordChannels;
			componentCount = K15_RESOURCE_DESC_NUM_TEXCOORD_VALUES;
			p_SubmeshFormat->textureChannelCount = p_ChannelCount;
			//memory = (float**)malloc(p_ChannelCount * K15_RESOURCE_DESC_NUM_TEXCOORD_VALUES);
			break;
		}

	default:
		{
			//log error
			return K15_ERROR_WRONG_DATA_IDENTIFIER;
		}
	}

	memory = (float**)malloc(p_ChannelCount * sizeof(float*));

	if(!memory)
	{
		// log error
		return K15_ERROR_OUT_OF_MEMORY;
	}

	for(uint32 channelIndex = 0;
		channelIndex < p_ChannelCount;
		++channelIndex)
	{
		memory[channelIndex] = (float*)malloc(sizeof(float) * vertexCount * componentCount);
	}

	(*memorySlot) = memory;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_SaveMeshFormat(K15_MeshFormat* p_MeshFormat, const char* p_FileName, uint32 p_SaveFlags)
{
	assert(p_MeshFormat &&
		p_FileName);

	K15_DataAccessContext accessContext;

	K15_CreateDataAccessContextFromFile(&accessContext, p_FileName);

	uint8 result = K15_InternalSaveMeshFormat(p_MeshFormat, &accessContext, p_SaveFlags);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadMeshFormat(K15_MeshFormat* p_MeshFormat, const char* p_FileName)
{
	assert(p_MeshFormat && p_FileName);

	K15_DataAccessContext accessContext;

	K15_CreateDataAccessContextFromFile(&accessContext, p_FileName);

	uint8 result = K15_InternalLoadMeshFormat(p_MeshFormat, &accessContext);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
uint8 K15_LoadMeshFormatFromMemory(K15_MeshFormat* p_MeshFormat, byte* p_Memory)
{
	assert(p_MeshFormat && p_Memory);

	K15_DataAccessContext accessContext;

	K15_CreateDataAccessContextFromMemory(&accessContext, p_Memory);

	uint8 result = K15_InternalLoadMeshFormat(p_MeshFormat, &accessContext);

	K15_CloseDataAccessContext(&accessContext);

	return result;
}
/*********************************************************************************/
void K15_FreeMeshFormat(K15_MeshFormat p_MeshFormat)
{
	for(uint32 submeshIndex = 0;
		submeshIndex < p_MeshFormat.submeshCount;
		++submeshIndex)
	{
		K15_SubMeshFormat submeshFormat = p_MeshFormat.submeshes[submeshIndex];

		//free(submeshFormat.vertexData);
		free(submeshFormat.triangleData);
		submeshFormat.triangleData = 0;
	}

	free(p_MeshFormat.meshName);
	free(p_MeshFormat.submeshes);
	p_MeshFormat.submeshes = 0;
}
/*********************************************************************************/
uint8 K15_InternalLoadMeshFormat(K15_MeshFormat* p_MeshFormat, K15_DataAccessContext* p_DataAccessContext)
{
	assert(p_MeshFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;

	// read header and check resource file type
	K15_InternalReadHeaderFormat(p_DataAccessContext, &headerFormat, K15_RESOURCE_FORMAT_MESH);

	if(headerFormat.resourceType != K15_RESOURCE_FORMAT_MESH)
	{
		return K15_ERROR_WRONG_FILE_FORMAT;
	}

	K15_MeshFormat meshFormat;

	//skip header, as already read before to determine resource file type
	//K15_SkipData(p_DataAccessContext, sizeof(K15_HeaderFormat));

	uint32 nameBufferLength = 0;
	char* nameBuffer = 0;

	//mesh name
	K15_ReadData(p_DataAccessContext, sizeof(uint32), &nameBufferLength);

	nameBuffer = (char*)malloc(nameBufferLength + 1); //+1 for null terminator

	if(!nameBuffer)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	K15_ReadData(p_DataAccessContext, nameBufferLength, nameBuffer);

	nameBuffer[nameBufferLength] = 0;

	meshFormat.meshName = nameBuffer;
	meshFormat.meshNameLength = nameBufferLength;

	//submesh count
	K15_ReadData(p_DataAccessContext, sizeof(uint16), &meshFormat.submeshCount);

	//allocate submesh memory
	meshFormat.submeshes = (K15_SubMeshFormat*)malloc(sizeof(K15_SubMeshFormat) * meshFormat.submeshCount);

	for(uint32 submeshIndex = 0;
		submeshIndex < meshFormat.submeshCount;
		++submeshIndex)
	{
		K15_SubMeshFormat* currentSubmesh = &meshFormat.submeshes[submeshIndex];

		//material key
		K15_ReadData(p_DataAccessContext, sizeof(uint32), &currentSubmesh->materialIndex);

		//texture channel count
		K15_ReadData(p_DataAccessContext, sizeof(uint8), &currentSubmesh->textureChannelCount);

		//color channel count
		K15_ReadData(p_DataAccessContext, sizeof(uint8), &currentSubmesh->colorChannelCount);

		//vertex count
		K15_ReadData(p_DataAccessContext, sizeof(uint32), &currentSubmesh->vertexCount);

		//calculate size of vertex data
		uint32 vertexSizeInBytes =	K15_RESOURCE_DESC_SIZE_POSITION 
			+ K15_RESOURCE_DESC_SIZE_NORMAL
			+ K15_RESOURCE_DESC_SIZE_TANGENT
			+ K15_RESOURCE_DESC_SIZE_BITANGENT
			+ currentSubmesh->textureChannelCount * K15_RESOURCE_DESC_SIZE_TEXCOORD
			+ currentSubmesh->colorChannelCount * K15_RESOURCE_DESC_SIZE_COLOR;

		uint32 vertexDataSizeInBytes = vertexSizeInBytes * currentSubmesh->vertexCount;

		assert(vertexDataSizeInBytes % sizeof(float) == 0);

		//allocate memory for vertex data
		float* vertexData = (float*)malloc(vertexDataSizeInBytes);

		if(!vertexData)
		{
			//log error
			return K15_ERROR_OUT_OF_MEMORY;
		}

		//get vertex data
		K15_ReadData(p_DataAccessContext, vertexDataSizeInBytes, vertexData);

		currentSubmesh->vertexData = vertexData;

		//get triangle count
		K15_ReadData(p_DataAccessContext, sizeof(uint32), &currentSubmesh->triangleCount);

		//calculate size of triangle data
		uint32 triangleDataSizeInBytes = currentSubmesh->triangleCount * K15_RESOURCE_DESC_TRIANGLE_INDICES;

		assert(triangleDataSizeInBytes % sizeof(K15_TriangleFormat) == 0);

		//allocate memory for triangle data
		K15_TriangleFormat* triangleData = (K15_TriangleFormat*)malloc(triangleDataSizeInBytes);

		if(!triangleData)
		{
			//log error
			free(vertexData);
			currentSubmesh->vertexData = 0;
			return K15_ERROR_OUT_OF_MEMORY;
		}

		//read triangle data
		for (uint32 triangleIndex = 0;
			 triangleIndex < currentSubmesh->triangleCount;
			 ++triangleIndex)
		{
			K15_ReadData(p_DataAccessContext, K15_RESOURCE_DESC_TRIANGLE_INDICES, triangleData[triangleIndex].indices);
		}

		currentSubmesh->triangleData = triangleData;
	}

	*p_MeshFormat = meshFormat;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_InternalSaveMeshFormat(K15_MeshFormat* p_MeshFormat, K15_DataAccessContext* p_DataAccessContext, uint32 p_SaveFlags)
{
	assert(p_MeshFormat && p_DataAccessContext);

	K15_HeaderFormat headerFormat;

	if(K15_CreateHeader(&headerFormat, K15_RESOURCE_FORMAT_MESH) != K15_SUCCESS)
	{
		//log error
	}

	uint32 offset = 0;
	uint32 submeshCount = p_MeshFormat->submeshCount;

	//store header
	K15_WriteData(p_DataAccessContext, sizeof(K15_HeaderFormat), &headerFormat);

	//mesh name
	K15_WriteData(p_DataAccessContext, sizeof(uint32), &p_MeshFormat->meshNameLength);
	K15_WriteData(p_DataAccessContext, p_MeshFormat->meshNameLength, p_MeshFormat->meshName);

	//submesh count
	K15_WriteData(p_DataAccessContext, sizeof(uint16), &p_MeshFormat->submeshCount);

	//submesh data
	for(uint32 submeshIndex = 0;
		submeshIndex < submeshCount;
		++submeshIndex)
	{
		K15_SubMeshFormat submeshFormat = p_MeshFormat->submeshes[submeshIndex];

		//material key
		K15_WriteData(p_DataAccessContext, sizeof(uint32), &submeshFormat.materialIndex);

		//texture channel count
		K15_WriteData(p_DataAccessContext, sizeof(uint8), &submeshFormat.textureChannelCount);

		//color channel count
		K15_WriteData(p_DataAccessContext, sizeof(uint8), &submeshFormat.colorChannelCount);

		//vertex count
		K15_WriteData(p_DataAccessContext, sizeof(uint32), &submeshFormat.vertexCount);

		//interleave vertex data (Position | Normal | Tangent | Bitangent | Color | TexCoord)
		for(uint32 vertexIndex = 0;
			vertexIndex < submeshFormat.vertexCount;
			++vertexIndex)
		{
			//position
			K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_POSITION, ((byte*)submeshFormat.helper.positions) + submeshFormat.helper.vertexDataOffset);

			//normal
			K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_NORMAL, ((byte*)submeshFormat.helper.normals) + submeshFormat.helper.vertexDataOffset);

			//tangent
			K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_TANGENT, ((byte*)submeshFormat.helper.tangents) + submeshFormat.helper.vertexDataOffset);

			//bitangent
			K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_BITANGENT, ((byte*)submeshFormat.helper.bitangents) + submeshFormat.helper.vertexDataOffset);

			//increase Vertex Data Offset
			submeshFormat.helper.vertexDataOffset += sizeof(float) * 3;

			//color
			for(uint32 colorChannelIndex = 0;
				colorChannelIndex < submeshFormat.colorChannelCount;
				++colorChannelIndex)
			{
				K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_COLOR, ((byte*)submeshFormat.helper.colorChannels[colorChannelIndex]) + submeshFormat.helper.colorOffset);
				submeshFormat.helper.colorOffset += K15_RESOURCE_DESC_SIZE_COLOR;
			}

			//texture coordinates
			for(uint32 textureChannelIndex = 0;
				textureChannelIndex < submeshFormat.textureChannelCount;
				++textureChannelIndex)
			{
				K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_SIZE_TEXCOORD, ((byte*)submeshFormat.helper.texcoordChannels[textureChannelIndex]) + submeshFormat.helper.texcoordOffset);
				submeshFormat.helper.texcoordOffset += K15_RESOURCE_DESC_SIZE_TEXCOORD;
			}
		}

		//triangle count
		K15_WriteData(p_DataAccessContext, sizeof(uint32), &submeshFormat.triangleCount);

		for(uint32 triangleIndex = 0;
			triangleIndex < submeshFormat.triangleCount;
			++triangleIndex)
		{
			//indices 1, 2, 3
			K15_WriteData(p_DataAccessContext, K15_RESOURCE_DESC_TRIANGLE_INDICES, (byte*)submeshFormat.triangleData[triangleIndex].indices);
		}
	}

	if((p_SaveFlags & K15_SAVE_FLAG_FREE_DATA) > 0)
	{
		K15_FreeMeshFormat(*p_MeshFormat);
	}

	return K15_SUCCESS;
}
/*********************************************************************************/