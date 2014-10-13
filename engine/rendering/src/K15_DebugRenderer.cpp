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
#include "K15_GpuProgramCatalog.h"
#include "K15_Material.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	uint32 const DebugRenderer::DEBUG_VERTEX_CACHE = 1000;
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
		pass1->setProgram(GpuProgramCatalog::getGpuProgram("debug.vert", GpuProgram::PS_VERTEX), GpuProgram::PS_VERTEX);
		pass1->setProgram(GpuProgramCatalog::getGpuProgram("debug.frag", GpuProgram::PS_FRAGMENT), GpuProgram::PS_FRAGMENT);
		pass1->setFillMode(RendererBase::FM_WIREFRAME);
	}
	/*********************************************************************************/
	DebugRenderer::~DebugRenderer()
	{
		K15_DELETE m_IndexBuffer;
		K15_DELETE m_VertexBuffer;
		K15_DELETE m_RenderQueue;
		K15_DELETE m_DebugMaterial;
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
		float length = 0.5f;
		Vector3 points[10];
		uint32 bufferPos = m_BufferOffset;
		uint16 indices = m_VertexCounter;
		points[0] = p_Start;
		points[1] = points[2] = points[3] = points[4] = points[5] = points[6] = points[7] = points[8] = points[9] = p_End;
		points[3].y -= 1.f;
		points[5].y -= 1.f;
		points[7].y -= 1.f;
		points[9].y -= 1.f;
		
		points[3].x += MathUtil::cos(MathUtil::HalfPi); points[3].z -= MathUtil::sin(MathUtil::HalfPi); 
		points[5].x += MathUtil::cos(MathUtil::Pi);		points[5].z -= MathUtil::sin(MathUtil::Pi);
		points[7].x += MathUtil::cos(MathUtil::HalfPi + MathUtil::Pi); points[7].z -= MathUtil::sin(MathUtil::HalfPi + MathUtil::Pi); 
		points[9].x += MathUtil::cos(MathUtil::TwoPi);	points[9].z -= MathUtil::sin(MathUtil::TwoPi);

		for(int i = 0; i < 10; ++i)
		{
			points[i] = points[i] * length;
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
		/*RenderOperation* rop = K15_NEW RenderOperation();
		Vector3 points[] = {p_P1, p_P2};
		uint32 offset;

		for(int i = 0; i < 2; ++i)
		{
			_writeVertex(points[i], p_Color, &offset);
		}

		rop->topology = RenderOperation::T_TRIANGLE;
		rop->vertexData = K15_NEW VertexData(m_VertexDeclaration, m_VertexBuffer, m_BufferOffset, 2);
		rop->material = m_DebugMaterial;
		m_RenderQueue->addRenderOperation(rop);
		m_BufferOffset = offset;*/
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