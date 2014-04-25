/**
 * @file K15_WGL_Renderer.cpp
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

#include "K15_LogManager.h"

#include "K15_WGL_Renderer.h"
#include "K15_WGL_GpuBufferImpl.h"
#include "K15_WGL_GpuProgramImpl.h"
#include "K15_WGL_GpuProgramBatchImpl.h"
#include "K15_WGL_TextureImpl.h"
#include "K15_WGL_TextureSamplerImpl.h"
#include "K15_WGL_VertexDeclarationImpl.h"

#include "K15_IndexBuffer.h"
#include "K15_VertexBuffer.h"

#include "Win32\K15_RenderWindow_Win32.h"

namespace K15_Engine { namespace Rendering { namespace WGL {
	/*********************************************************************************/
	const GLenum Renderer::GLFunctionTestConverter[RendererBase::FT_COUNT] = {
		GL_NONE, 	  //FT_NONE
		GL_LESS,    //FT_LESS
		GL_LEQUAL,  //FT_LESS_EQUAL
		GL_EQUAL,   //FT_EQUAL
		GL_GEQUAL,  //FT_GREATER_EQUAL
		GL_GREATER  //FT_GREATER
	};//DepthTestConverter
	/*********************************************************************************/
	const GLenum Renderer::GLTopologyConverter[RenderOperation::T_COUNT] = {
		GL_POINTS,    //T_DOT
		GL_LINES,     //T_LINE
		GL_TRIANGLES, //T_TRIANGLE
		GL_TRIANGLE_STRIP, //T_TRIANGLESTRIP
		GL_QUADS      //T_QUAD
	}; //TopologyConverter
	/*********************************************************************************/
	const GLenum Renderer::GLBlendOperationConverter[AlphaState::BO_COUNT] = {
		GL_FUNC_ADD,              //BO_ADD
		GL_FUNC_SUBTRACT,         //BO_SUBTRACT
		GL_FUNC_REVERSE_SUBTRACT, //BO_REVERSE_SUBTRACT
		GL_MIN,                   //BO_MIN
		GL_MAX                    //BO_MAX
	}; //BlendOperationConverter
	/*********************************************************************************/
	const GLenum Renderer::GLBlendFunctionConverter[AlphaState::BF_COUNT] = {
		GL_ZERO,                      //BF_ZERO
		GL_ONE,                       //BF_ONE
     
		GL_SRC_COLOR,                 //BF_SRC_COLOR
		GL_ONE_MINUS_SRC_COLOR,       //BF_ONE_MINUS_SRC_COLOR
		GL_DST_COLOR,                 //BF_DST_COLOR
		GL_ONE_MINUS_DST_COLOR,       //BF_ONE_MINUS_DST_COLOR

		GL_SRC_ALPHA,                 //BF_SRC_ALPHA
		GL_ONE_MINUS_SRC_ALPHA,       //BF_ONE_MINUS_SRC_ALPHA
		GL_DST_ALPHA,                 //BF_DST_ALPHA
		GL_ONE_MINUS_DST_ALPHA,       //BF_ONE_MINUS_DST_ALPHA

		GL_CONSTANT_COLOR,            //BF_CONSTANT_COLOR
		GL_ONE_MINUS_CONSTANT_COLOR,  //BF_ONE_MINUS_CONSTANT_COLOR

		GL_CONSTANT_ALPHA,            //BF_CONSTANT_ALPHA
		GL_ONE_MINUS_CONSTANT_ALPHA   //BF_ONE_MINUS_CONSTANT_ALPHA
	}; //BlendFunctionConverter
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
		msg = renderer->getLastError() + "\n" + msg;

		if(renderer)
		{
			renderer->setLastError(msg);
		}
	}
	/*********************************************************************************/

	/*********************************************************************************/
	Renderer::Renderer()
		: RendererBase(),
		  m_RenderContext(0),
		  m_DeviceContext(0),
		  m_ProgramPipeline(0),
		  m_VertexArray(0)
	{
		
	}
	/*********************************************************************************/
	Renderer::~Renderer()
	{

	}
	/*********************************************************************************/
	void Renderer::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear color, depth and stencil buffer
	}
	/*********************************************************************************/
	void Renderer::endFrame()
	{
		SwapBuffers(m_DeviceContext);
	}
	/*********************************************************************************/
	bool Renderer::_initialize()
	{
		K15_ASSERT(m_RenderWindow,
			"Render window has not been set. Can not initialize renderer without having a render window.");
		
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

		if(!GLEW_VERSION_3_3)
		{
			_LogError("OpenGL 3.3 is not supported.");
			return false;
		}

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

		if(GL_ARB_separate_shader_objects)
		{
			//create program pipeline
			glGenProgramPipelines(1,&m_ProgramPipeline);
			glBindProgramPipeline(m_ProgramPipeline);

			glGenVertexArrays(1,&m_VertexArray);
			glBindVertexArray(m_VertexArray);
		}
		else
		{
			_LogError("Can't use gl extension GL_ARB_separate_shader_objects");
			return false;
		}

		const Resolution& resolution = renderwindow->getResolution();
		//force viewport resizing
		onResolutionChanged(resolution);
		setClearColor(0.8f,0.2224f,0.005f);
		glEnable(GL_STENCIL_TEST); //enable stencil testing
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		return m_LastError.empty();
	}
	/*********************************************************************************/
	void Renderer::_shutdown()
	{
		if(m_ProgramPipeline != 0)
		{
			glBindProgramPipeline(0);
			glDeleteProgramPipelines(1,&m_ProgramPipeline);
			m_ProgramPipeline = 0;
		}

		if(m_VertexArray != 0)
		{
			glBindVertexArray(0);
			glDeleteVertexArrays(1,&m_VertexArray);
			m_VertexArray = 0;
		}
		
		if(m_RenderContext)
		{
			wglMakeCurrent(0,0);
			wglDeleteContext(m_RenderContext);
		}
	}
	/*********************************************************************************/
	void Renderer::onResolutionChanged(const Resolution& p_Resolution)
	{
		glViewport(0,0,p_Resolution.width,p_Resolution.height);
	}
	/*********************************************************************************/
	bool Renderer::createDummyContext(HWND* p_Hwnd,HDC* p_DC)
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
	GpuBufferImplBase* Renderer::createGpuBufferImpl()
	{
		return K15_NEW GpuBufferImpl();
	}
	/*********************************************************************************/
	TextureImplBase* Renderer::createTextureImpl()
	{
		return K15_NEW TextureImpl();
	}
	/*********************************************************************************/
	GpuProgramImplBase* Renderer::createGpuProgramImpl()
	{
		return K15_NEW GpuProgramImpl();
	}
	/*********************************************************************************/
	TextureSamplerImplBase* Renderer::createTextureSamplerImpl()
	{
		return K15_NEW TextureSamplerImpl();
	}
	/*********************************************************************************/
	VertexDeclarationImplBase* Renderer::createVertexDeclarationImpl()
	{
		return K15_NEW VertexDeclarationImpl();
	}
	/*********************************************************************************/
	GpuProgramBatchImplBase* Renderer::createGpuProgramBatchImpl()
	{
		return K15_NEW GpuProgramBatchImpl();
	}
	/*********************************************************************************/
	void Renderer::_setAlphaState(const AlphaState& p_AlphaState)
	{
		bool changeState = false;
		if(p_AlphaState.getEnabled() != m_AlphaState.getEnabled())
		{
			if(!p_AlphaState.getEnabled())
			{
				glDisable(GL_BLEND);
			}
			else
			{
				glEnable(GL_BLEND);
				changeState = true;
			}
		}
    
		changeState = m_AlphaState.getEnabled();

		if(changeState)
		{
			glBlendFunc(GLBlendFunctionConverter[p_AlphaState.getSourceBlendFunction()],
			GLBlendFunctionConverter[p_AlphaState.getDestinationBlendFunction()]);

			glBlendColor(p_AlphaState.getConstantColor().RedComponent,
			p_AlphaState.getConstantColor().GreenComponent,
			p_AlphaState.getConstantColor().BlueComponent,
			p_AlphaState.getConstantColor().AlphaComponent);

			glBlendEquation(GLBlendOperationConverter[p_AlphaState.getBlendOperation()]);
		}
	}
	/*********************************************************************************/
	void Renderer::_setDepthState(const DepthState& p_DepthState)
	{
		if(p_DepthState.getEnabled() != m_DepthState.getEnabled())
		{
			if(p_DepthState.getEnabled())
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GLFunctionTestConverter[p_DepthState.getFunction()]);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			} 
		}
		else
		{
			if(m_DepthState.getEnabled())
			{
				glDepthFunc(GLFunctionTestConverter[p_DepthState.getFunction()]);
			}
		}
	}
	/*********************************************************************************/
	void Renderer::_setRenderWindow(RenderWindowBase* p_RenderWindow)
	{
	}
	/*********************************************************************************/
	void Renderer::_setRenderTarget(RenderTarget* p_RenderTarget)
	{
	}
	/*********************************************************************************/
	void Renderer::_setActiveCamera(CameraComponent* p_Camera)
	{
	}
	/*********************************************************************************/
	void Renderer::_setCullingMode(Enum p_CullingMode)
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
	void Renderer::_setBackFaceCullingEnabled(bool p_Enabled)
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
	void Renderer::_setFillMode(Enum p_FillMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK,p_FillMode == FM_SOLID ? GL_FILL : GL_LINE);
	}
	/*********************************************************************************/
	void Renderer::_setFrameBufferPixelFormat(Enum p_PixelFormat)
	{
	}
	/*********************************************************************************/
	void Renderer::_setDepthBufferPixelFormat(Enum p_DepthFormat)
	{
	}
	/*********************************************************************************/
	void Renderer::_setStencilBufferPixelFormat(Enum p_StencilFormat)
	{
	}
	/*********************************************************************************/
	void Renderer::_setClearColor(const ColorRGBA& p_ClearColor)
	{
		glClearColor(p_ClearColor.RedComponent,p_ClearColor.GreenComponent,p_ClearColor.BlueComponent,1.0f);
	}
	/*********************************************************************************/
	void Renderer::_bindBuffer(GpuBuffer* p_Buffer, Enum p_BufferType)
	{
		const GpuBufferImpl* bufferOGL = static_cast<const GpuBufferImpl*>(p_Buffer->getImpl());
		GLenum target = GpuBufferImpl::GLBufferTypeConverter[p_BufferType];

		if(p_Buffer)
		{
			glBindBuffer(target,bufferOGL->getBufferGL());
		}
		else
		{
			glBindBuffer(target,0);
		}
	}
	/*********************************************************************************/
	void Renderer::_bindProgram(GpuProgram* p_Program, Enum p_ProgramStage)
	{
		GLbitfield stages = GL_NONE;

		if(p_ProgramStage == GpuProgram::PS_VERTEX)
		{
			stages |= GL_VERTEX_SHADER_BIT;
		}
		else if(p_ProgramStage == GpuProgram::PS_FRAGMENT)
		{
			stages |= GL_FRAGMENT_SHADER_BIT;
		}
		else if(p_ProgramStage == GpuProgram::PS_GEOMETRY)
		{
			stages |= GL_GEOMETRY_SHADER_BIT;
		}
		else if(p_ProgramStage == GpuProgram::PS_COMPUTE)
		{
			stages |= GL_COMPUTE_SHADER_BIT;
		}

		const GpuProgramImpl* programOGL = static_cast<const GpuProgramImpl*>(p_Program->getImpl());

		if(p_Program)
		{
			glUseProgramStages(m_ProgramPipeline,stages,programOGL->getProgramGL());
		}
		else
		{
			glUseProgramStages(m_ProgramPipeline,stages,0);
		}
	}
	/*********************************************************************************/
	void Renderer::_drawIndexed(uint32 p_Offset)
	{
		IndexBuffer* indexBuffer = (IndexBuffer*)m_GpuBuffers[GpuBuffer::BT_INDEX_BUFFER];

		if(p_Offset != 0)
		{
			glDrawElementsBaseVertex(GLTopologyConverter[m_Topology],indexBuffer->getIndexCount(),
			GpuBufferImpl::GLIndexBufferTypeConverter[indexBuffer->getIndexType()],0,p_Offset);
		}
		else
		{
			glDrawElements(GLTopologyConverter[m_Topology],
			indexBuffer->getIndexCount(),GpuBufferImpl::GLIndexBufferTypeConverter[indexBuffer->getIndexType()],0);
		}
	}
	/*********************************************************************************/
	void Renderer::_drawDirect(uint32 p_Offset)
	{
		VertexBuffer* vertexBuffer = (VertexBuffer*)m_GpuBuffers[GpuBuffer::BT_VERTEX_BUFFER];
		glDrawArrays(GLTopologyConverter[m_Topology],p_Offset,vertexBuffer->getVertexCount());
	}
	/*********************************************************************************/
// 	void RendererOGL::_bindProgramParameter(const GpuProgramParameter& p_Parameter,const RawData& p_Data)
// 	{
// 		
// 	}
	/*********************************************************************************/
	void Renderer::_setVertexDeclaration(VertexDeclaration* p_Declaration)
	{
		VertexDeclarationImpl* impl = static_cast<VertexDeclarationImpl*>(p_Declaration->getImpl());
		
		if(m_VertexDeclaration)
		{
			if(m_VertexDeclaration->getElementCount() < p_Declaration->getElementCount())
			{
				//disable old attribts
				uint32 diff = m_VertexDeclaration->getElementCount() - p_Declaration->getElementCount();
				for(uint32 i = diff;i != m_VertexDeclaration->getElementCount();++i)
				{
					glDisableVertexAttribArray(i);
				}
			}
		}

		for(uint32 i = 0;i < p_Declaration->getElementCount();++i)
		{
			const VertexElement& element = p_Declaration->getElement(i);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,element.size,VertexDeclarationImpl::GLVertexElementTypeConverter[element.type],GL_FALSE,p_Declaration->getVertexSize(),(void*)element.offset);
		}
	}
	/*********************************************************************************/
	void Renderer::_bindTexture(Texture* p_Texture, Enum p_Type)
	{
		GLenum textureType = TextureImpl::GLTextureTypeConverter[p_Type];

		if(p_Texture)
		{
			TextureImpl* texImpl = static_cast<TextureImpl*>(p_Texture->getImpl());
			glActiveTexture(GL_TEXTURE0 + p_Texture->getSlot());
			glBindTexture(textureType,texImpl->getTextureHandle());
		}
		else
		{
			glBindTexture(textureType,0);
		}
	}	
	/*********************************************************************************/
	void Renderer::_bindProgramBatch(GpuProgramBatch* p_ProgramBatch)
	{
		GpuProgram* program = 0;
		const GpuProgramBatch::GpuProgramList& programs = p_ProgramBatch->getGpuProgramList();

		for(GpuProgramBatch::GpuProgramList::const_iterator iter = programs.begin();iter != programs.end();++iter)
		{
			program = (*iter);

			bindGpuProgram(program,program->getStage());
		}
	}
	/*********************************************************************************/
	void Renderer::_updateGpuProgramParameter(const RawData& p_Data, const GpuProgramParameter& p_Parameter)
	{
		const GpuProgramImpl* glProgram =  static_cast<const GpuProgramImpl*>(p_Parameter.getGpuProgram()->getImpl());
		if(p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_2D)
		{
			GLint value = (int)p_Data.data;
			glProgramUniform1i(glProgram->getProgramGL(),p_Parameter.getRegisterIndex(),value);
			
			TextureSampler* sampler = m_BoundSamplers.at(value);
			if(sampler)
			{
				TextureSamplerImpl* glSampler = static_cast<TextureSamplerImpl*>(sampler->getImpl());

				glBindSampler(value,glSampler->getHandle());
			}
		}
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::WGL