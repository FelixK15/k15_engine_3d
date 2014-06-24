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

#include "K15_Prerequisites.h"

#include "K15_ColorRGBA.h"

#include "K15_GpuProgram.h"
#include "K15_GpuBuffer.h"
#include "K15_Texture.h"

#include "K15_AlphaState.h"
#include "K15_DepthState.h"

namespace K15_Engine { namespace Rendering { 
	
	class K15_CORE_API RendererBase : public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef FixedArray(GpuProgram*,GpuProgram::PS_COUNT) GpuProgramArray;
		typedef FixedArray(GpuBuffer*,GpuBuffer::BT_COUNT) GpuBufferArray;
		typedef FixedArray(Texture*,Texture::TS_COUNT) TextureArray;
		typedef FixedArray(TextureSampler*,Texture::TS_COUNT) TextureSamplerArray;
		/*********************************************************************************/

		/*********************************************************************************/
		enum eFillMode
		{
		  FM_SOLID = 0,
		  FM_WIREFRAME,

		  FM_COUNT
		};// FillMode
		/*********************************************************************************/
		enum eCullingMode
		{
			CM_CCW = 0,
			CM_CW,

			CM_COUNT
		};// CullingMode
		/*********************************************************************************/
		enum eFunctionType
		{
			FT_NONE = 0,
 			FT_LESS,
			FT_LESS_EQUAL,
			FT_EQUAL,
			FT_GREATER_EQUAL,
			FT_GREATER,

			FT_COUNT
		}; //eFunctionType
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

		bool initialize();
		void shutdown();

		bool draw(RenderOperation* p_Rop);
		//bool draw(RenderQueue* m_RenderQueue);

		bool setRenderTarget(RenderTarget* p_RenderTarget);
		INLINE RenderTarget* getRenderTarget() const;

		bool setActiveCameraGameObject(GameObject* p_Camera);
		bool setActiveCamera(CameraComponent* p_Camera);
		INLINE CameraComponent* getActiveCamera() const;
		GameObject* getActiveCameraGameObject() const;

		bool setCullingMode(Enum p_CullingMode);
		INLINE Enum getCullingMode() const;

		bool setTopology(Enum p_Topology);
		INLINE Enum getTopology() const;

		bool setBackFaceCullingEnabled(bool p_Enabled);
		INLINE bool getBackFaceCullingEnabled() const;

		bool setFrameBufferPixelFormat(Enum p_PixelFormat);
		INLINE Enum getFrameBufferPixelFormat() const;

		bool setDepthBufferFormat(Enum p_DepthFormat);
		INLINE Enum getDepthBufferFormat() const;

		bool setStencilBufferFormat(Enum p_StencilFormat);
		INLINE Enum getStencilBufferFormat() const;

		bool setFillMode(Enum p_FillMode);
		INLINE Enum getFillMode() const;

		bool bindTexture(Texture* p_Texture, Enum p_Type, Enum p_Slot);
		INLINE Texture* getBoundTexture(Enum p_Slot) const;

		bool bindTextureSampler(TextureSampler* p_Sampler, Enum p_Slot);
		INLINE TextureSampler* getBoundSampler(Enum p_Slot) const;

		bool setClearColor(const ColorRGBA& p_ClearColor);
		bool setClearColor(byte p_Red = 255, byte p_Green = 255, byte p_Blue = 255);
		INLINE ColorRGBA& getClearColor();

		INLINE void setLightningEnabled(bool p_Enabled);
		INLINE bool getLightningEnabled() const;

		bool setAlphaState(const AlphaState& p_AlphaState);
		INLINE AlphaState& getAlphaState();

		bool setDepthState(const DepthState& p_DepthState);
		INLINE DepthState& getDepthState();

		bool bindGpuProgramBatch(GpuProgramBatch* p_GpuProgramBatch);
		INLINE GpuProgramBatch* getBoundGpuProgramBatch() const;

		bool bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage);
		INLINE GpuProgram* getBoundGpuProgram(Enum p_GpuProgramType) const;
		INLINE bool isBoundGpuProgram(Enum p_GpuProgramType) const;

		INLINE bool errorOccured() const;

		void updateGpuProgramParameter(RenderOperation* p_Rop);

		bool bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType);
		bool bindMaterial(Material* p_Material);

		GpuBuffer* getBoundBuffer(Enum p_BufferType) const;

		bool setVertexDeclaration(VertexDeclaration* p_Declaration);
		INLINE VertexDeclaration* getVertexDeclaration() const; 

		virtual GpuBufferImplBase* createGpuBufferImpl() = 0;
		virtual	TextureImplBase* createTextureImpl() = 0;
		virtual GpuProgramImplBase* createGpuProgramImpl() = 0;
		virtual TextureSamplerImplBase* createTextureSamplerImpl() = 0;
		virtual VertexDeclarationImplBase* createVertexDeclarationImpl() = 0;
		virtual GpuProgramBatchImplBase* createGpuProgramBatchImpl() = 0;

		INLINE const String& getLastError();
		INLINE void setLastError(const String& p_String);

		INLINE bool isInitialized() const;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		bool onEventResolutionChanged(GameEvent* p_Event);
		bool onEventRenderWindowInitialized(GameEvent* p_Event);

	protected:
		virtual void _resolutionChanged(const Resolution& p_Resolution){}
		virtual void _renderWindowInitialized(){}
		virtual bool _initialize() = 0;
		virtual void _shutdown() = 0;
		virtual void _setFillMode(Enum p_FillMode){}
		virtual void _setAlphaState(const AlphaState& p_AlphaState){}
		virtual void _setDepthState(const DepthState& p_DepthState){}
		virtual void _setRenderTarget(RenderTarget* p_RenderTarget){}
		virtual void _setActiveCamera(CameraComponent* p_Camera){}
		virtual void _setVertexDeclaration(VertexDeclaration* p_Declaration){}
		virtual void _setCullingMode(Enum p_CullingMode){}
		virtual void _setTopology(Enum p_Topology){}
		virtual void _setBackFaceCullingEnabled(bool p_Enabled){}
		virtual void _setFrameBufferPixelFormat(Enum p_PixelFormat){}
		virtual void _setDepthBufferPixelFormat(Enum p_DepthFormat){}
		virtual void _setStencilBufferPixelFormat(Enum p_StencilFormat){}
		virtual void _setClearColor(const ColorRGBA& p_ClearColor){}
		virtual void _updateGpuProgramParameter(const GpuProgramParameter& p_Parameter){}
		virtual void _bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType){}
		virtual void _bindProgram(GpuProgram* p_Program, Enum p_ProgramType){}
		virtual void _bindProgramBatch(GpuProgramBatch* p_ProgramBatch){}
		virtual void _bindTexture(Texture* p_Texture, Enum p_Type){}
		virtual void _bindTextureSampler(TextureSampler* p_Sampler, Enum p_Slot){}
		virtual void _drawIndexed(uint32 p_Offset = 0){}
		virtual void _drawDirect(uint32 p_Offset = 0){}

	protected:
		ColorRGBA m_ClearColor;
		AlphaState m_AlphaState;
		DepthState m_DepthState;
		GpuProgramArray m_GpuPrograms;
		GpuBufferArray m_GpuBuffers;
		TextureArray m_BoundTextures;
		TextureSamplerArray m_BoundSamplers;
		String m_LastError;
		CameraComponent* m_ActiveCamera;
		RenderTarget* m_RenderTarget;
		RenderTarget* m_DefaultRenderTarget;
		Material* m_Material;
		VertexDeclaration* m_VertexDeclaration;
		GpuProgramBatch* m_GpuProgramBatch;
		uint32 m_GpuParameterUpdateMask;
		Enum m_FillMode;
		Enum m_CullingMode;
		Enum m_Topology;
		Enum m_FrameBufferFormat;
		Enum m_DepthBufferFormat;
		Enum m_StencilBufferFormat;
		bool m_LightningEnabled;
		bool m_BackFaceCullingEnabled;
		bool m_Initialized;
	};// end of RendererBase class definition
#include "K15_RendererBase.inl"
}}//end of K15_Engine::Rendering class

#endif //_K15Engine_Renderer_RendererBase_h_