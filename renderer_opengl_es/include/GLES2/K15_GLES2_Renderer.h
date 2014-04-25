/**
 * @file K15_Renderer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
#ifndef _K15Engine_RendererGLES2_Renderer_h_
#define _K15Engine_RendererGLES2_Renderer_h_

#ifndef K15_RENDERER_USE_PREDEFINED_HEADERS
#	include "K15_RendererGLESPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_RendererBase.h"
#include "K15_RenderOperation.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
	class Renderer : public RendererBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLFunctionTestConverter[RendererBase::FT_COUNT];
		static const GLenum GLTopologyConverter[RenderOperation::T_COUNT];
		static const GLenum GLBlendOperationConverter[AlphaState::BO_COUNT];
		static const GLenum GLBlendFunctionConverter[AlphaState::BF_COUNT];
		/*********************************************************************************/
	public:
		Renderer();
		~Renderer();

		virtual GpuBufferImplBase* createGpuBufferImpl() OVERRIDE;
		virtual TextureImplBase* createTextureImpl() OVERRIDE;
		virtual GpuProgramImplBase* createGpuProgramImpl() OVERRIDE;
		virtual TextureSamplerImplBase* createTextureSamplerImpl() OVERRIDE;
		virtual VertexDeclarationImplBase* createVertexDeclarationImpl() OVERRIDE;
		virtual GpuProgramBatchImplBase* createGpuProgramBatchImpl() OVERRIDE;

		virtual void beginFrame() OVERRIDE;
		virtual void endFrame() OVERRIDE;

		virtual bool _initialize() OVERRIDE;
		virtual void _shutdown() OVERRIDE;

		virtual void onResolutionChanged(const Resolution& p_Resolution) OVERRIDE;

	protected:
		virtual void _setFillMode(Enum p_FillMode) OVERRIDE;
		virtual void _setAlphaState(const AlphaState& p_AlphaState) OVERRIDE;
		virtual void _setDepthState(const DepthState& p_DepthState) OVERRIDE;
		virtual void _setRenderWindow(RenderWindowBase* p_RenderWindow) OVERRIDE;
		virtual void _setRenderTarget(RenderTarget* p_RenderTarget) OVERRIDE;
		virtual void _setActiveCamera(CameraComponent* p_Camera) OVERRIDE;
		virtual void _setVertexDeclaration(VertexDeclaration* p_Declaration) OVERRIDE;
		virtual void _setCullingMode(Enum p_CullingMode) OVERRIDE;
		virtual void _setTopology(Enum p_Topology) OVERRIDE;
		virtual void _setBackFaceCullingEnabled(bool p_Enabled) OVERRIDE;
		virtual void _setFrameBufferPixelFormat(Enum p_PixelFormat) OVERRIDE;
		virtual void _setDepthBufferPixelFormat(Enum p_DepthFormat) OVERRIDE;
		virtual void _setStencilBufferPixelFormat(Enum p_StencilFormat) OVERRIDE;
		virtual void _setClearColor(const ColorRGBA& p_ClearColor) OVERRIDE;
		virtual void _updateGpuProgramParameter(const RawData& p_Data, const GpuProgramParameter& p_Parameter) OVERRIDE;
		virtual void _bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType) OVERRIDE;
		virtual void _bindProgram(GpuProgram* p_Program, Enum p_ProgramType) OVERRIDE;
		virtual void _bindProgramBatch(GpuProgramBatch* p_ProgramBatch) OVERRIDE;
		virtual void _bindTexture(Texture* p_Texture, Enum p_Type) OVERRIDE;
		virtual void _drawIndexed(uint32 p_Offset = 0) OVERRIDE;
		virtual void _drawDirect(uint32 p_Offset = 0) OVERRIDE;
	private:
		EGLDisplay m_Display;
		EGLSurface m_Surface;
		EGLContext m_Context;
		GLint m_MaxVertexAttribs;
	};// end of Renderer class declaration
}}}// end of K15_Engine::Rendering::GLES2 namespace

#endif //_K15Engine_RendererGLES2_Renderer_h_