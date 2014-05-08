/**
 * @file K15_SampleApplicationModule.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#include "K15_SampleApplicationModule.h"
#include "K15_LogManager.h"

#include "K15_InputManager.h"
#include "K15_Keyboard.h"
#include "K15_RendererBase.h"

#include "K15_VertexDeclaration.h"
#include "K15_RenderTask.h"
#include "K15_RenderSampleProcess.h"

#include "K15_GameObject.h"
#include "K15_Node.h"
#include "K15_CameraComponent.h"
#include "K15_ModelComponent.h"
#include "K15_Mesh.h"
#include "K15_SubMesh.h"
#include "K15_Material.h"
#include "K15_GpuProgramBatch.h"
#include "K15_IOUtil.h"
#include "K15_RenderOperation.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexBuffer.h"
#include "K15_Vertex.h"

#include "K15_GameEvent.h"

#include "K15_ResourceManager.h"
#include "K15_ResourceArchiveZip.h"
#include "K15_ResourceImporterTiff.h"
#include "K15_ResourceImporterObj.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	/*********************************************************************************/
	RenderTestApplicationModule::RenderTestApplicationModule()
	{

	}
	/*********************************************************************************/
	RenderTestApplicationModule::~RenderTestApplicationModule()
	{

	}
	/*********************************************************************************/
	void RenderTestApplicationModule::_onRendererInitialized()
	{
		g_ResourceManager->addResourceFile(K15_NEW ResourceArchiveZip(g_Application->getGameRootDir() + "resources.zip"));
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterTiff());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterObj());
		g_Application->getRenderTask()->setRenderProcess(K15_NEW K15_Engine::Plugins::RenderTest::RenderSampleProcess());

		m_Monster = K15_NEW GameObject("Monster");
		m_Monster->addComponent(K15_NEW ModelComponent("plane.obj"));

		m_Camera = K15_NEW GameObject("Camera");
		m_Camera->addComponent(K15_NEW CameraComponent());

		m_Crate = K15_NEW GameObject("Crate");
		m_Crate->addComponent(K15_NEW ModelComponent("cube.obj"));

// 		Mesh* mesh = K15_NEW Mesh();
// 		SubMesh* submesh = K15_NEW SubMesh(mesh);
// 
// 		float vbodata[] = {
// 			-1.0f,-1.0f,-1.0f,1.0f,
// 			0.0f,1.0f,-1.0f,1.0f,
// 			1.0f,-1.0f,-1.0f,1.0f
// 		};
// 
// 		uint16 indexdata[] = {
// 			0,1,2
// 		};
// 
// 		VertexBuffer::CreationOptions opts;
// 		opts.BufferType = GpuBuffer::BT_VERTEX_BUFFER;
// 		opts.InitialData.data = (byte*)vbodata;
// 		opts.InitialData.size = sizeof(vbodata);
// 
// 		IndexBuffer::CreationOptions ipts;
// 		ipts.BufferType = GpuBuffer::BT_INDEX_BUFFER;
// 		ipts.IndexType = IndexBuffer::IT_UINT16;
// 		ipts.InitialData.data = (byte*)indexdata;
// 		ipts.InitialData.size = sizeof(indexdata);
// 
// 		VertexBuffer* vbo = K15_NEW VertexBuffer(opts);
// 		IndexBuffer* ibo = K15_NEW IndexBuffer(ipts);
// 
// 		vbo->setVertexDeclaration(K15_NEW VertexDeclaration("PF4"));
// 
// 		submesh->setVertexBuffer(vbo);
// 		submesh->setIndexBUffer(ibo);
// 
// 		ModelComponent* mc = K15_NEW ModelComponent();
// 		mc->setMesh(mesh);
// 
// 		m_Crate->addComponent(mc);

		m_Monster->getNode()->translate(0.0f,0.0f,-2.0f);

		g_Application->getRenderer()->setActiveCameraGameObject(m_Camera);
		g_Application->getRenderer()->setClearColor(0.0f,0.0f,0.0f);

		m_Material = K15_NEW Material();
		MaterialPass* pass = m_Material->getPass(0,true);
		
		GpuProgram* vertexShader = K15_NEW GpuProgram("default_vertex",GpuProgram::PS_VERTEX);
		GpuProgram* fragmetShader = K15_NEW GpuProgram("default_fragment",GpuProgram::PS_FRAGMENT);

		String vertexShaderFile = g_Application->getGameRootDir() + "default.vert";
		String fragmentShaderFile = g_Application->getGameRootDir() + "default.frag";

		Texture* tex = g_ResourceManager->getResource<Texture>("Test_Image.tif");
		TextureSampler* sampler = K15_NEW TextureSampler();

		tex->setTextureSamplerSlot(Texture::TS_SLOT1);

		sampler->setUWrappingMode(TextureSampler::TWM_REPEAT);
		sampler->setMagFilterMode(TextureSampler::TFM_NEAREST);
		sampler->setMinFilterMode(TextureSampler::TFM_LINEAR);

		pass->setDiffuseMap(tex);
		pass->setDiffuseSampler(sampler);

		vertexShader->setProgramCode(IOUtil::readWholeFile(vertexShaderFile),false);
		fragmetShader->setProgramCode(IOUtil::readWholeFile(fragmentShaderFile),false);

		m_ProgramBatch = K15_NEW GpuProgramBatch();

		m_ProgramBatch->addGpuProgram(vertexShader);
		m_ProgramBatch->addGpuProgram(fragmetShader);

		m_ProgramBatch->compile();

		pass->setProgramBatch(m_ProgramBatch);

		pass->setFillMode(RendererBase::FM_SOLID);

		m_MonsterMaterial = K15_NEW Material();
		MaterialPass* mPass = m_MonsterMaterial->getPass(0,true);

		tex = g_ResourceManager->getResource<Texture>("monster.tif");
		tex->setTextureSamplerSlot(Texture::TS_SLOT1);
		
		mPass->setDiffuseMap(tex);
		mPass->setProgramBatch(m_ProgramBatch);

		mPass->setFillMode(RendererBase::FM_SOLID);

		AlphaState state;
		state.setBlendOperation(AlphaState::BO_ADD);
		state.setSourceBlendFunction(AlphaState::BF_SRC_ALPHA);
		state.setDestinationBlendFunction(AlphaState::BF_ONE_MINUS_SRC_ALPHA);

		mPass->setAlphaState(state);
		
		m_Crate->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->setMaterial(m_Material);
		m_Monster->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->setMaterial(m_MonsterMaterial);
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onPreTick()
	{
		if(m_RendererInitialized)
		{
			if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_UP))
			{
				m_Camera->getNode()->rotate(Vector3(1.0f,0.0f,0.0f),glm::radians(5.0f));
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_DOWN))
			{
				m_Camera->getNode()->rotate(Vector3(1.0f,0.0f,0.0f),glm::radians(-5.0f));
			}

			if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_LEFT))
			{
				m_Camera->getNode()->rotate(Vector3(0.0f,1.0f,0.0f),glm::radians(-5.0f));
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_RIGHT))
			{
				m_Camera->getNode()->rotate(Vector3(0.0f,1.0f,0.0f),glm::radians(5.0f));
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F1))
			{
				m_Camera->getComponentByType<CameraComponent>()->setProjectionType(CameraComponent::PT_ORTHOGRAPHIC);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F2))
			{
				m_Camera->getComponentByType<CameraComponent>()->setProjectionType(CameraComponent::PT_PERSPECTIVE);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F3))
			{
				_dumpMemoryStatistics();
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F4))
			{
				m_Camera->getComponentByType<CameraComponent>()->setZoom(0.5f);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F5))
			{
				m_Camera->getComponentByType<CameraComponent>()->setZoom(0.25f);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F6))
			{
				m_Camera->getComponentByType<CameraComponent>()->setZoom(1.0f);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F7))
			{
				m_Camera->getComponentByType<CameraComponent>()->setZoom(1.5f);
			}
			static const float gametime_threshold = 0.1f;
			static float gametime_counter = 0.0f;
			static float y = 0.0f;
			static float angle = 0.02f;

			angle += 0.02f;
			y = glm::sin(angle);

			m_Crate->getNode()->setPosition(Vector3(0.0f,0.0f,-4.0f));
			m_Crate->getNode()->rotate(Vector3(0.0f,1.0f,0.0f),0.02f);
			m_Crate->getNode()->translate(0.0f,y,0.0f);

			gametime_counter += g_Application->getGameTime().getDeltaTime();
			_LogDebug("%.3f gametime",gametime_counter);
			if(gametime_counter >= gametime_threshold)
			{
				Vector2 uv;
				gametime_counter = 0.0f;
				VertexBuffer* buffer = m_Monster->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getVertexBuffer();
				for(uint32 i = 0;i < buffer->getVertexCount();++i)
				{
					Vertex* vertex = buffer->getVertex(i);
					uv = vertex->getUV();
					uv.x += 0.25;
					vertex->setUV(uv);
				}
			}

			static RenderOperation rop[2];

			rop[0] = *m_Crate->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->createRenderOperation();
			rop[0].material = m_Material;
			rop[0].gameobject = m_Crate;
			rop[0].topology = RenderOperation::T_TRIANGLE;

			rop[1] = *m_Monster->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->createRenderOperation();
			rop[1].material = m_MonsterMaterial;
			rop[1].gameobject = m_Monster;
			rop[1].topology = RenderOperation::T_TRIANGLE;

			((RenderSampleProcess*)g_Application->getRenderTask()->getRenderProcess())->rop[0] = &rop[0];
			((RenderSampleProcess*)g_Application->getRenderTask()->getRenderProcess())->rop[1] = &rop[1];
		}
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onPostTick()
	{

	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onShutdown()
	{
		K15_DELETE g_Application->getRenderTask()->getRenderProcess();
		g_Application->getRenderTask()->setRenderProcess(0);
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::_dumpMemoryStatistics()
	{
		BaseAllocator* currentAllocator = 0;
		float totalSizeMB = 0.0f;
		float usedSizeMB = 0.0f;
		float percentage = 0.0f;

		for(uint32 i = 0;i < BaseAllocatedObject::AllocatorCount;++i)
		{
			currentAllocator = BaseAllocatedObject::Allocators.at(i);	
			totalSizeMB = (float)currentAllocator->getSize() / (float)MEGABYTE;
			usedSizeMB = (float)(currentAllocator->getSize() - currentAllocator->getFreeSize()) / (float)MEGABYTE;
			percentage = (usedSizeMB / totalSizeMB) * 100;
			_LogDebug("Dumping Allocator \"%s\":\nTotal size:\t%.2fMB\nMemory used:\t%.2fMB\nPercentage:\t%.4f%%\nParent:\t\t\"%s\"\n",
				currentAllocator->getName().c_str(),totalSizeMB,usedSizeMB,percentage,
				currentAllocator->getParentAllocator() ? currentAllocator->getParentAllocator()->getName().c_str() : "----");
			_LogDebug("===========================================================\n");
		}
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onMousePressed(const MouseActionArguments& p_EventArgs)
	{
		Vector4 translate(0.5f,0.0f,0.0f,0.0f);
		if(p_EventArgs.xNDC < -0.2f)
		{
			translate = -translate;
			translate = m_Camera->getNode()->getTransformation() * translate;
			m_Camera->getNode()->translate(translate.x,translate.y,translate.z);
		}else if(p_EventArgs.xNDC > 0.2f)
		{
			translate = m_Camera->getNode()->getTransformation() * translate;
			m_Camera->getNode()->translate(translate.x,translate.y,translate.z);
		}
		translate.x = 0.0f;
		translate.z = 0.5f;
		if(p_EventArgs.yNDC < -0.2f)
		{
			translate = m_Camera->getNode()->getTransformation() * translate;
			m_Camera->getNode()->translate(translate.x,translate.y,translate.z);
		}else if(p_EventArgs.yNDC > 0.2f)
		{
			translate = -translate;
			translate = m_Camera->getNode()->getTransformation() * translate;
			m_Camera->getNode()->translate(translate.x,translate.y,translate.z);
		}
	}
	/*********************************************************************************/

}}}// end of K15_Engine::Plugins::RenderTest namespace