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
#include "K15_RenderQueue.h"

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
		
		uint32 height = layout->getHeight() % 2 != 0 ? layout->getHeight() + 1 : layout->getHeight();
		uint32 width = layout->getWidth() % 2 != 0 ? layout->getWidth() + 1 : layout->getWidth();
		int32 height_half = height / 2;
		int32 width_half = width / 2;

		GameObject* plane = _createPlane(height_half,width_half);
		plane->getNode().translate(0,-.5f,0.0f);
		m_GameObjects.push_back(plane);
		for(int y = -height_half; y < height_half;++y)
		{
			for(int x = -width_half;x < width_half;++x)
			{
				ColorRGBA color = layout->getPixel(x + width_half, y + height_half);
				Mesh* modelMesh = 0;

				if(color == ColorRGBA::Yellow)
				{
					continue;
				}
				else if(color == ColorRGBA::Blue || color == ColorRGBA::Green ||
					color == ColorRGBA::Red || color == ColorRGBA::Black)
				{
					currentGameObject = K15_NEW GameObject();
					currentGameObject->getNode().translate((float)x,0,(float)y);

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
						currentGameObject->getNode().translate(0.0f,-1.0f,0.0f);

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
		static RenderQueue* rops = K15_NEW RenderQueue();
		GameObject* currentGameObject = 0;
		
		for(uint32 i = 0 ;i < m_GameObjects.size() - 1;++i)
		{
			currentGameObject = m_GameObjects.at(i);
			if(rops->size() < m_GameObjects.size())
			{
				RenderOperation* rop = K15_NEW RenderOperation();
				rop->gameobject = currentGameObject;
				rop->indexBuffer = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getIndexBuffer();
				rop->vertexBuffer = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getVertexBuffer();
				rop->material = currentGameObject->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getMaterial();
				rop->topology = RenderOperation::T_TRIANGLE;
				rops->addRenderOperation(rop);
			}

			p_Process->setRenderQueue(rops);
		}
	}
	/*********************************************************************************/
	GameObject* Level::_createPlane(uint32 p_HalfHeight, uint32 p_HalfWidth)
	{
		GameObject* plane = K15_NEW GameObject("plane");
		float left = (float)p_HalfWidth * -1.f;
		float right = (float)p_HalfWidth;
		float top = (float)p_HalfHeight;
		float bottom = (float)p_HalfHeight * -1.f;

		float vertexData[] = {
			left, 0.0f, bottom, 1.0f,
			0.0f, 0.0f,

			left, 0.0f, top, 1.0f,
			0.0f, top * 2.f,

			right, 0.0f, top, 1.0f,
			right * 2.f, top * 2.f,

			right, 0.0f, bottom, 1.0f,
			right * 2.f, 0.0f
		};


		uint16 indexData[] = {
			0,3,2,
			2,1,0
		};

		VertexBuffer::CreationOptions vbo_opts;
		IndexBuffer::CreationOptions ibo_opts;

		ibo_opts.InitialData.data = (byte*)indexData;
		ibo_opts.InitialData.size = sizeof(indexData);
		ibo_opts.IndexType = IndexBuffer::IT_UINT16;

		vbo_opts.InitialData.data = (byte*)vertexData;
		vbo_opts.InitialData.size = sizeof(vertexData);

		vbo_opts.VertexLayout = VertexDeclaration::create("PF4TF2");
		
		VertexBuffer* vbo = K15_NEW VertexBuffer(vbo_opts);
		IndexBuffer* ibo = K15_NEW IndexBuffer(ibo_opts);

		Material* material = g_ResourceManager->getResource<Material>("outer_floor.json");
		DepthState& state = material->getPass(0)->getDepthState();
		state.setBias(1.0f);
		material->getPass(0)->setDepthState(state);
		Mesh* planeMesh = K15_NEW Mesh();
		SubMesh* planeSubMesh = K15_NEW SubMesh(planeMesh);

		planeSubMesh->setVertexBuffer(vbo);
		planeSubMesh->setIndexBuffer(ibo);
		planeSubMesh->setMaterial(material);

		planeMesh->addSubMesh(planeSubMesh);

		plane->addComponent(K15_NEW ModelComponent(planeMesh));

		return plane;
	}
	/*********************************************************************************/
}