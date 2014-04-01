/**
 * @file K15_Language.h
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
 */

#include "K15_LogManager.h"

#include "K15_GLES2_Renderer.h"
#include "K15_GLES2_GpuBufferImpl.h"
#include "K15_GLES2_GpuProgramImpl.h"
#include "K15_GLES2_TextureImpl.h"
#include "K15_GLES2_TextureSamplerImpl.h"
#include "K15_GLES2_VertexDeclarationImpl.h"

#include "K15_IndexBuffer.h"
#include "K15_VertexBuffer.h"

#include "Android\K15_RenderWindow_Android.h"

namespace K15_Engine { namespace Rendering { namespace GLES2 {
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
		GL_POINTS,			//T_DOT
		GL_LINES,			//T_LINE
		GL_TRIANGLES,		//T_TRIANGLE
		GL_TRIANGLE_STRIP,	//T_TRIANGLESTRIP
		GL_NONE				//T_QUAD		// no quads in GLES 2.0
	}; //TopologyConverter
	/*********************************************************************************/
	const GLenum Renderer::GLBlendOperationConverter[AlphaState::BO_COUNT] = {
		GL_FUNC_ADD,				//BO_ADD
		GL_FUNC_SUBTRACT,			//BO_SUBTRACT
		GL_FUNC_REVERSE_SUBTRACT,	//BO_REVERSE_SUBTRACT
		GL_NONE,					//BO_MIN	// nope...not for GLES 2.0
		GL_NONE						//BO_MAX	// nope...not for GLES 2.0
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

	/*********************************************************************************/
	Renderer::Renderer()
		: RendererBase(),
		m_Display(EGL_NO_DISPLAY),
		m_Context(EGL_NO_CONTEXT),
		m_Surface(EGL_NO_SURFACE)
	{
		K15_DEBUG("GLES2::Renderer()");
	}
	/*********************************************************************************/
	Renderer::~Renderer()
	{

	}
	/*********************************************************************************/
	bool Renderer::initialize()
	{
    K15_ASSERT(GL_EXT_separate_shader_objects,"GL_EXT_separate_shader_objects is not supported by this device.");

		K15_ASSERT(m_RenderWindow,
			"Render window has not been set. Can not initialize renderer without having a render window.");

		m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		EGLint ver_Major = 0;
		EGLint ver_Minor = 0;

		if(eglInitialize(m_Display,&ver_Major,&ver_Minor) == EGL_FALSE)
		{
			_LogError("Could not initialize GLES Display.");
			return false;
		}

		_LogSuccess("Successfully initialize GLES Display.");
		_LogSuccess("GLES Version: %i.%i",ver_Major,ver_Minor);

		EGLConfig config = 0;
		EGLint configCount = 0;
		EGLint format = 0;

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};

		if(eglChooseConfig(m_Display,attribs,&config,1,&configCount) == EGL_FALSE)
		{
			_LogError("Could not get GLES config.");
			return false;
		}

		if(eglGetConfigAttrib(m_Display,config,EGL_NATIVE_VISUAL_ID,&format) == EGL_FALSE)
		{
			_LogError("Could not get config attributes 'EGL_NATIVE_VISUAL_ID'.");
			return false;
		}

#if defined K15_OS_ANDROID
		RenderWindow_Android* window = static_cast<RenderWindow_Android*>(m_RenderWindow);
		ANativeWindow_setBuffersGeometry(window->getNativeWindow(),0,0,format);
		if((m_Surface = eglCreateWindowSurface(m_Display,config,window->getNativeWindow(),0)) == 0)
#endif //K15_OS_ANDROID
		{
			_LogError("Could not create GLES window surface.");
			return false;
		}

		if((m_Context = eglCreateContext(m_Display,config,0,0)) == 0)
		{
			_LogError("Could not create GLES context.");
			return false;
		}

		if(eglMakeCurrent(m_Display,m_Surface,m_Surface,m_Context) == EGL_FALSE)
		{
			_LogError("Could not set GLES context as current context.");
			return false;
		}

		GLint width,height;
		eglQuerySurface(m_Display,m_Surface,EGL_WIDTH,&width);
		eglQuerySurface(m_Display,m_Surface,EGL_HEIGHT,&height);

		Resolution resolution;
		resolution.width = width;
		resolution.height = height;

		window->setResolution(resolution,false);

		return true;
	}
	/*********************************************************************************/
	void Renderer::shutdown()
	{
		if(m_Display)
		{
			eglMakeCurrent(m_Display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

			if(m_Surface)
			{
				eglDestroySurface(m_Display,m_Surface);
				m_Surface = EGL_NO_SURFACE;
			}

			if(m_Context)
			{
				eglDestroyContext(m_Display,m_Context);
				m_Context = EGL_NO_CONTEXT;
			}

			if(m_Display)
			{
				eglTerminate(m_Display);
				m_Display = EGL_NO_DISPLAY;
			}
		}
	}
	/*********************************************************************************/
	void Renderer::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear color, depth and stencil buffer
	}
	/*********************************************************************************/
	void Renderer::endFrame()
	{
		eglSwapBuffers(m_Display,m_Surface);
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
		_LogWarning("setFillMode is not supported for GLES2.");
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
	void Renderer::_drawIndexed(uint32 p_Offset)
	{
		IndexBuffer* indexBuffer = (IndexBuffer*)m_GpuBuffers[GpuBuffer::BT_INDEX_BUFFER];

		glDrawElements(GLTopologyConverter[m_Topology],
			indexBuffer->getIndexCount(),GpuBufferImpl::GLIndexBufferTypeConverter[indexBuffer->getIndexType()],0);
	}
	/*********************************************************************************/
	void Renderer::_drawDirect(uint32 p_Offset)
	{
		VertexBuffer* vertexBuffer = (VertexBuffer*)m_GpuBuffers[GpuBuffer::BT_VERTEX_BUFFER];
		glDrawArrays(GLTopologyConverter[m_Topology],p_Offset,vertexBuffer->getVertexCount());
	}
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
	void Renderer::_setRenderWindow(RenderWindowBase* p_RenderWindow)
	{

	}
	/*********************************************************************************/
	void Renderer::onResolutionChanged(const Resolution& p_Resolution)
	{
		glViewport(0,0,p_Resolution.width,p_Resolution.height);
	}
	/*********************************************************************************/
	void Renderer::_setAlphaState( const AlphaState& p_AlphaState )
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
	void Renderer::_setRenderTarget(RenderTarget* p_RenderTarget)
	{

	}
	/*********************************************************************************/
	void Renderer::_setActiveCamera(CameraComponent* p_Camera)
	{

	}
	/*********************************************************************************/
	void Renderer::_setTopology(Enum p_Topology)
	{

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
	void Renderer::_bindProgram(GpuProgram* p_Program, Enum p_ProgramType)
	{

	}
	/*********************************************************************************/
	void Renderer::_bindProgramParameter(const GpuProgramParameter& p_Parameter)
	{

	}
	/*********************************************************************************/
	void Renderer::_bindTexture(Texture* p_Texture, Enum p_Type)
	{

	}
	/*********************************************************************************/
	void Renderer::_bindTextureSampler(TextureSampler* p_Sampler, Enum p_Slot)
	{

	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::GLES2 namespace