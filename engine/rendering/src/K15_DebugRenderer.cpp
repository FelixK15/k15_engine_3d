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
#include "K15_IndexData.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_ResourceManager.h"
#include "K15_MathUtil.h"
#include "K15_MatrixUtil.h"
#include "K15_GpuProgramCatalog.h"
#include "K15_Material.h"
#include "K15_RendererBase.h"

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

		IndexBuffer::CreationOptions ib_option = IndexBuffer::DefaultOptions;
		ib_option.Size = sizeof(uint16) * DEBUG_VERTEX_CACHE;
		ib_option.IndexType = IndexBuffer::IT_UINT16;
		ib_option.UsageOption = IndexBuffer::UO_DYNAMIC;
		m_IndexBuffer = K15_NEW IndexBuffer(ib_option);

		m_DebugMaterial = K15_NEW Material();
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
		rop->indexData = K15_NEW IndexData(m_IndexBuffer, 1, indices * sizeof(uint16));
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
		rop->indexData = K15_NEW IndexData(m_IndexBuffer, 6, indices * sizeof(uint16));
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
		rop->indexData = K15_NEW IndexData(m_IndexBuffer, 2, indices * sizeof(uint16));
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
      //points[i] += p_Start;
      points[i] = rotation * points[i];
		  _writeVertex(points[i], p_Color);
		}

		RenderOperation* rop = K15_NEW RenderOperation();
		rop->topology = RenderOperation::T_LINE;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, bufferPos, 10);
		rop->indexData = K15_NEW IndexData(m_IndexBuffer, 10, indices * sizeof(uint16));
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
	}
	/*********************************************************************************/
	void DebugRenderer::drawCube(const Vector3& p_Min, const Vector3& p_Max, const ColorRGBA& p_Color)
	{
		Vector3 points[10] = {
      Vector3(p_Min.x, p_Min.y, p_Max.z),
      Vector3(p_Min.x, p_Max.y, p_Max.z),
      Vector3(p_Max.x, p_Max.y, p_Max.z),
      Vector3(p_Max.x, p_Min.y, p_Max.z),
      
      Vector3(p_Max.x, p_Max.y, p_Min.z),
      Vector3(p_Max.x, p_Min.y, p_Min.z),
      Vector3(p_Min.x, p_Max.y, p_Min.z),
      Vector3(p_Min.x, p_Max.y, p_Min.z))
    }

    points[0] = ;
    points[1] = Vector3(p_Min.x, p_Max.y, p_Max.z);
    points[2] = Vector3()
	}
	/*********************************************************************************/
	void DebugRenderer::drawAABB(const AABB& p_AABB, const ColorRGBA& p_Color)
	{
		drawCube(p_AABB.getMin(), p_AABB.getMax(), p_Color);
	}
	/*********************************************************************************/
	void DebugRenderer::drawSphere(const Vector3& p_Center, float p_Radius, const ColorRGBA& p_Color)
	{
		static int const verticesPerRing = 19;
		static int const rings = 5;
		static float const radiusStep = p_Radius / rings;
		static float const angleStep = MathUtil::TwoPi / verticesPerRing;
		float angle = 0.f; float sinA = 0.f; float cosA = 0.f;
		float y = 0.f; float radius = radiusStep;
		Vector3* vertices = (Vector3*)alloca(verticesPerRing * rings * sizeof(Vector3));
		int counter = 0; uint32 offset = m_BufferOffset; uint16 vertexCountPrev = m_VertexCounter;
		for(int i = 0; i < rings; ++i)
		{
			for(int j = 0; j < 10; ++j)
			{
				sinA = MathUtil::sin(angle) * radius;
				cosA = MathUtil::cos(angle) * radius;

				Vector3 vertex;
				vertex.y = y;
				vertex.x = cosA;
				vertex.z = sinA;
				vertex += p_Center;
				vertices[counter++] = vertex;
				if(j != 0)
				{
					vertices[counter++] = vertex;
				}

				angle += angleStep;
			}

			if(i < rings/2)
			{
				radius += radiusStep;
			}
			else
			{
				radius -= radiusStep;
			}

			y -= radiusStep;
			angle = 0.f;
		}

		for(int i = 0; i < counter; ++i)
		{
			_writeVertex(vertices[i], p_Color);
		}

		RenderOperation* rop = K15_NEW RenderOperation(); 
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, offset, counter);
		rop->indexData = K15_NEW IndexData(m_IndexBuffer, counter, vertexCountPrev * sizeof(uint16));
		rop->material = m_DebugMaterial;
		rop->topology = RenderOperation::T_LINE;
		m_RenderQueue->addRenderOperation(rop);
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

		m_IndexBuffer->writeData(sizeof(uint16), (byte*)&m_VertexCounter, m_VertexCounter * sizeof(uint16));
		++m_VertexCounter;
	}
	/*********************************************************************************/
	void DebugRenderer::clearRenderQueue()
	{
		m_RenderQueue->clear();
		m_BufferOffset = 0;
		m_VertexCounter = 0;
	}
/*********************************************************************************/
}}