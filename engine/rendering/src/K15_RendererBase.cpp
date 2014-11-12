/**
 * @file K15_RendererBase.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
#include "K15_RendererBase.h"
#include "K15_RenderWindow.h"
#include "K15_EventHandler.h"
#include "K15_EnumStrings.h"

#include "K15_GpuProgramCatalog.h"
#include "K15_GpuProgramParameter.h"
#include "K15_GpuProgram.h"
#include "K15_GpuProgramBatch.h"
#include "K15_GpuBuffer.h"

#include "K15_Vertex.h"
#include "K15_VertexData.h"
#include "K15_IndexData.h"
#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"
#include "K15_VertexDeclaration.h"

#include "K15_RawData.h"
#include "K15_ResourceManager.h"
#include "K15_Font.h"

#include "K15_GameObject.h"
#include "K15_NodeComponent.h"

#include "K15_CameraComponent.h"

#include "K15_Material.h"

#include "K15_RenderOperation.h"
#include "K15_RenderQueue.h"

#include "K15_LightComponent.h"

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	uint8 RendererBase::PixelFormatSize[PF_COUNT]= {
		8,	//PF_R_8_I
		8,  //PF_R_8_UI,
		16, //PF_RG_16_I
		16, //PF_RG_16_U
		16, //PF_RG_16_F
		96, //PF_RGB_32_I
		96, //PF_RGB_32_U
		96, //PF_RGB_32_F
		32, //PF_RGBA_8_I
		32, //PF_RGBA_8_U
		64, //PF_RGBA_16_I
		64, //PF_RGBA_16_U
		64, //PF_RGBA_16_F
		128, //PF_RGBA_32_I
		128, //PF_RGBA_32_U
		128  //PF_RGBA_32_F
	}; //PixelFormatSize
	/*********************************************************************************/
	uint8 RendererBase::DepthFormatSize[DBF_COUNT] = {
		16, //DBF_COMPONENT_16_I
		24, //DBF_COMPONENT_24_I
		32, //DBF_COMPONENT_32_I
		32  //DBF_COMPONENT_32_F
	}; //DepthFormatSize
	/*********************************************************************************/
	uint8 RendererBase::StencilFormatSize[SBF_COUNT] = {
		1, //SBF_COMPONENT_1_I
		4, //SBF_COMPONENT_4_I
		8, //SBF_COMPONENT_8_I
		16 //SBF_COMPONENT_16_I
	}; //StencilFormatSize
	/*********************************************************************************/

	/*********************************************************************************/
	RendererBase::RendererBase()
		: m_ActiveCamera(0),
		m_BackFaceCullingEnabled(true),
		m_ClearColor(ColorRGBA::DarkBlue),
		m_CullingMode(CM_CCW),
		m_DefaultRenderTarget(0),
		m_DepthBufferFormat(DBF_COMPONENT_24_I),
		m_FrameBufferFormat(PF_RGBA_8_I),
		m_LightningEnabled(true),
		m_Initialized(false),
		m_RenderTarget(0),
		m_VertexDeclaration(0),
		m_LastError(),
		m_Material(0),
		m_GpuBuffers(),
		m_GpuPrograms(),
		m_StencilBufferFormat(SBF_COMPONENT_8_I),
		m_Topology(RenderOperation::T_TRIANGLE),
		m_GpuParameterUpdateMask(0),
		m_AmbientColor(ColorRGBA::Black)
	{
		for(int i = 0;i < GpuBuffer::BT_COUNT;++i)
		{
			m_GpuBuffers[i] = 0;
		}

		for(int i = 0;i < GpuProgram::PS_COUNT;++i)
		{
			m_GpuPrograms[i] = 0;
		}

		g_EventManager->addHandler(RenderWindow::EventResolutionChanged,K15_EventHandler(RendererBase,onEventResolutionChanged,this));
		g_EventManager->addHandler(RenderWindow::EventInitialized,K15_EventHandler(RendererBase,onEventRenderWindowInitialized,this));
	}
	/*********************************************************************************/
	RendererBase::~RendererBase()
	{

	}
	/*********************************************************************************/
	bool RendererBase::initialize()
	{
		if(!m_Initialized)
		{
			m_Initialized = _initialize();
		}
		return m_Initialized;
	}
	/*********************************************************************************/
	void RendererBase::shutdown()
	{
		_shutdown();
		m_Initialized = false;
	}
	/*********************************************************************************/
	bool RendererBase::setRenderTarget(RenderTarget* p_RenderTarget)
	{
		K15_ASSERT(p_RenderTarget,"Render Target is NULL.");

		if(m_RenderTarget != p_RenderTarget)
		{
			m_RenderTarget = p_RenderTarget;
			_setRenderTarget(m_RenderTarget);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting render target. %s",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setActiveCamera(CameraComponent* p_Camera)
	{
		K15_ASSERT(p_Camera,"Camera is NULL.");

		if(m_ActiveCamera != p_Camera)
		{
			m_ActiveCamera = p_Camera;
			_setActiveCamera(m_ActiveCamera);

			m_GpuParameterUpdateMask |= GpuProgramParameter::UF_PER_CAMERA;

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting active camera. %s",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setCullingMode(Enum p_CullingMode)
	{
		K15_ASSERT(p_CullingMode < CM_COUNT,StringUtil::format("Invalid culling mode \"%u\"",p_CullingMode));

		if(m_CullingMode != p_CullingMode)
		{
			m_CullingMode = p_CullingMode;
			_setCullingMode(m_CullingMode);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting culling mode to %s. %s",eCullingModeStr[p_CullingMode],getLastError().c_str());
				return false;
			}
		}
		
		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setTopology(Enum p_Topology)
	{
		K15_ASSERT(p_Topology < RenderOperation::T_COUNT,StringUtil::format("Invalid topology \"%u\"",p_Topology));

		if(p_Topology != m_Topology)
		{
			m_Topology = p_Topology;
			_setTopology(m_Topology);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting topology to %s. %s",eTopologyStr[p_Topology],getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setFillMode(Enum p_FillMode)
	{
		K15_ASSERT(p_FillMode < RendererBase::FM_COUNT,StringUtil::format("Invalid fillmode \"%u\"",p_FillMode));

		if(p_FillMode != m_FillMode)
		{
			m_FillMode = p_FillMode;
			_setFillMode(m_FillMode);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting fill mode to %s. %s",eFillModeStr[p_FillMode],getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setBackFaceCullingEnabled(bool p_Enabled)
	{
		if(p_Enabled != m_BackFaceCullingEnabled)
		{
			m_BackFaceCullingEnabled = p_Enabled;
			_setBackFaceCullingEnabled(p_Enabled);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error %s backface culling. %s.",p_Enabled ? "enabling" : "disabling",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setFrameBufferPixelFormat(Enum p_PixelFormat)
	{
		K15_ASSERT(p_PixelFormat < PF_COUNT,StringUtil::format("Invalid frame buffer format \"%d\"",p_PixelFormat));

		if(m_FrameBufferFormat != p_PixelFormat)
		{
			m_FrameBufferFormat = p_PixelFormat;
			_setFrameBufferPixelFormat(m_FrameBufferFormat);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting frame buffer pixelformat. %s",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setDepthBufferFormat(Enum p_DepthFormat)
	{
		K15_ASSERT(p_DepthFormat < DBF_COUNT,StringUtil::format("Invalid depth buffer format \"%u\"",p_DepthFormat));

		if(m_DepthBufferFormat != p_DepthFormat)
		{
			m_DepthBufferFormat = p_DepthFormat;
			_setDepthBufferPixelFormat(m_DepthBufferFormat);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting depth buffer format. %s",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setStencilBufferFormat(Enum p_StencilFormat)
	{
		K15_ASSERT(p_StencilFormat < SBF_COUNT,StringUtil::format("Invalid stencil buffer format \"%u\"",p_StencilFormat));

		if(m_StencilBufferFormat != p_StencilFormat)
		{
			m_StencilBufferFormat = p_StencilFormat;
			_setStencilBufferPixelFormat(m_StencilBufferFormat);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting stencil buffer format. %s",getLastError().c_str());
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setClearColor(const ColorRGBA& p_ClearColor)
	{
		if(m_ClearColor != p_ClearColor)
		{
			m_ClearColor = p_ClearColor;
			_setClearColor(m_ClearColor);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting clear color to %d %d %d %d (RGBA). %s",
					p_ClearColor.R,p_ClearColor.G,p_ClearColor.B,p_ClearColor.A,
					getLastError().c_str());

				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setClearColor(byte p_Red, byte p_Green, byte p_Blue)
	{
		return setClearColor(ColorRGBA(p_Red,p_Green,p_Blue));
	}
	/*********************************************************************************/
	bool RendererBase::bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage)
	{
		K15_ASSERT(p_Stage < GpuProgram::PS_COUNT,StringUtil::format("Invalid GpuProgram stage. %u",p_Stage));

		if(m_GpuPrograms[p_Stage] != p_GpuProgram && p_GpuProgram)
		{
			if(m_GpuPrograms[p_Stage]) m_GpuPrograms[p_Stage]->setIsBound(false);

			m_GpuPrograms[p_Stage] = p_GpuProgram;
			_bindProgram(p_GpuProgram,p_Stage);
			p_GpuProgram->setIsBound(true);

			//After a gpu program has been bound, we need to reset the vertex declaration
			if(m_VertexDeclaration)
			{
				_setVertexDeclaration(m_VertexDeclaration);
			}
			
			m_GpuParameterUpdateMask = GpuProgramParameter::UF_ALL;

			if(errorOccured())
			{
				K15_LOG_ERROR("Error binding gpu program for program stage \"%s\". %s",
					p_GpuProgram->getName().c_str(),
					eProgramStageStr[p_Stage],
					getLastError().c_str());

				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType)
	{
		K15_ASSERT(p_BufferType < GpuBuffer::BT_COUNT,
			StringUtil::format("Invalid GpuBuffer type. %u",
			p_BufferType));

		if(p_Buffer)
		{
			if(m_GpuBuffers[p_BufferType] != p_Buffer)
			{
				//undbind old buffer
				if(m_GpuBuffers[p_BufferType]) m_GpuBuffers[p_BufferType]->setIsBound(false);

				m_GpuBuffers[p_BufferType] = p_Buffer;
				_bindBuffer(p_Buffer,p_BufferType);
				p_Buffer->setIsBound(true);

        if(p_Buffer->isDirty())
        {
          p_Buffer->uploadShadowBufferToGpu();
        }

				if(errorOccured())
				{
					K15_LOG_ERROR("Error during buffer binding. Buffer type \"%s\". %s",
						eBufferTypeStr[p_BufferType],
						getLastError().c_str());

					return false;
				}
			}
		}

		return true;
	}
	/*********************************************************************************/
	GpuBuffer* RendererBase::getBoundBuffer(Enum p_BufferType) const
	{
		K15_ASSERT(p_BufferType >= 0 && p_BufferType <= GpuBuffer::BT_COUNT,
			StringUtil::format("Invalid GpuBuffer type. %u",p_BufferType));

		return m_GpuBuffers[p_BufferType];
		
	}
	/*********************************************************************************/
	bool RendererBase::draw(RenderQueue* p_RenderQueue)
	{
		for(uint32 i = 0; i < p_RenderQueue->size(); ++i)
		{
			if(!draw(p_RenderQueue->getRenderOperation(i)))
			{
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::draw(RenderOperation* p_Rop)
	{
		K15_ASSERT(p_Rop, "RenderOperation is NULL.");
		K15_ASSERT(p_Rop->vertexData, "RenderOperation has no vertex data.");
//		K15_ASSERT(p_Rop->indexData, "RenderOperation has no index data.");

		m_GpuParameterUpdateMask |= GpuProgramParameter::UF_PER_OBJECT;

		if(m_ActiveCamera)
		{
			if(m_ActiveCamera->isProjectionMatrixDirty() || 
				m_ActiveCamera->isViewMatrixDirty())
			{
				m_GpuParameterUpdateMask |= GpuProgramParameter::UF_PER_CAMERA;
			}
		}

		if(!bindBuffer(p_Rop->vertexData->getVertexBuffer(), GpuBuffer::BT_VERTEX_BUFFER) ||
		   !bindMaterial(p_Rop->material) || !setTopology(p_Rop->topology))
		{
			return false;
		}

		if(p_Rop->indexData)
		{
			if(!bindBuffer(p_Rop->indexData->getIndexBuffer(), GpuBuffer::BT_INDEX_BUFFER))
			{
				return false;
			}
		}
		
		MaterialPass* pass = 0;
		for(uint32 i = 0;i < m_Material->getAmountPasses();++i)
		{
			pass = 	m_Material->getPass(i);
			if(pass->isEnabled())
			{
				//if the pass has a program batch, bind it first - otherwise use separate programs
				if(pass->getProgramBatch())
				{
					if(!bindGpuProgramBatch(pass->getProgramBatch())) 
					{
						continue;
					}
				}
				else
				{
					if(!bindGpuProgram(pass->getProgram(GpuProgram::PS_VERTEX),GpuProgram::PS_VERTEX) ||
						!bindGpuProgram(pass->getProgram(GpuProgram::PS_FRAGMENT),GpuProgram::PS_FRAGMENT)) 
					{
						continue;
					}
				}

				if(	!setCullingMode(pass->getCullingMode()) ||
					!setFillMode(pass->getFillMode()) ||
					!setAlphaState(pass->getAlphaState()) ||
					!setDepthState(pass->getDepthState()))
				{
					continue;
				}

				setLightningEnabled(pass->isLightningEnabled());

				//bind textures
				if(!bindTexture(pass->getDiffuseMap(),Texture::TT_2D,Texture::TS_SLOT1))
				{
					continue;
				}

				if(pass->getDiffuseSampler())
				{
					if(!bindTextureSampler(pass->getDiffuseSampler(),Texture::TS_SLOT1))
					{
						continue;
					}
				}

				if(!setVertexDeclaration(p_Rop->vertexData->getVertexDeclaration()))
				{
					continue;
				}

				updateGpuProgramParameter(p_Rop);

				if(p_Rop->indexData != 0)
				{
					_drawIndexed(p_Rop->indexData->getIndexCount(), 
					p_Rop->indexData->getOffsetInBytes());
				}
				else
				{
					_drawDirect(p_Rop->vertexData->getVertexCount(),
					p_Rop->vertexData->getOffsetInBytes());
				}

				if(errorOccured())
				{
					K15_LOG_ERROR_NS("Error during draw. Material: %s (pass %u) %s",
						m_Material->getName().c_str(),i+1,getLastError().c_str());
					continue;
				}
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::bindMaterial(Material* p_Material)
	{
		K15_ASSERT(p_Material,"Material is NULL.");

#if defined K15_DEBUG
		MaterialPass* pass = 0;
		for(uint32 i = 0;i < p_Material->getAmountPasses();++i)
		{
			pass = p_Material->getPass(i);
			if(pass->isEnabled())
			{
				K15_ASSERT(pass,StringUtil::format("%u. pass of material %s is NULL.",i,p_Material->getName().c_str()));

				K15_ASSERT(pass->getProgramBatch() || (pass->getProgram(GpuProgram::PS_VERTEX) && pass->getProgram(GpuProgram::PS_FRAGMENT)),
					StringUtil::format("Pass %u (%s) from material %s has neither a gpu program batch nor separate gpu programs attached.",
					i,pass->getName().c_str(),p_Material->getName().c_str()));
			}
		}
#endif //K15_DEBUG
		
		if(m_Material != p_Material)
		{
			m_Material = p_Material;
			m_GpuParameterUpdateMask |= GpuProgramParameter::UF_PER_MATERIAL;
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::setAlphaState(const AlphaState& p_AlphaState)
	{
		if(p_AlphaState != m_AlphaState)
		{
			m_AlphaState = p_AlphaState;
			_setAlphaState(p_AlphaState);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting alpha state. %s",getLastError().c_str());
				return false;
			}
		}

    return true;
	}
	/*********************************************************************************/
	bool RendererBase::setDepthState(const DepthState& p_DepthState)
	{
		if(p_DepthState != m_DepthState)
		{
			m_DepthState = p_DepthState;
			_setDepthState(p_DepthState);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting depth state. %s",getLastError().c_str());
				return false;
			}
    }

    return true;
	}
	/*********************************************************************************/
	bool RendererBase::setVertexDeclaration(VertexDeclaration* p_Declaration)
	{
		K15_ASSERT(p_Declaration, "VertexDeclaration is NULL!");

		if(m_VertexDeclaration != p_Declaration)
		{
			if(m_VertexDeclaration) m_VertexDeclaration->setIsBound(false);

			m_VertexDeclaration = p_Declaration;
			_setVertexDeclaration(p_Declaration);
			p_Declaration->setIsBound(true);

			if(errorOccured())
			{
				K15_LOG_ERROR("Error setting vertex declaration \"%s\".%s",
					p_Declaration->getDeclarationNameAsString().c_str(),getLastError().c_str());

				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::bindTexture(Texture* p_Texture, Enum p_Type, Enum p_Slot)
	{
		K15_ASSERT(p_Type >= 0 && p_Type < Texture::TT_COUNT,"Invalid texture type.");
		K15_ASSERT(p_Slot >= 0 && p_Slot < Texture::TS_NO_SLOT,"Invalid texture slot.");

		if(p_Texture != m_BoundTextures[p_Slot])
		{
			if(m_BoundTextures[p_Slot])
			{
				m_BoundTextures[p_Slot]->setSlot(Texture::TS_NO_SLOT);
				m_BoundTextures[p_Slot]->setIsBound(false);
			}
			
			if(p_Texture)
			{
				m_BoundTextures[p_Slot] = p_Texture;
				p_Texture->setSlot(p_Slot);
			}

			_bindTexture(p_Texture,p_Type);

			if(p_Texture)
			{
				p_Texture->setIsBound(true);

				if(p_Texture->getTextureSamplerSlot() != Texture::TS_NO_SLOT)
				{
					K15_LOG_WARNING("Texture uses sampler in slot %u, but theres currently no sampler bounds to this slot.",
						p_Slot);
				}
			}

			if(errorOccured())
			{
				K15_LOG_ERROR("Error binding texture \"%s\".%s",
					p_Texture ? p_Texture->getName().c_str() : "",getLastError().c_str());

				return false;
			}

			return true;
		}

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::bindTextureSampler(TextureSampler* p_Sampler, Enum p_Slot)
	{
		K15_ASSERT(p_Slot >= 0 && p_Slot < Texture::TS_NO_SLOT,"Invalid texture slot.");

		if(p_Sampler != m_BoundSamplers[p_Slot])
		{
			if(m_BoundSamplers[p_Slot])
			{
				m_BoundSamplers[p_Slot]->setSlot(Texture::TS_NO_SLOT);
				m_BoundSamplers[p_Slot]->setIsBound(false);
			}

			if(p_Sampler)
			{
				m_BoundSamplers[p_Slot] = p_Sampler;
				p_Sampler->setSlot(p_Slot);
			}

			_bindTextureSampler(p_Sampler,p_Slot);
			
			if(p_Sampler)
			{
				p_Sampler->setIsBound(true);
			}

			if(errorOccured())
			{
				K15_LOG_ERROR("Error binding texture sampler.%s",getLastError().c_str());

				return false;
			}

			return true;
		}

		return true;
	}	
	/*********************************************************************************/
	bool RendererBase::bindGpuProgramBatch(GpuProgramBatch* p_GpuProgramBatch)
	{
		K15_ASSERT(p_GpuProgramBatch,"GpuProgramBatch is NULL.");

		if(m_GpuProgramBatch != p_GpuProgramBatch)
		{
			if(m_GpuProgramBatch) m_GpuProgramBatch->setIsBound(false);

			m_GpuProgramBatch = p_GpuProgramBatch;
			_bindProgramBatch(m_GpuProgramBatch);

			m_GpuProgramBatch->setIsBound(true);

			for(GpuProgramBatch::GpuProgramList::const_iterator iter = m_GpuProgramBatch->getGpuProgramList().begin();
				iter != m_GpuProgramBatch->getGpuProgramList().end();++iter)
			{
				bindGpuProgram((*iter),(*iter)->getStage());
			}

			if(errorOccured())
			{
				K15_LOG_ERROR("Error binding GpuProgramBatch. Error:\"%s\".",getLastError().c_str());

				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	void RendererBase::updateGpuProgramParameter(RenderOperation* p_Rop)
	{
		bool lightsHaveBeenUpdated = false;
		GpuProgram* program = 0;
		for(GpuProgramArray::iterator iter = m_GpuPrograms.begin();iter != m_GpuPrograms.end();++iter)
		{
			program = (*iter);

			if(program)
			{
				for(uint32 i = 0;i < program->getAmountUniforms();++i)
				{
					GpuProgramParameter& param = program->getUniform(i);
					
					if(param.isAutoParameter())
					{
						if((m_GpuParameterUpdateMask & param.getUpdateFrequency()) != 0)
						{
							if(param.getIdentifier() == GpuProgramParameter::PI_VIEW_MATRIX ||
								param.getIdentifier() == GpuProgramParameter::PI_PROJECTION_MATRIX ||
								param.getIdentifier() == GpuProgramParameter::PI_VIEW_PROJECTION_MATRIX)
							{ 
								CameraComponent* p_Camera = getActiveCamera();

								K15_ASSERT(p_Camera,
								StringUtil::format("Trying to set view/projection matrix in shader \"%s\", but there's no active camera to get it from.",
								program->getName().c_str()));

								Matrix4 mat;

								if(param.getIdentifier() == GpuProgramParameter::PI_VIEW_MATRIX)
								{
									mat = p_Camera->getViewMatrix();
								}
								else if(param.getIdentifier() == GpuProgramParameter::PI_PROJECTION_MATRIX)
								{
									mat = p_Camera->getProjectionMatrix();
								}
								else
								{
									mat = p_Camera->getProjectionMatrix();
									mat *= p_Camera->getViewMatrix();
								}

								param.setData((void*)&mat); 
							}
							else if(param.getIdentifier() == GpuProgramParameter::PI_NORMAL_MATRIX && p_Rop)
							{
								CameraComponent* p_Camera = getActiveCamera();

								K15_ASSERT(p_Camera,
									StringUtil::format("Trying to set normal matrix in shader \"%s\", but there's no active camera to get it from.",
									program->getName().c_str()));

								Matrix4 modelViewMatrix = p_Camera->getViewMatrix();

								GameObject* gameObject = 0;
								if((gameObject = p_Rop->gameobject) != 0)
								{
									modelViewMatrix *= gameObject->getTransformation();	
								}

								Matrix4 normalMatrix = modelViewMatrix;

								normalMatrix.inverse();
								normalMatrix.transpose();

								param.setData((void*)&normalMatrix);
							}
							else if(param.getIdentifier() == GpuProgramParameter::PI_MODEL_MATRIX && p_Rop)
							{
								GameObject* gameObject = 0;
								if((gameObject = p_Rop->gameobject) != 0)
								{
									Matrix4 modelMat = gameObject->getTransformation();

									param.setData((void*)&modelMat);
								}
							}
							else if(param.getIdentifier() >= GpuProgramParameter::PI_TEXTURE_1 && param.getIdentifier() <= GpuProgramParameter::PI_TEXTURE_8)
							{
								int actualTexSlot = param.getIdentifier() - GpuProgramParameter::PI_TEXTURE_1;

								K15_ASSERT(m_BoundTextures[actualTexSlot],
								StringUtil::format("Trying to use unbound texture on slot %d.",actualTexSlot));

								K15_ASSERT(m_BoundSamplers[m_BoundTextures[actualTexSlot]->getTextureSamplerSlot()],
								StringUtil::format("Texture on slot %d is trying to access NULL texture sampler on slot %d.",m_BoundTextures[actualTexSlot]->getTextureSamplerSlot()));

								param.setData((void*)&actualTexSlot);
							}
							else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_COUNT)
							{
								int lightCounter = 0;

								for(uint32 i = 0; i < m_LightArray.size(); ++i)
								{
									if(m_LightArray.at(i) != 0)
									{
										++lightCounter;
									}
								}

								param.setData((void*)&lightCounter);
							}
							else if(param.getIdentifier() == GpuProgramParameter::PI_AMBIENT_COLOR)
							{
								Vector4 ambientColor;
								m_AmbientColor.toColorVector(ambientColor);

								param.setData((void*)&ambientColor);
							}
							else if(param.getIdentifier() >= GpuProgramParameter::PI_LIGHT_TYPE && param.getIdentifier() <= GpuProgramParameter::PI_LIGHT_SPOT_EXPONENT)
							{
								if(LightComponent* light = m_LightArray.at(param.getArrayIndex()))
								{
									if(light->isActive() && light->isDirty())
									{
										lightsHaveBeenUpdated = true;

										if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_TYPE)
										{
											int type = light->getLightType();
											param.setData((void*)&type);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_RADIUS)
										{
											float radius = light->getRadius();
											param.setData((void*)&radius);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_POSITION)
										{
											Vector4 position = Vector4(light->getGameObject()->getPosition(), 1.f);
											position = getActiveCamera()->getViewMatrix() * position;

											param.setData((void*)&position);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_DIRECTION)
										{
											Vector3 direction = light->getDirection();
											param.setData((void*)&direction);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_DIFFUSE)
										{
											Vector4 colorVec;
											ColorRGBA diffuseColor = light->getDiffuseColor();
											diffuseColor.toColorVector(colorVec);
											param.setData((void*)&colorVec);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_SPECULAR)
										{
											Vector4 colorVec;
											ColorRGBA specularColor = light->getSpecularColor();
											specularColor.toColorVector(colorVec);
											param.setData((void*)&colorVec);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_CONE_ANGLE)
										{
											float coneAngle = light->getConeAngle();
											param.setData((void*)&coneAngle);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_CONSTANT_ATTENUATION)
										{
											float constantAttenuation = light->getConstantAttenuation();
											param.setData((void*)&constantAttenuation);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_LINEAR_ATTENUATION)
										{
											float linearAttenuation = light->getLinearAttenuation();
											param.setData((void*)&linearAttenuation);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_QUADRIC_ATTENUATION)
										{
											float quadricAttenuation = light->getQuadricAttenuation();
											param.setData((void*)&quadricAttenuation);
										}
										else if(param.getIdentifier() == GpuProgramParameter::PI_LIGHT_SPOT_EXPONENT)
										{
											float spotExponent = light->getSpotExponent();
											param.setData((void*)&spotExponent);
										}
									}
								}
							}

							//if data has been set, upload them to the gpu
							if(param.getData())
							{
								_updateGpuProgramParameter(param);
							}
						}
					}
				}
			}
		}

		if(lightsHaveBeenUpdated)
		{
			for(uint32 i = 0; i < m_LightArray.size(); ++i)
			{
				if(m_LightArray[i] != 0)
				{
					m_LightArray[i]->setDirty(false);
				}
			}
		}

		m_GpuParameterUpdateMask = 0;
	}
	/*********************************************************************************/
	bool RendererBase::setActiveCameraGameObject(GameObject* p_Camera)
	{
		if(p_Camera)
		{
			return setActiveCamera(static_cast<CameraComponent*>(p_Camera->getComponentByName(_ON(CameraComponent))));
		}

		return false;
	}
	/*********************************************************************************/
	GameObject* RendererBase::getActiveCameraGameObject() const
	{
		if(m_ActiveCamera)
		{
			return m_ActiveCamera->getGameObject();
		}

		return 0;
	}
	/*********************************************************************************/
	bool RendererBase::onEventResolutionChanged(GameEvent* p_Event)
	{
        ResizeEventArguments* args = static_cast<ResizeEventArguments*>(p_Event->getArgument());
        Resolution newResolution;
        newResolution.height = args->newHeight;
        newResolution.width = args->newWidth;
        _resolutionChanged(newResolution);

		return true;
	}
	/*********************************************************************************/
	bool RendererBase::onEventRenderWindowInitialized(GameEvent* p_Event)
	{
		_renderWindowInitialized();

		return true;
	}
	/*********************************************************************************/
}}
