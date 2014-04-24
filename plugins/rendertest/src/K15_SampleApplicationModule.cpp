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
#include "K15_Vertex.h"
#include "K15_Vector4.h"

#include "K15_VertexDeclaration.h"
#include "K15_RenderTask.h"
#include "K15_RenderSampleProcess.h"

#include "K15_ResourceManager.h"
#include "K15_ZipResourceArchive.h"
#include "K15_TiffResourceImporter.h"

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
	void RenderTestApplicationModule::onRendererInitialized()
	{
		g_ResourceManager->addResourceFile(K15_NEW ZipResourceArchive(g_Application->getGameRootDir() + "resources.zip"));
		g_ResourceManager->addResourceImporter(K15_NEW TiffResourceImporter());
		g_Application->getRenderTask()->setRenderProcess(K15_NEW K15_Engine::Plugins::RenderTest::RenderSampleProcess());
		//_dumpMemoryStatistics();
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onPreTick()
	{
		if(g_InputManager->isActive(_ON(Color_1)))
		{
			g_Application->getRenderer()->setClearColor(1.0f,0.0f,0.0f);
		}
		else if(g_InputManager->isActive(_ON(Color_2)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,1.0f,0.0f);
		}
		else if(g_InputManager->isActive(_ON(Color_3)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,0.0f,1.0f);
		}
		else if(g_InputManager->isActive(_ON(Color_4)))
		{
			g_Application->getRenderer()->setClearColor(1.0f,1.0f,0.0f);
		}
		else if(g_InputManager->isActive(_ON(Color_5)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,1.0f,1.0f);
		}


		if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F1))
		{
			_dumpMemoryStatistics();
		}

		if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F2))
		{
			g_Application->getRenderWindow()->setIsFullscreen(true);
		}
		else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_F3))
		{
			g_Application->getRenderWindow()->setIsFullscreen(false);
		}

		if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_UP))
		{
			VertexBuffer* buff = (VertexBuffer*)g_Application->getRenderer()->getBoundBuffer(GpuBuffer::BT_VERTEX_BUFFER);
			Vertex* vertex = buff->getVertex(1);

			Math::Vector4 pos = vertex->getPosition();
			ColorRGBA color = vertex->getColor();
			pos.y += 0.02f;
			color.GreenComponent += 0.02f;
			vertex->setPosition(pos);
			vertex->setColor(color);
		}
		else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_DOWN))
		{
			VertexBuffer* buff = (VertexBuffer*)g_Application->getRenderer()->getBoundBuffer(GpuBuffer::BT_VERTEX_BUFFER);
			Vertex* vertex = buff->getVertex(1);

			Math::Vector4 pos = vertex->getPosition();
			ColorRGBA color = vertex->getColor();
			pos.y -= 0.02f;
			color.GreenComponent -= 0.02f;
			vertex->setPosition(pos);
			vertex->setColor(color);
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

		for(uint32 i = 0;i < BaseAllocatedObject::Allocators.size();++i)
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
}}}// end of K15_Engine::Plugins::RenderTest namespace