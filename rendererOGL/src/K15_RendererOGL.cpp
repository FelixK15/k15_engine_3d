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

#include "GL\glew.h"
#include "GL\wglew.h"
#include "K15_RenderTask.h"
#include "K15_LogManager.h"
#include "K15_GpuBufferImplOGL.h"
#include "K15_GpuProgramImplOGL.h"
#include "K15_TextureImplOGL.h"
#include "K15_TextureSamplerImplOGL.h"
#include "K15_IndexBuffer.h"
#include "Win32\K15_RenderWindow_Win32.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	const GLenum RendererOGL::GLFunctionTestConverter[RendererBase::FT_COUNT] = {
    GL_NONE, 	  //FT_NONE
		GL_LESS,    //FT_LESS
		GL_LEQUAL,  //FT_LESS_EQUAL
		GL_EQUAL,   //FT_EQUAL
		GL_GEQUAL,  //FT_GREATER_EQUAL
		GL_GREATER  //FT_GREATER
	};//DepthTestConverter
  /*********************************************************************************/
  const GLenum RendererOGL::GLTopologyConverter[RenderOperation::T_COUNT] = {
    GL_POINTS,    //T_DOT
    GL_LINES,     //T_LINE
    GL_TRIANGLES, //T_TRIANGLE
    GL_QUADS      //T_QUAD
  }; //TopologyConverter
	/*********************************************************************************/
	void APIENTRY glLogError(GLenum p_Source, GLenum p_Type, GLuint p_ID, GLenum p_Severity, GLsizei p_Length, const GLchar* p_Message, GLvoid* p_UserParam)
	{
		static String msg;
		if(p_Source == GL_DEBUG_SOURCE_API)
		{
			msg = "API error:";
		}
		else if(p_Source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
		{
			msg = "Window system error:";
		}
		else if(p_Source == GL_DEBUG_SOURCE_SHADER_COMPILER)
		{
			msg = "Shader compiler error:";
		}
		else if(p_Source == GL_DEBUG_SOURCE_THIRD_PARTY)
		{
			msg = "Third party error:";
		}
		else if(p_Source == GL_DEBUG_SOURCE_APPLICATION)
		{
			msg = "Application error:";
		}
		else
		{
			msg = "Undefined OGL error:";
		}

		msg += p_Message;

		RendererBase* renderer = (RendererBase*)p_UserParam;
			
		if(renderer)
		{
			renderer->setLastError(msg);
		}
	}
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
	void RendererOGL::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear color, depth and stencil buffer
	}
	/*********************************************************************************/
	void RendererOGL::endFrame()
	{
		SwapBuffers(m_DeviceContext);
	}
	/*********************************************************************************/
	bool RendererOGL::initialize()
	{
		K15_ASSERT(m_RenderWindow,"Render window has not been set. Can not initialize renderer without having a render window.");
		
		_LogNormal("Creating dummy OGL context to initialize GLEW library.");
		HWND tempHwnd = 0;
		HDC tempDC = 0;
		if(!createDummyContext(&tempHwnd,&tempDC))
		{
			_LogError("Could not create dummy OGL context (%s)",g_Application->getLastError());
			return false;
		}
		_LogSuccess("Successfully created dummy OGL context!");

		_LogNormal("Trying to initialize GLEW library...");
		GLenum error = glewInit();
		if(error != GLEW_OK)
		{
			_LogError("Could not initialize GLEW library. (%s)",glewGetErrorString(error));
			return false;
		}

		_LogSuccess("Successfully initialized GLEW library!");
		_LogSuccess("Supported GLEW Version:%s",glewGetString(GLEW_VERSION));

		_LogNormal("Destroying dummy OGL context...");
		shutdown();
		//destroy the objects for the temp handles
		ReleaseDC(tempHwnd,tempDC);
		DestroyWindow(tempHwnd);

		RenderWindow_Win32* renderwindow = (RenderWindow_Win32*)m_RenderWindow;
		m_DeviceContext = renderwindow->getDeviceContext();

		int colorBits = PixelFormatSize[m_FrameBufferFormat];
		int depthBits = DepthFormatSize[m_DepthBufferFormat];
		int stencilBits = StencilFormatSize[m_StencilBufferFormat];

		_LogNormal("Creating real OGL context...");

		const int pixelFormatAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, colorBits,
			WGL_DEPTH_BITS_ARB, depthBits,
			WGL_STENCIL_BITS_ARB, stencilBits,
			0
		};

		int pixelFormat = 0;
		unsigned int formatCount = 1;

		_LogNormal("Trying to create pixelformat\n\tColorbuffer size per pixel:%i\n\tDepthbuffer size per pixel:%i\n\tStencilbuffer size per pixel:%i.",
			       colorBits,depthBits,stencilBits);

		wglChoosePixelFormatARB(m_DeviceContext,pixelFormatAttributes,0,1,&pixelFormat,&formatCount);

		K15_ASSERT(pixelFormat,StringUtil::format("Can't create pixel format because the is not supported. Error:%s",glGetString(glGetError())));

		SetPixelFormat(m_DeviceContext,pixelFormat,0);

		const int contextAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB,WGL_CONTEXT_DEBUG_BIT_ARB,
			0 //END
		};

		_LogNormal("Creating OGL rendering context...");
		if((m_RenderContext = wglCreateContextAttribsARB(m_DeviceContext,0,contextAttributes)) == 0)
		{
			_LogError("Could not create OGL rendering context. Error:%s",g_Application->getLastError());
			return false;
		}

		_LogSuccess("Successfully created OGL rendering context!");
		_LogNormal("Setting OGL rendering context as current context.");

		if(wglMakeCurrent(m_DeviceContext,m_RenderContext) == FALSE)
		{
			_LogError("Could not set OGL rendering context as current context. %s",g_Application->getLastError());
			return false;
		}

		_LogSuccess("Succesfully set OGL rendering context.");
		_LogSuccess("Supported OpenGL Version:\"%s\"",glGetString(GL_VERSION));

		PFNGLDEBUGMESSAGECALLBACKARBPROC debugfunc = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");

		if(debugfunc)
		{
			debugfunc(glLogError,(const void*)(this));
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		//create program pipeline
		glGenProgramPipelines(1,&m_ProgramPipeline);
		glBindProgramPipeline(m_ProgramPipeline);

		
		const Resolution& resolution = renderwindow->getResolution();
		//force viewport resizing
		onResolutionChanged(resolution);
		setClearColor(0.8f,0.2224f,0.005f);
		glEnable(GL_STENCIL_TEST); //enable stencil testing

		return true;
	}
	/*********************************************************************************/
	void RendererOGL::shutdown()
	{
		glBindProgramPipeline(0);
		glDeleteProgramPipelines(1,&m_ProgramPipeline);
		m_ProgramPipeline = 0;

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
	bool RendererOGL::createDummyContext(HWND* p_Hwnd,HDC* p_DC)
	{
		//Create dummy window
		HWND tempHandle = 0;
		HDC tempDC = 0; int pixelFormat = 0;
		if((tempHandle = CreateWindow("K15_RenderWindowClass","OGL DUMMY WINDOW",0,0,0,0,0,0,0,0,0)) != 0)
		{
			tempDC = GetDC(tempHandle);

			PIXELFORMATDESCRIPTOR px;
			ZeroMemory(&px,sizeof(px));

			px.nSize = sizeof(px);
			px.nVersion = 1;
			px.iPixelType = PFD_TYPE_RGBA;
			px.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
			px.cColorBits = 32;
			px.cDepthBits = 24;
			px.cStencilBits = 8;
			px.dwLayerMask = PFD_MAIN_PLANE;

			if((pixelFormat = ChoosePixelFormat(tempDC,&px)) == 0)
			{
				return false;
			}

			SetPixelFormat(tempDC,pixelFormat,&px);

			m_RenderContext = wglCreateContext(tempDC);
			wglMakeCurrent(tempDC,m_RenderContext);
		
			return true;
		}

		return false;
	}
	/*********************************************************************************/
	GpuBufferImplBase* RendererOGL::createGpuBufferImpl()
	{
		return new GpuBufferImplOGL();
	}
	/*********************************************************************************/
	TextureImplBase* RendererOGL::createTextureImpl()
	{
		return new TextureImplOGL();
	}
	/*********************************************************************************/
	GpuProgramImplBase* RendererOGL::createGpuProgramImpl()
	{
		return new GpuProgramImplOGL();
	}
	/*********************************************************************************/
	TextureSamplerImplBase* RendererOGL::createTextureSamplerImpl()
	{
		return new TextureSamplerImplOGL();
	}
  /*********************************************************************************/
  void RendererOGL::_setAlphaState(const AlphaState& p_AlphaState)
  {
    throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setDepthState(const DepthState& p_DepthState)
  {
    if(p_DepthState.getEnabled())
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    } 

    glDepthFunc(GLFunctionTestConverter[p_DepthState.getFunction()]);
  }
  /*********************************************************************************/
  void RendererOGL::_setRenderWindow(RenderWindowBase* p_RenderWindow)
  {
    //throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setRenderTarget(RenderTarget* p_RenderTarget)
  {
    //throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setActiveCamera(CameraComponent* p_Camera)
  {
    //throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setCullingMode(Enum p_CullingMode)
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
  void RendererOGL::_setBackFaceCullingEnabled(bool p_Enabled)
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
  void RendererOGL::_setFrameBufferPixelFormat(Enum p_PixelFormat)
  {
    throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setDepthBufferPixelFormat(Enum p_DepthFormat)
  {
    throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setStencilBufferPixelFormat(Enum p_StencilFormat)
  {
    throw std::exception("The method or operation is not implemented.");
  }
  /*********************************************************************************/
  void RendererOGL::_setClearColor(const ColorRGBA& p_ClearColor)
  {
    glClearColor(p_ClearColor.RedComponent,p_ClearColor.GreenComponent,p_ClearColor.BlueComponent,1.0f);
  }
  /*********************************************************************************/
  void RendererOGL::_bindBuffer(GpuBuffer* p_Buffer)
  {
    const GpuBufferImplOGL* bufferOGL = static_cast<const GpuBufferImplOGL*>(p_Buffer->getImpl());
    GLenum target = GpuBufferImplOGL::GLBufferTypeConverter[p_Buffer->getType()];

    glBindBuffer(target,bufferOGL->getBufferGL());

    if(glGetError() != GL_NO_ERROR)
    {
      _LogError("Could not bind buffer. %s",
        g_Application->getRenderTask()->getRenderer()->getLastError().c_str());
    }
  }
  /*********************************************************************************/
  void RendererOGL::_bindProgram(GpuProgram* p_Program)
  {
    GLbitfield stages = GL_NONE;
    Enum programStage = p_Program->getStage();
    if(programStage == GpuProgram::PS_VERTEX)
    {
      stages |= GL_VERTEX_SHADER_BIT;
    }
    else if(programStage == GpuProgram::PS_FRAGMENT)
    {
      stages |= GL_FRAGMENT_SHADER_BIT;
    }
    else if(programStage == GpuProgram::PS_GEOMETRY)
    {
      stages |= GL_GEOMETRY_SHADER_BIT;
    }
    else if(programStage == GpuProgram::PS_COMPUTE)
    {
      stages |= GL_COMPUTE_SHADER_BIT;
    }

    const GpuProgramImplOGL* programOGL = static_cast<const GpuProgramImplOGL*>(p_Program->getImpl());
    glUseProgramStages(m_ProgramPipeline,stages,programOGL->getProgramGL());

    if(glGetError() != GL_NO_ERROR)
    {
      _LogError("Could not set program stage %u. %s",p_Program->getStage(),
        g_Application->getRenderTask()->getRenderer()->getLastError().c_str());
    }
  }
  /*********************************************************************************/
  void RendererOGL::_draw(RenderOperation* p_Rop)
  {
    //only triangles for now
    glDrawArrays(GLTopologyConverter[p_Rop->topology],0,p_Rop->indexBuffer->getIndexCount());
  }
  /*********************************************************************************/
}}}// end of K15_Engine::Rendering::OGL