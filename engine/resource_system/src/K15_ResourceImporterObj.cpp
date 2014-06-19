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
/*#include "K15_Vector3.h"*/
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_Material.h"

#include "tiny_obj_loader.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceImporterObj,ResourceImporterBase);
	typedef std::vector<tinyobj::shape_t> ShapeArray;
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
		StringStream stream;
		stream.write((char*)p_ResourceData.data,p_ResourceData.size);

		ShapeArray shapes;
		tinyobj::MaterialFileReader reader("");
		String err = tinyobj::LoadObj(shapes,(std::istream&)stream,reader);

		if(!err.empty())
		{
			setError(err);
			return 0;
		}

		Mesh* mesh = K15_NEW Mesh();
		SubMesh* submesh = 0;
		IndexBuffer* idbuffer = 0;
		VertexBuffer* vbbuffer = 0;
		VertexDeclaration* vdeclaration = 0;

		for(ShapeArray::iterator iter = shapes.begin();iter != shapes.end();++iter)
		{
			submesh = K15_NEW SubMesh(mesh);
			IndexBuffer::CreationOptions indexOptions;
			VertexBuffer::CreationOptions vertexOptions;
			String vertexDeclarationString;

			indexOptions.IndexType = IndexBuffer::IT_UINT16;

			tinyobj::shape_t& shape = (*iter);

			//Check if the indices are greater than 16bit
			for(uint32 i = 0;i < shape.mesh.indices.size();++i)
			{
				if(shape.mesh.indices.at(i) > std::numeric_limits<uint16>::max())
				{
					indexOptions.IndexType = IndexBuffer::IT_UINT32;
				}
			}

			uint32 indexSize = indexOptions.IndexType == IndexBuffer::IT_UINT32 ?  sizeof(uint32) : sizeof(uint16);
			uint32 indexDataSize = indexSize * shape.mesh.indices.size();

			byte* indexTempBuffer = K15_NEW_SIZE(Allocators[AC_GENERAL],indexDataSize) byte;

			//copy index data
			uint32 indexBufferPos = 0;
			for(uint32 i = 0;i < shape.mesh.indices.size();++i)
			{
				memcpy(indexTempBuffer + indexBufferPos,&shape.mesh.indices[i],indexSize);
				indexBufferPos += indexSize;
			}

			//set index data
			indexOptions.InitialData.data = indexTempBuffer;
			indexOptions.InitialData.size = indexDataSize;

			uint32 vertexDataSize = 0;
			byte* vertexTempBuffer = 0;

			//calculate vertex buffer size
			vertexDataSize = (shape.mesh.positions.size() + shape.mesh.positions.size() / 3) * sizeof(float);
			vertexDataSize += shape.mesh.normals.size() * sizeof(float);
			vertexDataSize += shape.mesh.texcoords.size() * sizeof(float);

			//allocate vertex temp buffer
			vertexTempBuffer = (byte*)K15_NEW_SIZE(Allocators[AC_GENERAL],vertexDataSize) byte;

			if(shape.mesh.positions.size() > 0)
			{
				vertexDeclarationString += "PF4";
			}

			if(shape.mesh.normals.size() > 0)
			{
				vertexDeclarationString += "NF3";
			}

			if(shape.mesh.texcoords.size() > 0)
			{
				vertexDeclarationString += "TF2";
			}

			uint32 bufferPosition = 0;
			static const float wComponent = 1.0f;

			uint32 pos_position = 0, pos_normal = 0, pos_tex = 0;
			for(uint32 i = 0;i < shape.mesh.indices.size();++i)
			{
				if(shape.mesh.positions.size() > pos_position)
				{
					memcpy(vertexTempBuffer + bufferPosition,&shape.mesh.positions[pos_position],sizeof(float)*3);
					bufferPosition += sizeof(float)*3;
					memcpy(vertexTempBuffer + bufferPosition,&wComponent,sizeof(float));
					bufferPosition += sizeof(float);

					pos_position += 3;
				}

				if(shape.mesh.normals.size() > pos_normal)
				{
					memcpy(vertexTempBuffer + bufferPosition,&shape.mesh.normals[pos_normal],sizeof(float)*3);
					bufferPosition += sizeof(float)*3;

					pos_normal += 3;
				}

				if(shape.mesh.texcoords.size() > pos_tex)
				{
					memcpy(vertexTempBuffer + bufferPosition,&shape.mesh.texcoords[pos_tex],sizeof(float)*2);
					bufferPosition += sizeof(float)*2;

					pos_tex += 2;
				}
			}

			vertexOptions.InitialData.data = vertexTempBuffer;
			vertexOptions.InitialData.size = vertexDataSize;

			vbbuffer = K15_NEW VertexBuffer(vertexOptions);
			idbuffer = K15_NEW IndexBuffer(indexOptions);
			vdeclaration = VertexDeclaration::create(vertexDeclarationString);

			vbbuffer->setVertexDeclaration(vdeclaration);

			submesh->setVertexBuffer(vbbuffer);
			submesh->setIndexBuffer(idbuffer);
		
			K15_DELETE_SIZE(Allocators[AC_GENERAL],vertexTempBuffer,vertexDataSize);
			K15_DELETE_SIZE(Allocators[AC_GENERAL],indexTempBuffer,indexDataSize);
		}

		return mesh;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace