/**
 * @file K15_RendererOGL.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_RendererBase.h"
#include "K15_RenderOperation.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	class RendererOGL : public RendererBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLFunctionTestConverter[RendererBase::FT_COUNT];
		static const GLenum GLTopologyConverter[RenderOperation::T_COUNT];
		static const GLenum GLBlendOperationConverter[AlphaState::BO_COUNT];
		static const GLenum GLBlendFunctionConverter[AlphaState::BF_COUNT];
		/*********************************************************************************/
	public:
		RendererOGL();
		virtual ~RendererOGL();

		virtual GpuBufferImplBase* createGpuBufferImpl() OVERRIDE;
		virtual TextureImplBase* createTextureImpl() OVERRIDE;
		virtual TextureSamplerImplBase* createTextureSamplerImpl() OVERRIDE;
		virtual GpuProgramImplBase* createGpuProgramImpl() OVERRIDE;
    virtual VertexDeclarationImplBase* createVertexDeclarationImpl() OVERRIDE;

		virtual void beginFrame() OVERRIDE;
		virtual void endFrame() OVERRIDE;

		virtual bool initialize() OVERRIDE;
		virtual void shutdown() OVERRIDE;

		virtual void onResolutionChanged(const Resolution& p_Resolution) OVERRIDE;

		//inline HGLRC getRenderContext() const;

	protected:
		bool createDummyContext(HWND*,HDC*);

		virtual void _setFillMode(Enum p_FillMode) OVERRIDE;
		virtual void _setAlphaState(const AlphaState& p_AlphaState) OVERRIDE;
		virtual void _setDepthState(const DepthState& p_DepthState) OVERRIDE;
		virtual void _setRenderWindow(RenderWindowBase* p_RenderWindow) OVERRIDE;
		virtual void _setRenderTarget(RenderTarget* p_RenderTarget) OVERRIDE;
		virtual void _setActiveCamera(CameraComponent* p_Camera) OVERRIDE;
		virtual void _setVertexDeclaration(VertexDeclaration* p_Declaration) OVERRIDE;
		virtual void _setCullingMode(Enum p_CullingMode) OVERRIDE;
		virtual void _setBackFaceCullingEnabled(bool p_Enabled) OVERRIDE;
		virtual void _setFrameBufferPixelFormat(Enum p_PixelFormat) OVERRIDE;
		virtual void _setDepthBufferPixelFormat(Enum p_DepthFormat) OVERRIDE;
		virtual void _setStencilBufferPixelFormat(Enum p_StencilFormat) OVERRIDE;
		virtual void _setClearColor(const ColorRGBA& p_ClearColor) OVERRIDE;
		virtual void _bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType) OVERRIDE;
		virtual void _bindProgram(GpuProgram* p_Program, Enum p_ProgramType) OVERRIDE;
		virtual void _bindProgramParameter(const GpuProgramParameter& p_Parameter,const RawData& p_Data) OVERRIDE;
		virtual void _drawIndexed(uint32 p_Offset) OVERRIDE;
		virtual void _drawDirect(uint32 p_Offset) OVERRIDE;

	protected:
		GLuint m_ProgramPipeline;
		HDC m_DeviceContext;
		HGLRC m_RenderContext;

	};// end of RendererOGL class
}}}//end of K15_Engine::Rendering::OGL