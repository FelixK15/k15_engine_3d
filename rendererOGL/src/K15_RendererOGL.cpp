#/**
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
 */

//#include "K15_RendererOGLPrecompiledHeaders.h"
#include "K15_RendererOGL.h"

#include "K15_LogManager.h"
#include "Win32\K15_RenderWindow_Win32.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	GLenum RendererOGL::GLFunctionTestConverter[DT_COUNT] = {
		GL_NONE,
		GL_LESS,
		GL_LEQUAL,
		GL_EQUAL,
		GL_GEQUAL,
		GL_GREATER
	};//DepthTestConverter
	/*********************************************************************************/

	/*********************************************************************************/
	RendererOGL::RendererOGL()
		: RendererBase(),
		  m_RenderContext(0),
		  m_DeviceContext(0)
	{

	}
	/*********************************************************************************/
	RendererOGL::~RendererOGL()
	{

	}
	/*********************************************************************************/
	void RendererOGL::setCullingMode(Enum p_CullingMode)
	{
		if(p_CullingMode == CM_CCW)
		{
			glFrontFace(GL_CCW);
		}
		else
		{
			glFrontFace(GL_CW);
		}
	}
	/*********************************************************************************/
	void RendererOGL::setDepthTestMode(Enum p_DepthTest)
	{
		glDepthFunc(GLFunctionTestConverter[p_DepthTest]);
	}
	/*********************************************************************************/
	void RendererOGL::setFrameBufferPixelFormat(Enum p_PixelFormat)
	{
		PIXELFORMATDESCRIPTOR px;
		ZeroMemory(&px,sizeof(px));

		px.nSize = sizeof(px);
		px.nVersion = 1;
		px.iPixelType = PFD_TYPE_RGBA;
		px.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		px.cColorBits = PixelFormatSize[p_PixelFormat];
		px.cDepthBits = DepthFormatSize[m_DepthBufferFormat];
		px.cStencilBits = StencilFormatSize[m_StencilBufferFormat];

		if(ChoosePixelFormat(m_DeviceContext,&px) == 0)
		{
			_LogError("Could not set pixel format.");
		}
	}
	/*********************************************************************************/
	void RendererOGL::setClearColor(float p_Red, float p_Green, float p_Blue)
	{
		RendererBase::setClearColor(p_Red,p_Green,p_Blue);

		glClearColor(p_Red,p_Green,p_Blue,1.0f);
	}
	/*********************************************************************************/
	void RendererOGL::setDepthTestEnabled(bool p_Enabled)
	{
		RendererBase::setDepthTestEnabled(p_Enabled);

		if(p_Enabled)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
	/*********************************************************************************/
	void RendererOGL::setBackFaceCullingEnabled(bool p_Enabled)
	{
		if(p_Enabled)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
	/*********************************************************************************/
	void RendererOGL::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear color, depth and stencil buffer
	}
	/*********************************************************************************/
	void RendererOGL::endFrame()
	{
		SwapBuffers(m_DeviceContext);
		//wglSwapLayerBuffers(m_DeviceContext,WGL_SWAP_MAIN_PLANE);
	}
	/*********************************************************************************/
	bool RendererOGL::initialize()
	{
		K15_ASSERT(m_RenderWindow,"Render window has not been set. Can not initialize renderer without having a render window.");

		RenderWindow_Win32* renderwindow = (RenderWindow_Win32*)m_RenderWindow;
		m_DeviceContext = renderwindow->getDeviceContext();

		_LogNormal("Creating OGL rendering context...");

		if((m_RenderContext = wglCreateContext(m_DeviceContext)) == 0)
		{
			return false;
		}

		_LogSuccess("Successfully created OGL rendering context!");
		_LogNormal("Setting OGL rendering context as current context.");

		if(wglMakeCurrent(m_DeviceContext,m_RenderContext) == TRUE)
		{
			return false;
		}

		_LogSuccess("Succesfully set OGL rendering context.");

		const Resolution& resolution = renderwindow->getResolution();

		onResolutionChanged(resolution);
		setClearColor(1.0f,1.0f,1.0f); 
		setDepthTestEnabled(true); //enable depth testing
		glEnable(GL_STENCIL_TEST); //enable stencil testing

		return true;

	}
	/*********************************************************************************/
	void RendererOGL::shutdown()
	{
		if(m_RenderContext)
		{
			wglMakeCurrent(0,0);
			wglDeleteContext(m_RenderContext);
		}
	}
	/*********************************************************************************/
	void RendererOGL::onResolutionChanged(const Resolution& p_Resolution)
	{
		glViewport(0,0,p_Resolution.width,p_Resolution.height);
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL