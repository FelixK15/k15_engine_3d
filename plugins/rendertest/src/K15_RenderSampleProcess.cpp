/**
 * @file K15_RenderSampleProcess.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/09
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

#include "K15_RenderSampleProcess.h"

#include "K15_Material.h"
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"
#include "K15_IOUtil.h"
#include "K15_RenderOperation.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	/*********************************************************************************/
	RenderSampleProcess::RenderSampleProcess()
	{
		float vertexData[] = {
			-1.0f,-1.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			1.0f,-1.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
		};

		uint16 indexData[] = {
			0,1,2
		};

		m_Material = K15_NEW Material();
		MaterialPass* pass = m_Material->getPass(0,true);
		m_VertexShader = K15_NEW GpuProgram(GpuProgram::PS_VERTEX);
		m_FragmetShader = K15_NEW GpuProgram(GpuProgram::PS_FRAGMENT);
		m_VertexBuffer = K15_NEW VertexBuffer();
		m_IndexBuffer = K15_NEW IndexBuffer();
		m_VertexDeclaration = K15_NEW VertexDeclaration("PF4CF4");

		m_IndexBuffer->setIndexType(IndexBuffer::IT_UINT16);

		m_VertexBuffer->setVertexCount(3);
		m_VertexBuffer->setVertexSize(sizeof(vertexData) / 3);

		m_VertexBuffer->writeData(sizeof(vertexData),(byte*)vertexData);
		m_IndexBuffer->writeData(sizeof(indexData),(byte*)indexData);

		String vertexShaderCode = IOUtil::readWholeFile("default.vert");
		String fragmentShaderCode = IOUtil::readWholeFile("default.frag");

		m_VertexShader->setProgramCode(vertexShaderCode,true);
		m_FragmetShader->setProgramCode(fragmentShaderCode,true);

		pass->setProgram(m_VertexShader,GpuProgram::PS_VERTEX);
		pass->setProgram(m_FragmetShader,GpuProgram::PS_FRAGMENT);

		pass->setFillMode(RendererBase::FM_SOLID);
		pass->setCullingMode(RendererBase::CM_CW);

		m_Rop = K15_NEW RenderOperation();

		m_Rop->vertexBuffer = m_VertexBuffer;
		m_Rop->indexBuffer = m_IndexBuffer;
		m_Rop->material = m_Material;
		m_Rop->topology = RenderOperation::T_TRIANGLE;
		m_Rop->vertexDeclaration = m_VertexDeclaration;
	}
	/*********************************************************************************/
	RenderSampleProcess::~RenderSampleProcess()
	{
		K15_DELETE m_Rop;
		K15_DELETE m_Material;
		K15_DELETE m_VertexShader;
		K15_DELETE m_FragmetShader;
		K15_DELETE m_VertexBuffer;
		K15_DELETE m_IndexBuffer;
		K15_DELETE m_VertexDeclaration;
	}
	/*********************************************************************************/
	void RenderSampleProcess::renderSingleFrame()
	{
		g_Application->getRenderer()->draw(m_Rop);
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Plugins::RenderTest namespace