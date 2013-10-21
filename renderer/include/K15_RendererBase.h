/**
 * @file K15_RendererBase.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_RendererBase_h_
#define _K15Engine_Renderer_RendererBase_h_

#ifndef K15_RENDERER_USE_PRECOMPILED_HEADERS
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_USE_PRECOMPILED_HEADERS

#include "K15_RenderWindowBase.h"

namespace K15_Engine { namespace Rendering { 
	
	class RendererBase : public RenderWindowBase::Listener
	{
	public:
		/*********************************************************************************/
		enum eCullingMode
		{
			CM_CCW = 0,
			CM_CW,

			CM_COUNT
		};// CullingMode
		/*********************************************************************************/
		enum eDepthFunction
		{
			DF_NONE = 0,		//<! Pixels are always written.
 			DF_LESS,
			DF_LESS_EQUAL,
			DF_EQUAL,
			DF_GREATER_EQUAL,
			DF_GREATER,

			DT_COUNT
		}; //DepthFunction
		/*********************************************************************************/
		enum eTopology
		{
			T_DOT = 0,
			T_LINE,
			T_TRIANGLE,
			T_QUAD,

			T_COUNT
		}; //Topology
		/*********************************************************************************/
		enum eGpuProgramType
		{
			GPT_VERTEX = 0,
			GPT_GEOMETRY,
			GPT_PIXEL,

			GPT_COUNT
		}; //GpuProgramType
		/*********************************************************************************/
		enum ePixelFormat
		{
			PF_RGB_8_I = 0,
			PF_RGB_8_UI,
			PF_RGB_16_I,
			PF_RGB_16_U,
			PF_RGB_16_F,
			PF_RGB_32_I,
			PF_RGB_32_U,
			PF_RGB_32_F,
			PF_RGBA_8_I,
			PF_RGBA_8_U,
			PF_RGBA_16_I,
			PF_RGBA_16_U,
			PF_RGBA_16_F,
			PF_RGBA_32_I,
			PF_RGBA_32_U,
			PF_RGBA_32_F,

			PF_COUNT
		}; //PixelFormat
		/*********************************************************************************/
		enum eDepthBufferFormat
		{
			DBF_COMPONENT_16_I = 0,
			DBF_COMPONENT_24_I,
			DBF_COMPONENT_32_I,
			DBF_COMPONENT_32_F,

			DBF_COUNT
		}; //DepthBufferFormat
		/*********************************************************************************/
		enum eStencilBufferFormat
		{
			SBF_COMPONENT_1_I = 0,
			SBF_COMPONENT_4_I,
			SBF_COMPONENT_8_I,
			SBF_COMPONENT_16_I,

			SBF_COUNT
		}; //StencilBufferFormat
		/*********************************************************************************/
		static uint8 PixelFormatSize[PF_COUNT];
		static uint8 DepthFormatSize[DBF_COUNT];
		static uint8 StencilFormatSize[SBF_COUNT];
		/*********************************************************************************/
	public:
		RendererBase();
		virtual ~RendererBase();

		virtual void setRenderWindow(RenderWindowBase* p_RenderWindow);
		inline RenderWindowBase* getRenderWindow() const;

		virtual void setRenderTarget(RenderTarget* p_RenderTarget);
		inline RenderTarget* getRenderTarget() const;

		virtual void setActiveCamera(Camera* p_Camera);
		inline Camera* getActiveCamera() const;

		virtual void setCullingMode(Enum p_CullingMode);
		inline Enum getCullingMode() const;

		virtual void setDepthTestMode(Enum p_DepthTest);
		inline Enum getDepthTestMode() const;

		virtual void setTopology(Enum p_Topology);
		inline Enum getTopology() const;

		virtual void setDepthTestEnabled(bool p_Enabled);
		inline bool getDepthTestEnabled() const;

		virtual void setBackFaceCullingEnabled(bool p_Enabled);
		inline bool getBackFaceCullingEnabled() const;

		virtual void setFrameBufferPixelFormat(Enum p_PixelFormat);
		inline Enum getFrameBufferPixelFormat() const;

		virtual void setDepthBufferFormat(Enum p_DepthFormat);
		inline Enum getDepthBufferFormat() const;

		virtual void setStencilBufferFormat(Enum p_StencilFormat);
		inline Enum getStencilBufferFormat() const;

		//virtual void setClearColor(const Color& p_ClearColor);
		virtual void setClearColor(float p_Red = 1.0f, float p_Green = 1.0f, float p_Blue = 1.0f);
		//inline const Color& getClearColor() const;

		inline void setLightningEnabled(bool p_Enabled);
		inline bool getLightningEnabled() const;

		virtual void bindGpuProgram(GpuProgram* p_GpuProgram);
		inline GpuProgram* getBoundGpuProgram(Enum p_GpuProgramType);
		inline bool isBoundGpuProgram(Enum p_GpuProgramType);

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual bool initialize() = 0;
		virtual void shutdown() = 0;

		virtual void onResolutionChanged(const Resolution& p_Resolution);

	protected:
		//Color m_ClearColor;
		RenderWindowBase* m_RenderWindow;
		Camera* m_ActiveCamera;
		RenderTarget* m_RenderTarget;
		RenderTarget* m_DefaultRenderTarget;
		Enum m_CullingMode;
		Enum m_DepthFunction;
		Enum m_Topology;
		Enum m_FrameBufferFormat;
		Enum m_DepthBufferFormat;
		Enum m_StencilBufferFormat;
		GpuProgram* m_GpuPrograms[GPT_COUNT];
		bool m_LightningEnabled;
		bool m_BackFaceCullingEnabled;
		bool m_DepthTestEnabled;
	};// end of RendererBase class definition
#include "K15_RendererBase.inl"
}}//end of K15_Engine::Rendering class

#endif //_K15Engine_Renderer_RendererBase_h_