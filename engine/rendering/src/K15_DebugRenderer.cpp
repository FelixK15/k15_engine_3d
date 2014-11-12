/**
 * @file K15_DebugRenderer.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/10
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
#include "K15_DebugRenderer.h"

#include "K15_RenderQueue.h"
#include "K15_RenderOperation.h"
#include "K15_Vector3.h"
#include "K15_Vector4.h"
#include "K15_ColorRGBA.h"
#include "K15_AABB.h"
#include "K15_VertexBuffer.h"
#include "K15_VertexData.h"
#include "K15_Vertex.h"
#include "K15_IndexData.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_ResourceManager.h"
#include "K15_MathUtil.h"
#include "K15_MatrixUtil.h"
#include "K15_GpuProgramCatalog.h"
#include "K15_Material.h"
#include "K15_RendererBase.h"
#include "K15_GameObject.h"
#include "K15_ModelComponent.h"
#include "K15_MeshInstance.h"
#include "K15_Mesh.h"
#include "K15_SubMesh.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	uint32 const DebugRenderer::DEBUG_VERTEX_CACHE = 1000;
	namespace internal
	{
		GpuProgram* debugVertexShader = 0;
		GpuProgram* debugFragmentShader = 0;

		const String debugVertexShaderCode(
			"in vec3 a_Position;\n"
			"in vec4 a_Color;\n"
			"uniform mat4 g_ProjMatrix;\n"
			"uniform mat4 g_ViewMatrix;\n"
			"out vec4 v_Color;\n"
			"void main(void)\n"
			"{\n"
			"mat4 mvp = g_ProjMatrix * g_ViewMatrix;\n"
			"v_Color = a_Color;\n"
			"gl_Position = mvp * vec4(a_Position, 1.0);\n"
			"}"
			);

		const String debugFragmentShaderCode(
			"in vec4 v_Color;\n"
			"void main(void)\n"
			"{\n"
			"gl_FragColor = v_Color;\n"
			"}"
			);
	}
	/*********************************************************************************/
	DebugRenderer::DebugRenderer()
		: m_RenderQueue(0),
		m_VertexBuffer(0),
		m_BufferOffset(0),
		m_VertexCounter(0),
		m_Active(true)
	{
		m_RenderQueue = K15_NEW RenderQueue();
		m_VertexDeclaration = VertexDeclaration::create("PF3CF4");

		VertexBuffer::CreationOptions vb_option = VertexBuffer::DefaultOptions;
		vb_option.Size = m_VertexDeclaration->getVertexSize() * DEBUG_VERTEX_CACHE;
		vb_option.UsageOption = VertexBuffer::UO_DYNAMIC;
		m_VertexBuffer = K15_NEW VertexBuffer(vb_option);

		_createDebugMaterial();
	}
	/*********************************************************************************/
	DebugRenderer::~DebugRenderer()
	{
		K15_DELETE m_IndexBuffer;
		K15_DELETE m_VertexBuffer;
		K15_DELETE m_RenderQueue;
		K15_DELETE m_DebugMaterial;

		K15_DELETE internal::debugFragmentShader;
		K15_DELETE internal::debugVertexShader;
	}
	/*********************************************************************************/
	void DebugRenderer::drawPoint(const Vector3& p_Position, float p_Radius, const ColorRGBA& p_Color)
	{
		RenderOperation* rop = K15_NEW RenderOperation();
		uint32 bufferPos = m_BufferOffset;
		uint16 indices = m_VertexCounter;

		_writeVertex(p_Position, p_Color);
		rop->topology = RenderOperation::T_DOT;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, bufferPos, 1);
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawCross(const Vector3& p_Position, float p_Length, const ColorRGBA& p_Color)
	{
		RenderOperation* rop = K15_NEW RenderOperation();
		Vector3 points[6];

		points[0] = p_Position; points[0].x -= p_Length;
		points[1] = p_Position; points[1].x += p_Length;
		points[2] = p_Position; points[2].y -= p_Length;
		points[3] = p_Position; points[3].y += p_Length;
		points[4] = p_Position; points[4].z -= p_Length;
		points[5] = p_Position; points[5].z += p_Length;

		uint32 bufferPos = m_BufferOffset;
		uint16 indices = m_VertexCounter;

		for(int i = 0; i < 6; ++i)
		{
			_writeVertex(points[i], p_Color);
		}

		rop->topology = RenderOperation::T_LINE;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, bufferPos, 6);
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawLine(const Vector3& p_P1, const Vector3& p_P2, const ColorRGBA& p_Color)
	{
		RenderOperation* rop = K15_NEW RenderOperation();
		Vector3 points[] = {p_P1, p_P2};
		uint32 bufferPos = m_BufferOffset;
		uint16 indices = m_VertexCounter;

		for(int i = 0; i < 2; ++i)
		{
			_writeVertex(points[i], p_Color);
		}

		rop->topology = RenderOperation::T_LINE;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, bufferPos, 2);
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawArrow( const Vector3& p_Start, const Vector3& p_End, const ColorRGBA& p_Color )
	{
		Vector3 points[10];
		Vector3 delta = p_Start - p_End;
		float deltaLength = delta.length();
		float length = 0.5f;
		uint32 bufferPos = m_BufferOffset;
		uint16 indices = m_VertexCounter;
		points[0] = Vector3::Zero;
		points[1] = points[2] = points[3] = points[4] = points[5] = points[6] = points[7] = points[8] = points[9] = (Vector3::Forward * deltaLength);
		points[3].z += length;  points[3].x += length;  points[3].y += length;
		points[5].z += length;  points[5].x += length;  points[5].y -= length;
		points[7].z += length;  points[7].x -= length;  points[7].y += length;
		points[9].z += length;  points[9].x -= length;  points[9].y -= length;

		delta.normalize();

		float rotX = MathUtil::atan2(delta.y, delta.x);
		float rotY = MathUtil::atan2(delta.z, delta.x);

		Matrix4 rotation = MatrixUtil::rotate(Vector3::Right, rotX);
		rotation *= MatrixUtil::rotate(Vector3::Up, rotY);

		for(int i = 0; i < 10; ++i)
		{
			points[i] = rotation * points[i];
			_writeVertex(points[i], p_Color);
		}

		RenderOperation* rop = K15_NEW RenderOperation();
		rop->topology = RenderOperation::T_LINE;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, bufferPos, 10);
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawCube(const Vector3& p_Min, const Vector3& p_Max, const ColorRGBA& p_Color)
	{
		Vector3 points[] = {
			Vector3(p_Min.x, p_Min.y, p_Max.z),
			Vector3(p_Min.x, p_Max.y, p_Max.z),
			Vector3(p_Max.x, p_Max.y, p_Max.z),
			Vector3(p_Max.x, p_Min.y, p_Max.z),

			Vector3(p_Max.x, p_Max.y, p_Min.z),
			Vector3(p_Max.x, p_Min.y, p_Min.z),
			Vector3(p_Min.x, p_Max.y, p_Min.z),
			Vector3(p_Min.x, p_Max.y, p_Min.z)
		};

		uint16 indices = 36;
		uint16 vertexCountPrev = m_VertexCounter;
		uint32 offset = m_BufferOffset;

		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Max.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Max.z), p_Color);

		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Max.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Max.z), p_Color);

		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Max.z), p_Color);

		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Max.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Min.z), p_Color);

		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Min.z), p_Color);
		_writeVertex(Vector3(p_Max.x, p_Min.y, p_Max.z), p_Color);
		_writeVertex(Vector3(p_Min.x, p_Min.y, p_Max.z), p_Color);

		RenderOperation* rop = K15_NEW RenderOperation(); 
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, offset, indices);
		rop->material = m_DebugMaterial;
		rop->topology = RenderOperation::T_TRIANGLE;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawAABB(const AABB& p_AABB, const ColorRGBA& p_Color)
	{
		drawCube(p_AABB.getMin(), p_AABB.getMax(), p_Color);
	}
	/*********************************************************************************/
	void DebugRenderer::drawSphere(const Vector3& p_Center, float p_Radius, const ColorRGBA& p_Color)
	{
		
	}
	/*********************************************************************************/
	void DebugRenderer::drawNormals(GameObject* p_GameObject, float p_Length, const ColorRGBA& p_Color)
	{
		if(p_GameObject && p_GameObject->getModelComponent())
		{
			ModelComponent* modelComponent = p_GameObject->getModelComponent();
			Mesh* mesh = modelComponent->getMeshInstance()->getMesh();
			SubMesh* submesh = 0;
			VertexData* vertexData = 0;
			VertexDeclaration* vertexDeclaration = 0;
			Vertex* vertex = 0;
			Matrix4 modelMatrix = p_GameObject->getTransformation();
			VertexElement normalVertexElement;
			Vector3 normal; Vector3 position;
			bool foundNormalElement = true;


			const Mesh::SubMeshArray& submeshes = mesh->getSubMeshes();

			for(uint32 i = 0; i < submeshes.size(); ++i)
			{
				submesh = submeshes[i];
				vertexData = submesh->getVertexData();
				vertexDeclaration = vertexData->getVertexDeclaration();

				for(uint32 j = 0; j < vertexDeclaration->getElementCount(); ++j)
				{
					const VertexElement& element = vertexDeclaration->getElement(j);

					if(element.semantic == VertexElement::ES_NORMAL)
					{
						normalVertexElement = element;
						foundNormalElement = true;
						break;
					}
				}


				if(foundNormalElement)
				{
					uint32 renderQueueSize = 0;
					for(uint32 j = 0; j < vertexData->getVertexCount(); ++j)
					{
						vertex = vertexData->getVertex(j);
						normal = vertex->getNormal();
						position = vertex->getPosition();

						normal = modelMatrix * normal;
						position = modelMatrix * position;

						normal *= p_Length;

						drawLine(position, position + normal, p_Color);

						renderQueueSize = m_RenderQueue->size();
					}
				}

				foundNormalElement = false;
			}
		}
	}
	/*********************************************************************************/
	void DebugRenderer::drawAxis(GameObject* p_GameObject, float p_Length)
	{
		NodeComponent* nodeComponent = 0;

		if((nodeComponent = p_GameObject->getNodeComponent()) != 0)
		{
			const Matrix4& transformation = nodeComponent->getTransformation();
			Vector3 position = p_GameObject->getPosition();
			Vector3 xAxis = transformation.getXAxis() * p_Length;
			Vector3 yAxis = transformation.getYAxis() * p_Length;
			Vector3 zAxis = transformation.getZAxis() * p_Length;

			//position = transformation * position;

			drawArrow(position, position + xAxis, ColorRGBA::Red);
			drawArrow(position, position + yAxis, ColorRGBA::Green);
			drawArrow(position, position + zAxis, ColorRGBA::Blue);
		}
	}
	/*********************************************************************************/
	void DebugRenderer::_writeVertex(const Vector3& p_Position, const ColorRGBA& p_Color)
	{
		Vector4 colorVec;
		p_Color.toColorVector(colorVec);
		m_VertexBuffer->writeData(sizeof(Vector3), (byte*)&p_Position, m_BufferOffset);
		m_BufferOffset += sizeof(Vector3);
		m_VertexBuffer->writeData(sizeof(Vector4), (byte*)&colorVec, m_BufferOffset);
		m_BufferOffset += sizeof(Vector4);
		++m_VertexCounter;
	}
	/*********************************************************************************/
	void DebugRenderer::clearRenderQueue()
	{
		for(uint32 i = 0; i < m_RenderQueue->size(); ++i)
		{
			RenderOperation* rop = m_RenderQueue->getRenderOperation(i);
			K15_DELETE rop->vertexData;
			K15_DELETE rop->indexData;
		}

		m_RenderQueue->clear();
		m_BufferOffset = 0;
		m_VertexCounter = 0;
	}
	/*********************************************************************************/
	void DebugRenderer::_createDebugMaterial()
	{
		m_DebugMaterial = K15_NEW Material();
		m_DebugMaterial->setName(_TN(DebugMaterial));

		MaterialPass* pass1 = m_DebugMaterial->getPass(0, true);

		internal::debugFragmentShader = K15_NEW GpuProgram("DebugFragment", GpuProgram::PS_FRAGMENT);
		internal::debugVertexShader   = K15_NEW GpuProgram("DebugVertex", GpuProgram::PS_VERTEX);

		internal::debugFragmentShader->setProgramCode(internal::debugFragmentShaderCode, true);
		internal::debugVertexShader->setProgramCode(internal::debugVertexShaderCode, true);

		pass1->setProgram(internal::debugFragmentShader, GpuProgram::PS_FRAGMENT);
		pass1->setProgram(internal::debugVertexShader, GpuProgram::PS_VERTEX);
		pass1->setFillMode(RendererBase::FM_WIREFRAME);
	}
	/*********************************************************************************/
}}