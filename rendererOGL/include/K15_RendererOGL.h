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

namespace K15_Engine { namespace Rendering { namespace OGL {
	class RendererOGL : public RendererBase
	{
	public:
		/*********************************************************************************/
		static const GLenum GLFunctionTestConverter[DT_COUNT];
		/*********************************************************************************/
	public:
		RendererOGL();
		virtual ~RendererOGL();

		//virtual void setRenderTarget(RenderTarget* p_RenderTarget) OVERRIDE;
		//virtual void setActiveCamera(Camera* p_Camera);
		virtual void setCullingMode(Enum p_CullingMode) OVERRIDE;
		virtual void setDepthTestFunction(Enum p_DepthTest) OVERRIDE;
		virtual void bindGpuProgram(GpuProgram* p_GpuProgram,Enum p_Stage) OVERRIDE;
		virtual void setClearColor(float p_Red, float p_Green, float p_Blue) OVERRIDE;
		virtual void setDepthTestEnabled(bool p_Enabled) OVERRIDE;
		virtual void setBackFaceCullingEnabled(bool p_Enabled) OVERRIDE;

		virtual GpuBufferImplBase* createGpuBufferImpl() OVERRIDE;
		virtual TextureImplBase* createTextureImpl() OVERRIDE;
		virtual TextureSamplerImplBase* createTextureSamplerImpl() OVERRIDE;
		virtual GpuProgramImplBase* createGpuProgramImpl() OVERRIDE;

		virtual void beginFrame() OVERRIDE;
		virtual void endFrame() OVERRIDE;

		virtual bool initialize() OVERRIDE;
		virtual void shutdown() OVERRIDE;

		virtual void onResolutionChanged(const Resolution& p_Resolution) OVERRIDE;

		//inline HGLRC getRenderContext() const;

	protected:
		bool createDummyContext(HWND*,HDC*);

	protected:
		GLuint m_ProgramPipeline;
		HDC m_DeviceContext;
		HGLRC m_RenderContext;

	};// end of RendererOGL class
}}}//end of K15_Engine::Rendering::OGL