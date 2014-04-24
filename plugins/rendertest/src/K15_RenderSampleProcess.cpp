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
#include "K15_GpuProgramBatch.h"
#include "K15_GpuProgramAutoParameter.h"
#include "K15_ResourceManager.h"
#include "K15_Texture.h"
#include "K15_Image.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	/*********************************************************************************/
	RawData UpdateDiffuseSampler(const GpuProgramParameter& p_Param, void* p_UserData)
	{
		Texture* texture = *(Texture**)p_UserData;
		Enum slot = texture->getSlot();

		return RawData((byte*)slot,sizeof(Enum));
	}
	/*********************************************************************************/

	/*********************************************************************************/
	RenderSampleProcess::RenderSampleProcess()
	{
		float vertexData[] = {
			-1.0f,-1.0f,0.0f,1.0f,
			0.0f,0.0f,

			-1.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,

			1.0f,-1.0f,0.0f,1.0f,
			1.0f,0.0f,

			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f
		};

		uint16 indexData[] = {
			0,1,2,3
		};
 
		GpuProgramAutoParameter::addAutoParameter("g_Diffuse",
			GpuProgramAutoParameter::AutoParameterSettings(UpdateDiffuseSampler,GpuProgramParameter::UF_PER_MATERIAL,&m_Texture));

 		m_Material = K15_NEW Material();
 		MaterialPass* pass = m_Material->getPass(0,true);
 
 		GpuProgram* vertexShader = K15_NEW GpuProgram("default_vertex",GpuProgram::PS_VERTEX);
 		GpuProgram* fragmetShader = K15_NEW GpuProgram("default_fragment",GpuProgram::PS_FRAGMENT);
 
		String vertexShaderFile = g_Application->getGameRootDir() + "default.vert";
		String fragmentShaderFile = g_Application->getGameRootDir() + "default.frag";

		vertexShader->setProgramCode(IOUtil::readWholeFile(vertexShaderFile),false);
		fragmetShader->setProgramCode(IOUtil::readWholeFile(fragmentShaderFile),false);

		m_ProgramBatch = K15_NEW GpuProgramBatch();

		m_ProgramBatch->addGpuProgram(vertexShader);
		m_ProgramBatch->addGpuProgram(fragmetShader);
		
		m_ProgramBatch->compile();

		m_VertexDeclaration = K15_NEW VertexDeclaration("PF4TF2");

		VertexBuffer::CreationOptions optionsVB;
		IndexBuffer::CreationOptions optionsIB;
		optionsVB.BufferType = GpuBuffer::BT_VERTEX_BUFFER;
		optionsVB.VertexLayout = m_VertexDeclaration;
		optionsVB.InitialData.data = (byte*)vertexData;
		optionsVB.InitialData.size = sizeof(vertexData);

		optionsIB.BufferType = GpuBuffer::BT_INDEX_BUFFER;
		optionsIB.IndexType = IndexBuffer::IT_UINT16;
		optionsIB.InitialData.data = (byte*)indexData;
		optionsIB.InitialData.size = sizeof(indexData);

		m_VertexBuffer = K15_NEW VertexBuffer(optionsVB);
		m_IndexBuffer = K15_NEW IndexBuffer(optionsIB);

		pass->setProgramBatch(m_ProgramBatch);
// 		pass->setProgram(vertexShader,GpuProgram::PS_VERTEX);
// 		pass->setProgram(fragmetShader,GpuProgram::PS_FRAGMENT);
		pass->setFillMode(RendererBase::FM_SOLID);
		pass->setCullingMode(RendererBase::CM_CW);
		

		m_Texture = (Texture*)g_ResourceManager->getResource<Texture>("Test_Image.tif",0);
		
		m_Sampler = K15_NEW TextureSampler();

		m_Sampler->setMagFilterMode(TextureSampler::TFM_LINEAR);
		m_Sampler->setMinFilterMode(TextureSampler::TFM_LINEAR);

		pass->setDiffuseMap(m_Texture);
		pass->setDiffuseSampler(m_Sampler);

		m_Rop = K15_NEW RenderOperation();

		m_Rop->vertexBuffer = m_VertexBuffer;
		m_Rop->indexBuffer = m_IndexBuffer;
		m_Rop->material = m_Material;
		m_Rop->topology = RenderOperation::T_TRIANGLE_STRIP;
	}
	/*********************************************************************************/
	RenderSampleProcess::~RenderSampleProcess()
	{
		K15_DELETE m_Rop;
		K15_DELETE m_Material;
		K15_DELETE m_ProgramBatch;
		K15_DELETE m_VertexBuffer;
		K15_DELETE m_IndexBuffer;
		K15_DELETE m_VertexDeclaration;
		K15_DELETE m_Texture;
	}
	/*********************************************************************************/
	void RenderSampleProcess::renderSingleFrame()
	{
		g_Application->getRenderer()->draw(m_Rop);
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Plugins::RenderTest namespace