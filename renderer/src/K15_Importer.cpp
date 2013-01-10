/**
 * @file K15_Importer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_Importer.h"
using namespace K15_Renderer;

bool Importer::ImportOBJ(const char *pFileName,VertexLayoutPtr pVertexLayout,float *&pVertices,U32 *&pIndices,U32 &iVertexCount,U32 &iIndexCount)
{
	U32 iNumVertices = 0;
	U32 iNumIndices = 0;

	char cBuffer[128];
	FILE *pFile = fopen(pFileName,"r");
	if(pFile){
		while(fgets(cBuffer,128,pFile) != NULL){
			if(cBuffer[0] == 'v'){
				++iNumVertices;
			}else if(cBuffer[0] == 'f'){
				iNumIndices += 3;
			}
		}
		rewind(pFile);
		U32 iAmountFloatPerVertex = pVertexLayout->GetSizeOfVertex() / sizeof(float);
		pVertices = K15_NEW float[iAmountFloatPerVertex * iNumVertices];
		pIndices = K15_NEW U32[iNumIndices];

		float *fVertex = K15_NEW float[iAmountFloatPerVertex];

		U32 iVertexIndex = 0;
		U32 iIndexIndex = 0;
		U32 iColorIndex = 4;

		while(fgets(cBuffer,128,pFile) != NULL){
			if(cBuffer[0] == 'v'){
				//Vertex
				char cColor = '-';
				sscanf(&cBuffer[1],"%f %f %f%c",&fVertex[0],&fVertex[1],&fVertex[2],&cColor);
				Memory::MemCopy((void*)&pVertices[iVertexIndex],&fVertex[0],3 * sizeof(float));
				iVertexIndex += 3;
				if(cColor == 'c'){ //Temporary
					sscanf(&cBuffer[31],"%f %f %f %f",&fVertex[3],&fVertex[4],&fVertex[5],&fVertex[6]);
					Memory::MemCopy((void*)&pVertices[iVertexIndex],&fVertex[3],4 * sizeof(float));
					iVertexIndex += 4;
				}
			}else if(cBuffer[0] == 'f'){
				//face
				U32 i1 = 0;
				U32 i2 = 0;
				U32 i3 = 0;

				sscanf(&cBuffer[1],"%d %d %d",&i1,&i2,&i3);
				pIndices[iIndexIndex++] = --i1;
				pIndices[iIndexIndex++] = --i2;
				pIndices[iIndexIndex++] = --i3;
			}
		}

		iIndexCount = iNumIndices;
		iVertexCount = iNumVertices;

		fclose(pFile);
		K15_DELETE[] fVertex;
		return true;
	}

	return false;
}