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
#include "K15_Vector3.h"
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_Material.h"

namespace K15_Engine { namespace Core {
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
	ResourceBase* ResourceImporterObj::_load(const RawData& p_ResourceData)
	{
		String line;
		StringStream objContent;
		objContent.write((char*)p_ResourceData.data,p_ResourceData.size);

		struct face
		{
			int p1;
			int n1;

			int p2;
			int n2;

			int p3;
			int n3;
		};

		DynamicArray(Vector3) positions;
		DynamicArray(Vector3) normals;
		DynamicArray(face) faces;

		while(std::getline(objContent,line))
		{
			//check for normal
			if(line[0] == 'v' && line[1] == 'n')
			{
				Vector3 normal;
				sscanf(line.c_str(),"vn %f %f %f",&normal.x,&normal.y,&normal.z);

				normals.push_back(normal);
				
			}
			else if(line.find_first_of("v") == 0) //pos
			{
				Vector3 pos;
				sscanf(line.c_str(),"v %f %f %f",&pos.x,&pos.y,&pos.z);

				positions.push_back(pos);
			}
			else if(line.find_first_of("f") == 0) //face
			{
				face curFace;

				sscanf(line.c_str(),"f %i//%i %i//%i %i//%i",&curFace.p1,&curFace.n1,&curFace.p2,&curFace.n2,&curFace.p3,&curFace.n3);

				faces.push_back(curFace);
			}
		}

		int vbo_size = sizeof(float) *  18 * faces.size();
		int ibo_size = sizeof(uint16) * faces.size();
		uint16* indexBuffer_temp = K15_NEW_SIZE(Allocators[AC_CORE],ibo_size) uint16;
		float* vertexBuffer_temp = K15_NEW_SIZE(Allocators[AC_GENERAL],vbo_size) float;

		int id_counter= 0;
		int counter = 0;
		for(uint16 i = 0;i < faces.size();++i)
		{
			int pi1 = faces.at(i).p1 - 1;
			int pi2 = faces.at(i).p2 - 1;
			int pi3 = faces.at(i).p3 - 1;

			int ni1 = faces.at(i).n1 - 1;
			int ni2 = faces.at(i).n2 - 1;
			int ni3 = faces.at(i).n3 - 1;

			Vector3& p1 = positions.at(pi1);
			Vector3& p2 = positions.at(pi2);
			Vector3& p3 = positions.at(pi3);

			Vector3& n1 = normals.at(ni1);
			Vector3& n2 = normals.at(ni2);
			Vector3& n3 = normals.at(ni3);

			//Vertex 1
			memcpy(vertexBuffer_temp + counter,&p1,sizeof(Vector3));
			counter += 3;
			memcpy(vertexBuffer_temp + counter,&n1,sizeof(Vector3));
			counter +=3;

			//Vertex 2
			memcpy(vertexBuffer_temp + counter,&p2,sizeof(Vector3));
			counter += 3;
			memcpy(vertexBuffer_temp + counter,&n2,sizeof(Vector3));
			counter +=3;

			//Vertex 3
			memcpy(vertexBuffer_temp + counter,&p3,sizeof(Vector3));
			counter += 3;
			memcpy(vertexBuffer_temp + counter,&n3,sizeof(Vector3));
			counter +=3;


			indexBuffer_temp[id_counter++] = i;
		}

		VertexBuffer::CreationOptions vbo_opts;
		IndexBuffer::CreationOptions id_opts;

		vbo_opts.BufferType = VertexBuffer::BT_VERTEX_BUFFER;
		vbo_opts.AccessOption = VertexBuffer::BA_WRITE_ONLY;
		vbo_opts.LockOption = VertexBuffer::LO_NORMAL;
		vbo_opts.UsageOption = VertexBuffer::UO_STATIC;
		vbo_opts.VertexLayout = K15_NEW VertexDeclaration("PF3NF3");
		vbo_opts.InitialData.data = (byte*)vertexBuffer_temp;
		vbo_opts.InitialData.size = vbo_size;

		id_opts.AccessOption = IndexBuffer::BA_WRITE_ONLY;
		id_opts.LockOption = IndexBuffer::BA_WRITE_ONLY;
		id_opts.IndexType = IndexBuffer::IT_UINT16;
		id_opts.InitialData.data = (byte*)indexBuffer_temp;
		id_opts.InitialData.size = ibo_size;

		VertexBuffer* vbo = K15_NEW VertexBuffer(vbo_opts);
		IndexBuffer* ibo = K15_NEW IndexBuffer(id_opts);

		Mesh* mesh = K15_NEW Mesh("");
		SubMesh* submesh = K15_NEW SubMesh();
		Material* mat = K15_NEW Material();
		
		mat->getPass(0,true)->setFillMode(RendererBase::FM_WIREFRAME);

		
		submesh->setVertexBuffer(vbo);
		submesh->setIndexBUffer(ibo);
		submesh->setMaterial(mat);

		mesh->addSubMesh(submesh);

		return mesh;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace