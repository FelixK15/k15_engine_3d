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
		static GLenum GLFunctionTestConverter[DT_COUNT];
	public:
		RendererOGL();
		virtual ~RendererOGL();

		virtual void setRenderTarget(RenderTarget* p_RenderTarget);
		virtual void setActiveCamera(Camera* p_Camera);
		virtual void setCullingMode(Enum p_CullingMode);
		virtual void setDepthTestMode(Enum p_DepthTest);
		virtual void setFrameBufferPixelFormat(Enum p_PixelFormat);
		virtual void bindGpuProgram(GpuProgram* p_GpuProgram);
		virtual void setClearColor(float p_Red, float p_Green, float p_Blue);
		virtual void setDepthTestEnabled(bool p_Enabled);
		virtual void setBackFaceCullingEnabled(bool p_Enabled);

		virtual void beginFrame();
		virtual void endFrame();

		virtual bool initialize();
		virtual void shutdown();

		virtual void onResolutionChanged(const Resolution& p_Resolution);

		//inline HGLRC getRenderContext() const;

	protected:
		HDC m_DeviceContext;
		HGLRC m_RenderContext;
	};// end of RendererOGL class
}}}//end of K15_Engine::Rendering::OGL