#include "K15_TextureAtlas.h"

#include "K15_Math.h"

/*********************************************************************************/
intern int K15_InternalSortTextureAtlasNodesByArea(const void* p_NodeA, const void* p_NodeB)
{
	K15_TextureAtlasNode* nodeA = (K15_TextureAtlasNode*)p_NodeA;
	K15_TextureAtlasNode* nodeB = (K15_TextureAtlasNode*)p_NodeB;

	return nodeB->area - nodeA->area;
}
/*********************************************************************************/
intern void K15_InternalSetAccessFlag(unsigned char* p_AccessMap, unsigned int p_PosX, unsigned int p_PosY, unsigned int p_Width, unsigned int p_Height, unsigned int p_Stride)
{
	unsigned int index = p_PosY * p_Stride + p_PosX;

	for (unsigned int y = p_PosY;
		y < p_Height + p_PosY;
		++y)
	{
		memset(p_AccessMap + index, 1, p_Width);
		index += p_Stride;
	}
}
/*********************************************************************************/
intern void K15_InternalSetBitmap(unsigned char* p_AtlasBitmap, unsigned char* p_NodeBitmap, unsigned int p_NumAtlasColorComponents, unsigned int p_PosX, unsigned int p_PosY, unsigned int p_NodeWidth, unsigned int p_NodeHeight, unsigned int p_Stride)
{
	const unsigned int nodeBitmapStride = p_NodeWidth;
	unsigned int bitmapIndex = p_PosY * p_Stride + p_PosX;

	for (unsigned int y = p_PosY;
		y < p_PosY + p_NodeHeight;
		++y)
	{
		memcpy(p_AtlasBitmap + bitmapIndex, p_NodeBitmap + (y-p_PosY) * nodeBitmapStride, nodeBitmapStride);
		bitmapIndex += p_Stride;
	}
}
/*********************************************************************************/
intern void K15_InternalPrintAccessMap(unsigned char* p_AccessMap, unsigned int p_Width, unsigned int p_Height, unsigned int p_MarkIndex)
{
	for (unsigned int y = 0;
		y < p_Height;
		++y)
	{
		for (unsigned int x = 0;
			x < p_Width;
			++x)
		{
			unsigned int index = x + y * p_Width;

			if (p_MarkIndex != 0xFFFFFFFF &&
				index == p_MarkIndex)
			{
				printf("x");
			}
			else
			{
				printf("%d", p_AccessMap[x + y * p_Width]);
			}
		}
		printf("\n");
	}
	printf("\n----------------------------------------------------------------------\n");
}
/*********************************************************************************/
intern bool8 K15_InternalAddTextureNodeToTexture(K15_TextureAtlas* p_TextureAtlas, K15_TextureAtlasNode* p_TextureAtlasNode, unsigned char* p_AtlasBitmap, unsigned char* p_AccessMap, unsigned int* p_OutPosX, unsigned int* p_OutPosY)
{
	const unsigned int atlasWidth = p_TextureAtlas->width;
	const unsigned int atlasHeight = p_TextureAtlas->height;
	const unsigned int nodeWidth = p_TextureAtlasNode->width;	
	const unsigned int nodeHeight = p_TextureAtlasNode->height;
	const unsigned int nodeArea = p_TextureAtlasNode->area;
	const unsigned int stride = p_TextureAtlas->width;
	const unsigned int numAtlasColorComponents = p_TextureAtlas->numComponents;
	unsigned char* nodeBitmap = p_TextureAtlasNode->pixelData;

	unsigned int posY = 0;
	unsigned int posX = 0;
	unsigned int counter = 0;

	const unsigned int maxAccessIndex = p_TextureAtlas->width * p_TextureAtlas->height;
	bool8 foundPossibleMatch = K15_TRUE;
	bool8 foundMatch = K15_FALSE;

	//find next free item in the access map
	for (unsigned int accessIndex = 0;
		accessIndex < maxAccessIndex;
		++accessIndex)
	{
		foundPossibleMatch = K15_TRUE;

		if (p_AccessMap[accessIndex] == 0)
		{
			//try to insert
			posY = accessIndex / stride;
			posX = accessIndex - (posY * stride);
			counter = 0;

			if (posY + nodeHeight > atlasHeight ||
				posX + nodeWidth > atlasWidth)
			{
				foundPossibleMatch = K15_FALSE;
				continue;
			}

			for (unsigned int y = posY;
				y < nodeHeight + posY;
				++y)
			{
				for (unsigned int x = posX;
					x < nodeWidth + posX;
					++x, ++counter)
				{
					const unsigned int index = y * stride + x;

					if (p_AccessMap[index] != 0)
					{
						foundPossibleMatch = K15_FALSE;
						break;
					}
				}

				if (counter == nodeArea)
				{
					*p_OutPosX = posX;
					*p_OutPosY = posY;
					foundMatch = K15_TRUE;
					break;
				}

				if (!foundPossibleMatch)
				{
					break;
				}
			}

			if (foundMatch)
			{
				break;
			}
		}

		if (foundMatch)
		{
			break;
		}
	}
	

	if (foundMatch)
	{
		K15_InternalSetAccessFlag(p_AccessMap, posX, posY, nodeWidth, nodeHeight, stride);
		K15_InternalSetBitmap(p_AtlasBitmap, nodeBitmap, numAtlasColorComponents, posX, posY, nodeWidth, nodeHeight, stride);
	}

	return foundMatch;
}
/*********************************************************************************/
intern void K15_InternalResizeTextureAtlas(K15_TextureAtlas* p_TextureAtlas)
{
	unsigned int oldWidth = p_TextureAtlas->width;
	unsigned int oldHeight = p_TextureAtlas->height;

	unsigned int newWidth = oldWidth * 2;
	unsigned int newHeight = oldHeight * 2;

	unsigned char* oldAtlasBitmap = p_TextureAtlas->pixelData;
	unsigned char* oldAccessMap = p_TextureAtlas->accessMap;
	unsigned char* newAtlasBitmap = (unsigned char*)malloc(newWidth * newHeight * p_TextureAtlas->numComponents);
	unsigned char* newAccessMap = (unsigned char*)malloc(newWidth * newHeight);

	memset(newAccessMap, 0, newWidth * newHeight);

	unsigned int oldStride = oldWidth;
	unsigned int newStride = newWidth;

	for (unsigned int y = 0;
		y < oldWidth;
		++y)
	{
		memcpy(newAtlasBitmap + newStride * y, oldAtlasBitmap + oldStride * y, oldStride * p_TextureAtlas->numComponents);
		memcpy(newAccessMap + newStride * y, oldAccessMap + oldStride * y, oldStride);
	}

	free(oldAtlasBitmap);
	free(oldAccessMap);

	p_TextureAtlas->pixelData = newAtlasBitmap;
	p_TextureAtlas->accessMap = newAccessMap;

	p_TextureAtlas->width = newWidth;
	p_TextureAtlas->height = newHeight;
}
/*********************************************************************************/
K15_TextureAtlas K15_CreateTextureAtlas(unsigned int p_Components)
{
	K15_TextureAtlas atlas = {};
	atlas.width = 16;
	atlas.height = 16;
	atlas.numComponents = p_Components;
	atlas.numNodes = 0;
	atlas.accessMap = (unsigned char*)malloc(16*16);
	atlas.pixelData = (unsigned char*)malloc(16*16*p_Components);
	
	memset(atlas.accessMap, 0, 16*16);
	memset(atlas.pixelData, 0, 16*16*p_Components);

	return atlas;
}
/*********************************************************************************/
void K15_FreeTextureAtlas(K15_TextureAtlas* p_TextureAtlas)
{
	free(p_TextureAtlas->pixelData);
	free(p_TextureAtlas->atlasNodes);
	free(p_TextureAtlas->accessMap);
}
/*********************************************************************************/
void K15_SetTextureCount(K15_TextureAtlas* p_TextureAtlas, unsigned int p_TextureCount)
{
	if (p_TextureAtlas->atlasNodes)
	{
		free(p_TextureAtlas->atlasNodes);
	}

	p_TextureAtlas->atlasNodes = (K15_TextureAtlasNode*)malloc(sizeof(K15_TextureAtlasNode) * p_TextureCount);
	p_TextureAtlas->numNodes = p_TextureCount;
}
/*********************************************************************************/
void K15_AddTextureToAtlas(K15_TextureAtlas* p_TextureAtlas, unsigned char* p_PixelData, unsigned int p_PixelDataWidth, unsigned int p_PixelDataHeight, unsigned int p_TextureIndex, unsigned int* p_OutX, unsigned int* p_OutY)
{
	K15_ASSERT(p_TextureAtlas->numNodes > p_TextureIndex);

	if (p_PixelDataWidth == 0 ||
		p_PixelDataHeight == 0)
	{
		return;
	}

	K15_TextureAtlasNode* node = &p_TextureAtlas->atlasNodes[p_TextureIndex];

	node->area = p_PixelDataWidth * p_PixelDataHeight;
	node->height = p_PixelDataHeight;
	node->width = p_PixelDataWidth;
	node->x = 0;
	node->y = 0;
	node->pixelData = p_PixelData;

	while (!K15_InternalAddTextureNodeToTexture(p_TextureAtlas, node, p_TextureAtlas->pixelData, p_TextureAtlas->accessMap, &node->x, &node->y))
	{
		K15_InternalResizeTextureAtlas(p_TextureAtlas);
	}

	if (p_OutX)
	{
		*p_OutX = node->x;
	}

	if (p_OutY)
	{
		*p_OutY = node->y;
	}
}
/*********************************************************************************/