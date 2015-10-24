#ifndef _K15_ResourceCompiler_TextureAtlas_h_
#define _K15_ResourceCompiler_TextureAtlas_h_

#include "K15_ResourceCompilerConfig.h"

struct K15_TextureAtlasNode
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int area;
	unsigned char* pixelData;
};

struct K15_TextureAtlas
{
	unsigned int width;
	unsigned int height;
	unsigned int numComponents;
	unsigned int numNodes;
	
	unsigned char* pixelData;
	unsigned char* accessMap;
	K15_TextureAtlasNode* atlasNodes;
};

K15_TextureAtlas K15_CreateTextureAtlas(unsigned int p_Components);
void K15_FreeTextureAtlas(K15_TextureAtlas* p_TextureAtlas);

void K15_SetTextureCount(K15_TextureAtlas* p_TextureAtlas, unsigned int p_TextureCount);
void K15_AddTextureToAtlas(K15_TextureAtlas* p_TextureAtlas, unsigned char* p_PixelData, 
						   unsigned int p_PixelDataWidth, unsigned int p_PixelDataHeight,
						   unsigned int p_TextureIndex, unsigned int* p_OutX = 0, unsigned int* p_OutY = 0);

#endif //_K15_ResourceCompiler_TextureAtlas_h_