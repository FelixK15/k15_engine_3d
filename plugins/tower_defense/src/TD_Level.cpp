#include "TD_Level.h"
#include "TD_RenderProcess.h"

#include "K15_Image.h"
#include "K15_ResourceManager.h"
#include "K15_GameObject.h"
#include "K15_Mesh.h"
#include "K15_SubMesh.h"
#include "K15_ModelComponent.h"
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_RendererBase.h"
#include "K15_Application.h"
#include "K15_GpuProgramBatch.h"
#include "K15_Material.h"
#include "K15_VertexDeclaration.h"
#include "K15_Node.h"
#include "K15_RenderOperation.h"

namespace TowerDefense
{
	/*********************************************************************************/
	Level::Level(const String& p_Name)
		: Object(p_Name)
	{
		_loadLevel(p_Name);
	}
	/*********************************************************************************/
	Level::~Level()
	{
		for(uint32 i = 0;i < m_GameObjects.size();++i)
		{
			K15_DELETE m_GameObjects.at(i);
		}
	}
	/*********************************************************************************/
	void Level::_loadLevel(const String& p_Name)
	{
		Image* layout = g_ResourceManager->getResource<Image>(p_Name + ".tif");
		GameObject* currentGameObject = 0;
		
		uint32 height = layout->getHeight() % 2 == 0 ? layout->getHeight() : layout->getHeight() + 1;
		uint32 width = layout->getWidth() % 2 == 0 ? layout->getWidth() : layout->getWidth() + 1;
		int32 height_half = height / 2;
		int32 width_half = width / 2;
		for(int y = -height_half; y < height_half;++y)
		{
			for(int x = -width_half;x < width_half;++x)
			{
				ColorRGBA color = layout->getPixel(x + width_half, y + height_half);
				currentGameObject = K15_NEW GameObject();
				currentGameObject->getNode()->translate(x,0,y);

				Mesh* modelMesh = 0;

				if(color == ColorRGBA::Yellow || color == ColorRGBA::Blue || color == ColorRGBA::Green || color == ColorRGBA::Red ||
				   color == ColorRGBA::Black)
				{
					if(color == ColorRGBA::Black)
					{
						//wall
						currentGameObject->setName("wall");
						currentGameObject->addComponent(K15_NEW ModelComponent("cube_wall.obj"));
						modelMesh = currentGameObject->getComponentByType<ModelComponent>()->getMesh();
						modelMesh->getSubMesh(0)->setMaterial(g_ResourceManager->getResource<Material>("wall.json"));
					}
					else
					{
						//ground
						currentGameObject->getNode()->translate(0.0f,-1.0f,0.0f);

						if(color == ColorRGBA::Green)
						{
							currentGameObject->addComponent(K15_NEW ModelComponent("cube_goal.obj"));
							modelMesh = currentGameObject->getComponentByType<ModelComponent>()->getMesh();
							modelMesh->getSubMeshes()[0]->setMaterial(g_ResourceManager->getResource<Material>("goal.json"));
						}
						else if(color == ColorRGBA::Red)
						{
							currentGameObject->addComponent(K15_NEW ModelComponent("cube_start.obj"));
							modelMesh = currentGameObject->getComponentByType<ModelComponent>()->getMesh();
							modelMesh->getSubMeshes()[0]->setMaterial(g_ResourceManager->getResource<Material>("start.json"));
						}
						else if(color == ColorRGBA::Yellow)
						{
							currentGameObject->addComponent(K15_NEW ModelComponent("cube_outer_floor.obj"));
							modelMesh = currentGameObject->getComponentByType<ModelComponent>()->getMesh();
							modelMesh->getSubMeshes()[0]->setMaterial(g_ResourceManager->getResource<Material>("outer_floor.json"));
						}
						else if(color == ColorRGBA::Blue)
						{
							currentGameObject->addComponent(K15_NEW ModelComponent("cube_inner_floor.obj"));
							modelMesh = currentGameObject->getComponentByType<ModelComponent>()->getMesh();
							modelMesh->getSubMeshes()[0]->setMaterial(g_ResourceManager->getResource<Material>("inner_floor.json"));
						}
					}
				}

				m_GameObjects.push_back(currentGameObject);
			}
		}
	}
	/*********************************************************************************/
	void Level::draw(RenderProcess* p_Process)
	{
		static DynamicArray(RenderOperation*) rops;
		GameObject* currentGameObject = 0;
		
		for(uint32 i = 0 ;i < m_GameObjects.size() - 1;++i)
		{
			currentGameObject = m_GameObjects.at(i);
			if(rops.size() < m_GameObjects.size())
			{
				RenderOperation* rop = K15_NEW RenderOperation();
				rop->gameobject = currentGameObject;
				rop->indexBuffer = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getIndexBuffer();
				rop->vertexBuffer = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getVertexBuffer();
				rop->material = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getMaterial();
				rop->topology = RenderOperation::T_TRIANGLE;
				rops.push_back(rop);
			}

			p_Process->addRop(rops.at(i));
		}
	}
	/*********************************************************************************/
}