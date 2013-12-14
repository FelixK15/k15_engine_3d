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

#include "K15_GpuProgram.h"
#include "K15_GpuBuffer.h"

#include "K15_VertexBuffer.h"
#include "K15_IndexBuffer.h"

#include "K15_Material.h"

#include "K15_RenderOperation.h"

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	uint8 RendererBase::PixelFormatSize[PF_COUNT]= {
		24,	//PF_RGB_8_I
		24, //PF_RGB_8_UI,
		48, //PF_RGB_16_I
		48, //PF_RGB_16_U
		48, //PF_RGB_16_F
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
		  m_DepthTestFunction(DF_GREATER_EQUAL),
		  m_DepthTestEnabled(true),
		  m_FrameBufferFormat(PF_RGB_8_I),
		  m_LightningEnabled(true),
		  m_RenderTarget(0),
		  m_RenderWindow(g_Application->getRenderWindow()),
		  m_StencilBufferFormat(SBF_COMPONENT_8_I),
		  m_Topology(RenderOperation::T_TRIANGLE)
	{
		
	}
	/*********************************************************************************/
	RendererBase::~RendererBase()
	{

	}
	/*********************************************************************************/
	void RendererBase::setRenderWindow(RenderWindowBase* p_RenderWindow)
	{
		K15_ASSERT(p_RenderWindow,"Render Window is NULL.");

		if(m_RenderWindow != p_RenderWindow)
		{
			m_RenderWindow = p_RenderWindow;
			_setRenderWindow(m_RenderWindow);
		}
	}
	/*********************************************************************************/
	void RendererBase::setRenderTarget(RenderTarget* p_RenderTarget)
	{
		K15_ASSERT(p_RenderTarget,"Render Target is NULL.");

		if(m_RenderTarget != p_RenderTarget)
		{
			m_RenderTarget = p_RenderTarget;
			_setRenderTarget(m_RenderTarget);
		}
	}
	/*********************************************************************************/
	void RendererBase::setActiveCamera(Camera* p_Camera)
	{
		K15_ASSERT(p_Camera,"Camera is NULL.");

		if(m_ActiveCamera != p_Camera)
		{
			m_ActiveCamera = p_Camera;
			_setActiveCamera(m_ActiveCamera);
		}
	}
	/*********************************************************************************/
	void RendererBase::setCullingMode(Enum p_CullingMode)
	{
		K15_ASSERT(p_CullingMode > 0 && p_CullingMode < CM_COUNT,StringUtil::format("Invalid culling mode \"%u\"",p_CullingMode));

		if(m_CullingMode != p_CullingMode)
		{
			m_CullingMode = p_CullingMode;
			_setCullingMode(m_CullingMode);
		}
	}
	/*********************************************************************************/
	void RendererBase::setDepthTestFunction(Enum p_DepthTestFunction)
	{
		K15_ASSERT(p_DepthTestFunction > 0 && p_DepthTestFunction < DF_COUNT,StringUtil::format("Invalid topology \"%u\"",p_DepthTestFunction));

		if(m_DepthTestFunction != p_DepthTestFunction)
		{
			m_DepthTestFunction = p_DepthTestFunction;
			_setDepthTestFunction(m_DepthTestFunction);
		}
	}
	/*********************************************************************************/
	void RendererBase::setTopology(Enum p_Topology)
	{
		K15_ASSERT(p_Topology > 0 && p_Topology < RenderOperation::T_COUNT,StringUtil::format("Invalid topology \"%u\"",p_Topology));

		if(p_Topology != m_Topology)
		{
			m_Topology = p_Topology;
			_setTopology(m_Topology);
		}
	}
	/*********************************************************************************/
	void RendererBase::setDepthTestEnabled(bool p_Enabled)
	{
		if(p_Enabled != m_DepthTestEnabled)
		{
			m_DepthTestEnabled = p_Enabled;
			_setDepthTestEnabled(p_Enabled);
		}
	}
	/*********************************************************************************/
	void RendererBase::setBackFaceCullingEnabled(bool p_Enabled)
	{
		if(p_Enabled != m_BackFaceCullingEnabled)
		{
			m_BackFaceCullingEnabled = p_Enabled;
			_setBackFaceCullingEnabled(p_Enabled);
		}
	}
	/*********************************************************************************/
	void RendererBase::setFrameBufferPixelFormat(Enum p_PixelFormat)
	{
		K15_ASSERT(p_PixelFormat > 0 && p_PixelFormat < PF_COUNT,StringUtil::format("Invalid frame buffer format \"%u\"",p_PixelFormat));

		if(m_FrameBufferFormat != p_PixelFormat)
		{
			m_FrameBufferFormat = p_PixelFormat;
			_setFrameBufferPixelFormat(m_FrameBufferFormat);
		}
	}
	/*********************************************************************************/
	void RendererBase::setDepthBufferFormat(Enum p_DepthFormat)
	{
		K15_ASSERT(p_DepthFormat > 0 && p_DepthFormat < DBF_COUNT,StringUtil::format("Invalid depth buffer format \"%u\"",p_DepthFormat));

		if(m_DepthBufferFormat != p_DepthFormat)
		{
			m_DepthBufferFormat = p_DepthFormat;
			_setDepthBufferPixelFormat(m_DepthBufferFormat);
		}
	}
	/*********************************************************************************/
	void RendererBase::setStencilBufferFormat(Enum p_StencilFormat)
	{
		K15_ASSERT(p_StencilFormat > 0 && p_StencilFormat < SBF_COUNT,StringUtil::format("Invalid stencil buffer format \"%u\"",p_StencilFormat));

		if(m_StencilBufferFormat != p_StencilFormat)
		{
			m_StencilBufferFormat = p_StencilFormat;
			_setStencilBufferPixelFormat(m_StencilBufferFormat);
		}
	}
	/*********************************************************************************/
	void RendererBase::setClearColor(const ColorRGBA& p_ClearColor)
	{
		if(m_ClearColor != p_ClearColor)
		{
			m_ClearColor = p_ClearColor;
			_setClearColor(m_ClearColor);
		}
	}
	/*********************************************************************************/
	void RendererBase::setClearColor(float p_Red, float p_Green, float p_Blue)
	{
		setClearColor(ColorRGBA(p_Red,p_Green,p_Blue));
	}
	/*********************************************************************************/
	void RendererBase::bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage)
	{
		K15_ASSERT(p_Stage > 0 && p_Stage < GpuProgram::PS_COUNT,StringUtil::format("Invalid GpuProgram stage. %u",p_Stage));

		if(m_GpuPrograms[p_Stage] != p_GpuProgram && p_GpuProgram)
		{
			m_GpuPrograms[p_Stage] = p_GpuProgram;
			_bindProgram(p_GpuProgram);
		}
	}
	/*********************************************************************************/
	void RendererBase::bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType)
	{
		K15_ASSERT(p_BufferType > 0 && p_BufferType < GpuBuffer::BT_COUNT,StringUtil::format("Invalid GpuBuffer type. %u",p_BufferType));

		if(m_GpuBuffers[p_BufferType] != p_Buffer && p_Buffer)
		{
			m_GpuBuffers[p_BufferType] = p_Buffer;
			_bindBuffer(p_Buffer);
		}
	}
	/*********************************************************************************/
	void RendererBase::draw(RenderOperation* p_Rop)
	{
		K15_ASSERT(p_Rop,"RenderOperation is NULL.");
		bindBuffer(p_Rop->vertexBuffer,GpuBuffer::BT_VERTEX_BUFFER);
		bindBuffer(p_Rop->indexBuffer,GpuBuffer::BT_INDEX_BUFFER);
		bindMaterial(p_Rop->material);
		setTopology(p_Rop->topology);
		
		MaterialPass* pass = 0;
		for(uint32 i = 0;i < m_Material->getAmountPasses();++i)
		{
			pass = 	m_Material->getPass(i);
			bindGpuProgram(pass->getProgram(GpuProgram::PS_VERTEX),GpuProgram::PS_VERTEX);
			bindGpuProgram(pass->getProgram(GpuProgram::PS_FRAGMENT),GpuProgram::PS_FRAGMENT);
			setCullingMode(pass->getCullingMode());
			setDepthTestFunction(pass->getDepthTestFunction());
			setLightningEnabled(pass->isLightningEnabled());
			
			_draw(p_Rop);
		}
	}
	/*********************************************************************************/
	void RendererBase::bindMaterial(Material* p_Material)
	{
		K15_ASSERT(p_Material,"Material is NULL.");

#if defined K15_DEBUG
		MaterialPass* pass = 0;
		for(uint32 i = 0;i < p_Material->getAmountPasses();++i)
		{
			pass = p_Material->getPass(i);
			K15_ASSERT(pass,StringUtil::format("%u. pass of material %s is NULL.",i,p_Material->getName().c_str()));
			K15_ASSERT(pass->getProgram(GpuProgram::PS_VERTEX),StringUtil::format("Pass %u (%s) from material %s has no vertex program.",
				i,pass->getName().c_str(),p_Material->getName().c_str()));

			K15_ASSERT(pass->getProgram(GpuProgram::PS_FRAGMENT),StringUtil::format("Pass %u (%s) from material %s has no fragment program.",
				i,pass->getName().c_str(),p_Material->getName().c_str()));
		}
#endif //K15_DEBUG
		
		if(m_Material != p_Material)
		{
			m_Material = p_Material;
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Rendering namespace