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

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceImporterObj,ResourceImporterBase);
	typedef DynamicArray(Vector3) PositionArray;
	typedef PositionArray NormalArray;
	typedef DynamicArray(int) IndexArray;

	namespace internal
	{
		PositionArray getPositions(const String& p_OBJString);
		NormalArray getNormals(const PositionArray& p_Positions);
	}
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
		p_ExtensionSet.insert(".obj");
	}
	/*********************************************************************************/
	void ResourceImporterObj::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{

	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterObj::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		Mesh* mesh = K15_NEW Mesh();
		String objString((char*)p_ResourceData.data);
		PositionArray positions;
		NormalArray normals;

		positions = internal::getPositions(objString);
		normals = internal::getNormals(positions);

		uint32 vertexBufferSize = sizeof(Vector3) * 2 * positions.size();
		uint32 vertexBufferTempOffset = 0;
		byte* vertexBufferTemp = (byte*)OSLayer::os_malloc(vertexBufferSize);

		for(size_t i = 0; i < positions.size(); ++i)
		{
			Vector3 position = positions[i];
			Vector3 normal = normals[i / 3];

			memcpy(vertexBufferTemp + vertexBufferTempOffset, &position, sizeof(Vector3));
			vertexBufferTempOffset += sizeof(Vector3);

			memcpy(vertexBufferTemp + vertexBufferTempOffset, &normal, sizeof(Vector3));
			vertexBufferTempOffset += sizeof(Vector3);
		}

		VertexBuffer::CreationOptions vertexBufferOptions;

		vertexBufferOptions.InitialData.data = vertexBufferTemp;
		vertexBufferOptions.InitialData.size = vertexBufferSize;

		VertexBuffer* vertexBuffer = K15_NEW VertexBuffer(vertexBufferOptions);
		VertexDeclaration* vertexDeclaration = VertexDeclaration::create("PF3NF3");

		VertexData* vertexData = K15_NEW VertexData(vertexDeclaration, vertexBuffer, 0, positions.size());
		SubMesh* subMesh = K15_NEW SubMesh(mesh);

		subMesh->setVertexData(vertexData);

		OSLayer::os_free(vertexBufferTemp);

		return mesh;
	}
	/*********************************************************************************/



	/*********************************************************************************/
	PositionArray internal::getPositions(const String& p_OBJString)
	{
		PositionArray positions; IndexArray indices;
		PositionArray orderedPositions;

		StringStream stream(p_OBJString);
		char* lineBuffer = (char*)alloca(256);
		Vector3 position;
		int index[6] = {0};

		while(!stream.eof())
		{
			stream.getline(lineBuffer, 256);

			if(lineBuffer[0] == 'v' && isspace(lineBuffer[1]))
			{
				sscanf(lineBuffer, "v %f %f %f", &position.x, &position.y, &position.z);

				positions.push_back(position);
			}
			else if(lineBuffer[0] == 'f')
			{
				sscanf(lineBuffer, "f %d//%d %d//%d %d//%d", &index[0], &index[1], &index[2], &index[3], &index[4], &index[5]);

				indices.push_back(index[0] - 1);
				indices.push_back(index[2] - 1);
				indices.push_back(index[4] - 1);
			}
		}

		for(size_t i = 0; i < indices.size(); i += 3)
		{
			index[0] = indices[i];
			index[1] = indices[i + 1];
			index[2] = indices[i + 2];

			orderedPositions.push_back(positions[index[0]]);
			orderedPositions.push_back(positions[index[1]]);
			orderedPositions.push_back(positions[index[2]]);
		}

		return orderedPositions;
	}
	/*********************************************************************************/
	NormalArray internal::getNormals(const PositionArray& p_Positions)
	{
		NormalArray normals;
		size_t counter = 0;

		if(p_Positions.size() % 3 == 0)
		{
			while(counter < p_Positions.size())
			{
				Vector3 normal = Vector3::Cross(p_Positions[counter + 2] - p_Positions[counter + 1],
					p_Positions[counter] - p_Positions[counter + 1]);

				normal.normalize();

				normals.push_back(normal);

				counter += 3;
			}
		}

		return normals;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

