/**
 * @file K15_ResourceImporterObj.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/26
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

#include "K15_PrecompiledHeader.h"

#include "K15_ResourceManager.h"
#include "K15_ResourceImporterObj.h"
#include "K15_RawData.h"

#include "K15_RendererBase.h"

#include "K15_Mesh.h"
#include "K15_SubMesh.h"

#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_Material.h"

#include "K15_VertexData.h"
#include "K15_IndexData.h"

#include <K15_MeshFormat.h>

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceImporterObj,ResourceImporterBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceImporterObj::ResourceImporterObj()
		: ResourceImporterBase("Mesh")
	{

	}
	/*********************************************************************************/
	ResourceImporterObj::~ResourceImporterObj()
	{

	}
	/*********************************************************************************/
	void ResourceImporterObj::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".k15mesh");
	}
	/*********************************************************************************/
	void ResourceImporterObj::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{

	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterObj::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		K15_MeshFormat meshFormat;

		if (K15_LoadMeshFormatFromMemory(&meshFormat, p_ResourceData.data) != K15_SUCCESS)
		{
			return 0;
		}

		uint32 submeshCount = meshFormat.submeshCount;
		String vertexDeclearationString = "";

		Mesh* mesh = K15_NEW Mesh();
		mesh->setName(meshFormat.meshName);

		for(uint32 submeshIndex = 0;
			submeshIndex < submeshCount;
			++submeshIndex)
		{
			K15_SubMeshFormat* currentSubmeshFormat = &meshFormat.submeshes[submeshIndex];
			SubMesh* currentSubmesh = K15_NEW SubMesh(mesh);
			Material* submeshMaterial = g_ResourceManager->getResource<Material>("Robbie_the_Rabbit_rigged.k15material");
		
			vertexDeclearationString += "PF3NF3GF3BF3";

			for(uint8 textureChannelIndex = 0;
				textureChannelIndex < currentSubmeshFormat->textureChannelCount;
				++textureChannelIndex)
			{
				vertexDeclearationString += "TF2";
			}

			for(uint8 colorChannelIndex = 0;
				colorChannelIndex < currentSubmeshFormat->colorChannelCount;
				++colorChannelIndex)
			{
				vertexDeclearationString += "CF4";
			}

			VertexDeclaration* vertexDeclaration = VertexDeclaration::create(vertexDeclearationString);

			uint32 indexCount = currentSubmeshFormat->triangleCount * 3;
			uint32 vertexCount = currentSubmeshFormat->vertexCount;
			uint32 vertexDataSize = vertexCount * vertexDeclaration->getVertexSize();
			uint32 indexDataSize = indexCount * sizeof(uint16);

			VertexBuffer::CreationOptions vboOptions;
			IndexBuffer::CreationOptions iboOptions;

			vboOptions.InitialData.data = (byte*)currentSubmeshFormat->vertexData;
			vboOptions.InitialData.size = vertexDataSize;

			uint16* rawIndexData = (uint16*)malloc(indexDataSize);

			for(uint32 triangleIndex = 0;
				triangleIndex < currentSubmeshFormat->triangleCount;
				++triangleIndex)
			{
				uint32 index = triangleIndex * 3;
				rawIndexData[index] = currentSubmeshFormat->triangleData[triangleIndex].indices[0];
				rawIndexData[index + 1] = currentSubmeshFormat->triangleData[triangleIndex].indices[1];
				rawIndexData[index + 2] = currentSubmeshFormat->triangleData[triangleIndex].indices[2];
			}

			iboOptions.InitialData.data = (byte*)rawIndexData;
			iboOptions.InitialData.size = indexDataSize;

			iboOptions.IndexType = IndexBuffer::IT_UINT16;

			VertexBuffer* submeshVertexBuffer = K15_NEW VertexBuffer(vboOptions);
			IndexBuffer* submeshIndexBuffer = K15_NEW IndexBuffer(iboOptions);

			VertexData* vertexData = K15_NEW VertexData(vertexDeclaration, submeshVertexBuffer, 0, vertexCount);
			IndexData* indexData = K15_NEW IndexData(submeshIndexBuffer, indexCount, 0);

			currentSubmesh->setMaterial(submeshMaterial);
			currentSubmesh->setVertexData(vertexData);
			currentSubmesh->setIndexData(indexData);

			free(rawIndexData);
		}
		

		return mesh;
	}
	/*********************************************************************************/




	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

