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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_RenderWindowBase.h"
#include "K15_ColorRGBA.h"

#include "K15_GpuProgram.h"
#include "K15_GpuBuffer.h"

namespace K15_Engine { namespace Rendering { 
	
	class K15_CORE_API RendererBase : public RenderWindowBase::Listener
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

			DF_COUNT
		}; //DepthFunction
		/*********************************************************************************/
		enum ePixelFormat
		{
			//no alpha format
			PF_RGB_8_I = 0,
			PF_RGB_8_UI,
			PF_RGB_16_I,
			PF_RGB_16_U,
			PF_RGB_16_F,
			PF_RGB_32_I,
			PF_RGB_32_U,
			PF_RGB_32_F,
			PF_RGB_4_DXT1,

			//alpha format
			PF_RGBA_8_I,
			PF_RGBA_8_U,
			PF_RGBA_16_I,
			PF_RGBA_16_U,
			PF_RGBA_16_F,
			PF_RGBA_32_I,
			PF_RGBA_32_U,
			PF_RGBA_32_F,
			PF_RGBA_4_DXT1,
			PF_RGBA_8_DXT3,
			PF_RGBA_8_DXT5,

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

		void draw(RenderOperation* p_Rop);

		void setRenderWindow(RenderWindowBase* p_RenderWindow);
		INLINE RenderWindowBase* getRenderWindow() const;

		void setRenderTarget(RenderTarget* p_RenderTarget);
		INLINE RenderTarget* getRenderTarget() const;

		void setActiveCamera(Camera* p_Camera);
		INLINE Camera* getActiveCamera() const;

		void setCullingMode(Enum p_CullingMode);
		INLINE Enum getCullingMode() const;

		void setDepthTestFunction(Enum p_DepthTestFunction);
		INLINE Enum getDepthTestMode() const;

		void setTopology(Enum p_Topology);
		INLINE Enum getTopology() const;

		void setDepthTestEnabled(bool p_Enabled);
		INLINE bool getDepthTestEnabled() const;

		void setBackFaceCullingEnabled(bool p_Enabled);
		INLINE bool getBackFaceCullingEnabled() const;

		void setFrameBufferPixelFormat(Enum p_PixelFormat);
		INLINE Enum getFrameBufferPixelFormat() const;

		void setDepthBufferFormat(Enum p_DepthFormat);
		INLINE Enum getDepthBufferFormat() const;

		void setStencilBufferFormat(Enum p_StencilFormat);
		INLINE Enum getStencilBufferFormat() const;

		void setClearColor(const ColorRGBA& p_ClearColor);
		void setClearColor(float p_Red = 1.0f, float p_Green = 1.0f, float p_Blue = 1.0f);
		INLINE const ColorRGBA& getClearColor() const;

		INLINE void setLightningEnabled(bool p_Enabled);
		INLINE bool getLightningEnabled() const;

		void bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage);
		INLINE GpuProgram* getBoundGpuProgram(Enum p_GpuProgramType) const;
		INLINE bool isBoundGpuProgram(Enum p_GpuProgramType) const;

		INLINE void bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType);
		void bindMaterial(Material* p_Material);

		virtual GpuBufferImplBase* createGpuBufferImpl() = 0;
		virtual	TextureImplBase* createTextureImpl() = 0;
		virtual GpuProgramImplBase* createGpuProgramImpl() = 0;
		virtual TextureSamplerImplBase* createTextureSamplerImpl() = 0;

		INLINE const String& getLastError();
		INLINE void setLastError(const String& p_String);

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual bool initialize() = 0;
		virtual void shutdown() = 0;

		virtual void onResolutionChanged(const Resolution& p_Resolution){};

	protected:
		virtual void _setRenderWindow(RenderWindowBase* p_RenderWindow){}
		virtual void _setRenderTarget(RenderTarget* p_RenderTarget){}
		virtual void _setActiveCamera(Camera* p_Camera){}
		virtual void _setCullingMode(Enum p_CullingMode){}
		virtual void _setDepthTestFunction(Enum p_DepthTestFunction){}
		virtual void _setTopology(Enum p_Topology){}
		virtual void _setDepthTestEnabled(bool p_Enabled){}
		virtual void _setBackFaceCullingEnabled(bool p_Enabled){}
		virtual void _setFrameBufferPixelFormat(Enum p_PixelFormat){}
		virtual void _setDepthBufferPixelFormat(Enum p_DepthFormat){}
		virtual void _setStencilBufferPixelFormat(Enum p_StencilFormat){}
		virtual void _setClearColor(const ColorRGBA& p_ClearColor){}
		virtual void _bindBuffer(GpuBuffer* p_Buffer){}
		virtual void _bindProgram(GpuProgram* p_Program){}
		virtual void _draw(RenderOperation* p_Rop){}

	protected:
		ColorRGBA m_ClearColor;
		String m_LastError;
		RenderWindowBase* m_RenderWindow;
		Camera* m_ActiveCamera;
		RenderTarget* m_RenderTarget;
		RenderTarget* m_DefaultRenderTarget;
		Enum m_CullingMode;
		Enum m_DepthTestFunction;
		Enum m_Topology;
		Enum m_FrameBufferFormat;
		Enum m_DepthBufferFormat;
		Enum m_StencilBufferFormat;
		GpuProgram* m_GpuPrograms[GpuProgram::PS_COUNT];
		GpuBuffer* m_GpuBuffers[GpuBuffer::BT_COUNT];
		Material* m_Material;
		bool m_LightningEnabled;
		bool m_BackFaceCullingEnabled;
		bool m_DepthTestEnabled;
	};// end of RendererBase class definition
#include "K15_RendererBase.inl"
}}//end of K15_Engine::Rendering class

#endif //_K15Engine_Renderer_RendererBase_h_