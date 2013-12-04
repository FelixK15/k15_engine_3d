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
		  m_Topology(T_TRIANGLE)
	{
		
	}
	/*********************************************************************************/
	RendererBase::~RendererBase()
	{

	}
	/*********************************************************************************/
	void RendererBase::setRenderWindow(RenderWindowBase* p_RenderWindow)
	{
		m_RenderWindow = p_RenderWindow;
	}
	/*********************************************************************************/
	void RendererBase::setRenderTarget(RenderTarget* p_RenderTarget)
	{
		m_RenderTarget = p_RenderTarget;
	}
	/*********************************************************************************/
	void RendererBase::setActiveCamera(Camera* p_Camera)
	{
		m_ActiveCamera = p_Camera;
	}
	/*********************************************************************************/
	void RendererBase::setCullingMode(Enum p_CullingMode)
	{
		m_CullingMode = p_CullingMode;
	}
	/*********************************************************************************/
	void RendererBase::setDepthTestFunction(Enum p_DepthTestFunction)
	{
		m_DepthTestFunction = p_DepthTestFunction;
	}
	/*********************************************************************************/
	void RendererBase::setTopology(Enum p_Topology)
	{
		m_Topology = p_Topology;
	}
	/*********************************************************************************/
	void RendererBase::setDepthTestEnabled(bool p_Enabled)
	{
		m_DepthTestEnabled = p_Enabled;
	}
	/*********************************************************************************/
	void RendererBase::setBackFaceCullingEnabled(bool p_Enabled)
	{
		m_BackFaceCullingEnabled = p_Enabled;
	}
	/*********************************************************************************/
	void RendererBase::setFrameBufferPixelFormat(Enum p_PixelFormat)
	{
		m_FrameBufferFormat = p_PixelFormat;
	}
	/*********************************************************************************/
	void RendererBase::setDepthBufferFormat(Enum p_DepthFormat)
	{
		m_DepthBufferFormat = p_DepthFormat;
	}
	/*********************************************************************************/
	void RendererBase::setStencilBufferFormat(Enum p_StencilFormat)
	{
		m_StencilBufferFormat = p_StencilFormat;
	}
	/*********************************************************************************/
	void RendererBase::setClearColor(const ColorRGBA& p_ClearColor)
	{
		m_ClearColor = p_ClearColor;
	}
	/*********************************************************************************/
	void RendererBase::setClearColor(float p_Red, float p_Green, float p_Blue)
	{
		m_ClearColor = ColorRGBA(p_Red,p_Green,p_Blue);
	}
	/*********************************************************************************/
	void RendererBase::bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage)
	{
		m_GpuPrograms[p_Stage] = p_GpuProgram;
	}
	/*********************************************************************************/


}}//end of K15_Engine::Rendering namespace