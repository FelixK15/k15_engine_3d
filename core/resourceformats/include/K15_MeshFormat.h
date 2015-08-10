#ifndef _K15_ResourceFormat_MeshFormat_h_
#define _K15_ResourceFormat_MeshFormat_h_

#include "K15_ResourceFormats_Prerequisites.h"

struct K15_TriangleFormat
{
	uint16 indices[3];
};

struct K15_SubmeshFormatHelper
{
	float* positions;
	float* normals;
	float* tangents;
	float* bitangents;
	float** colorChannels;
	float** texcoordChannels;

	uint32 vertexDataOffset;	// same offset for positions, normals, tangents and bitangents
	uint32 colorOffset;
	uint32 texcoordOffset;
	uint32 triangleDataOffset;
};

struct K15_SubMeshFormat
{
	//Will not get serialized
	K15_SubmeshFormatHelper helper;

	K15_TriangleFormat* triangleData;

	uint32 materialNameHash;

	float* vertexData;

	uint32 vertexCount;
	uint32 triangleCount;
	uint32 materialIndex;
	uint8 textureChannelCount;
	uint8 colorChannelCount;
};

struct K15_MeshFormat
{
	K15_SubMeshFormat* submeshes;
	uint32 meshNameHash;
	uint16 submeshCount;
};

uint8 K15_SetMeshName(K15_MeshFormat* p_MeshFormat, const char* p_Name);
uint8 K15_SetMeshSubmeshCount(K15_MeshFormat* p_MeshFormat, uint32 p_SubmeshCount);
uint8 K15_SetSubmeshVertexCount(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_VertexCount);
uint8 K15_SetSubmeshMaterial(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_MaterialIndex);

uint8 K15_SetSubmeshTriangleCount(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_TriangleCount);
uint8 K15_SetSubmeshTriangleData(K15_SubMeshFormat* p_SubmeshFormat, uint32 p_TriangleIndex, uint32* p_Indices);

uint8 K15_SetSubmeshChannelCount(K15_SubMeshFormat* p_SubmeshFormat, uint8 p_ChannelCount, uint8 p_DataIdentifier);

uint8 K15_SetSubmeshData(K15_SubMeshFormat* p_SubmeshFormat, float* p_PositionData, uint8 p_DataIdentifier);
uint8 K15_SetSubmeshDataWithIndex(uint32 p_Index, K15_SubMeshFormat* p_SubmeshFormat, float* p_Data, uint8 p_DataIdentifier);

uint8 K15_SaveMeshFormatToFile(K15_MeshFormat* p_MeshFormat, const char* p_FileName, uint32 p_SaveFlags);
uint8 K15_LoadMeshFormatFromFile(K15_MeshFormat* p_MeshFormat, const char* p_FileName);
uint8 K15_LoadMeshFormatFromMemory(K15_MeshFormat* p_MeshFormat, byte* p_Memory, uint32 p_MemorySize);

uint32 K15_GetSubMeshFormatVertexDataSize(K15_SubMeshFormat* p_SubMeshFormat);
uint32 K15_GetSubMeshFormatTriangleDataSize(K15_SubMeshFormat* p_SubMeshFormat);

void K15_FreeMeshFormat(K15_MeshFormat p_MeshFormat);

#endif //_K15_ResourceFormat_MeshFormat_h_