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
#include "K15_GLES2_GpuProgramBatchImpl.h"
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
		m_Surface(EGL_NO_SURFACE),
		m_MaxVertexAttribs(0)
	{
		
	}
	/*********************************************************************************/
	Renderer::~Renderer()
	{

	}
	/*********************************************************************************/
	bool Renderer::_initialize()
	{
		K15_ASSERT(m_RenderWindow,
			"Render window has not been set. Can not initialize renderer without having a render window.");

		m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		EGLint ver_Major = 0;
		EGLint ver_Minor = 0;

		if(eglInitialize(m_Display,&ver_Major,&ver_Minor) == EGL_FALSE)
		{
			_LogError("Could not initialize EGL Display.");
			return false;
		}

		_LogSuccess("Successfully initialize EGL Display.");
		_LogSuccess("EGL Version: %i.%i",ver_Major,ver_Minor);

		EGLConfig config = 0;
		EGLint configCount = 0;
		EGLint format = 0;

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_DEPTH_SIZE, 24,
			EGL_STENCIL_SIZE, 8,
			EGL_NONE
		};

		if(eglChooseConfig(m_Display,attribs,&config,1,&configCount) == EGL_FALSE)
		{
			_LogError("Could not get EGL config.");
			return false;
		}
		
		_LogSuccess("EGL config retrieved.");

		if(eglGetConfigAttrib(m_Display,config,EGL_NATIVE_VISUAL_ID,&format) == EGL_FALSE)
		{
			_LogError("Could not get config attributes 'EGL_NATIVE_VISUAL_ID'.");
			return false;
		}

		_LogSuccess("Retrieved 'EGL_NATIVE_VISUAL_ID'. (%i)",format);

#if defined K15_OS_ANDROID
		RenderWindow_Android* window = static_cast<RenderWindow_Android*>(m_RenderWindow);
		ANativeWindow_setBuffersGeometry(window->getNativeWindow(),0,0,format);
		if((m_Surface = eglCreateWindowSurface(m_Display,config,window->getNativeWindow(),0)) == 0)
#endif //K15_OS_ANDROID
		{
			_LogError("Could not create EGL window surface.");
			return false;
		}

		_LogSuccess("Created EGL window surface.");

		const EGLint contextAttribList[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		if((m_Context = eglCreateContext(m_Display,config,0,contextAttribList)) == 0)
		{
			_LogError("Could not create GLES context.");
			return false;
		}

		_LogSuccess("Created GLES context.");

		if(eglMakeCurrent(m_Display,m_Surface,m_Surface,m_Context) == EGL_FALSE)
		{
			_LogError("Could not set GLES context as current context.");
			return false;
		}

		_LogSuccess("Set GLES context as current context.");

		const unsigned char* glesVersion = glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&m_MaxVertexAttribs);

		_LogSuccess("GLES Version: %s",glesVersion);

		GLint width,height;
		eglQuerySurface(m_Display,m_Surface,EGL_WIDTH,&width);
		eglQuerySurface(m_Display,m_Surface,EGL_HEIGHT,&height);

		Resolution resolution;
		resolution.width = width;
		resolution.height = height;

		_LogNormal("Setting resolution to %ix%i...",width,height);
		window->setResolution(resolution,false);

		_LogDebug("Supported OpenGL ES Extensions:%s",glGetString(GL_EXTENSIONS));

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		_checkForError();
		return true;
	}
	/*********************************************************************************/
	void Renderer::_shutdown()
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
	GpuProgramBatchImplBase* Renderer::createGpuProgramBatchImpl()
	{
		return K15_NEW GpuProgramBatchImpl();
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
			//glFrontFace(GL_CCW);
		}
		else
		{
			//glFrontFace(GL_CW);
		}
	}
	/*********************************************************************************/
	void Renderer::_setBackFaceCullingEnabled(bool p_Enabled)
	{
		if(p_Enabled)
		{
			//glEnable(GL_CULL_FACE);
		}
		else
		{
			//glDisable(GL_CULL_FACE);
		}
	}
	/*********************************************************************************/
	void Renderer::_setFillMode(Enum p_FillMode)
	{
		
	}
	/*********************************************************************************/
	void Renderer::_setClearColor(const ColorRGBA& p_ClearColor)
	{
		glClearColor((float)p_ClearColor.R / 255.f,
				     (float)p_ClearColor.G / 255.f,
					 (float)p_ClearColor.B / 255.f,
					 1.0f);
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

		_checkForError();
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

		if(m_GpuProgramBatch)
		{
			GLint program = ((GLES2::GpuProgramBatchImpl*)m_GpuProgramBatch->getImpl())->getProgram();

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
				const char* semanticName = 0;
				GLuint index = 0;

				semanticName = VertexElement::SemanticNames[element.semantic];

				index = glGetAttribLocation(program,semanticName);

				if(index == -1)
				{
					_LogError("Could not find semantic \"%s\" in vertex shader \"%s\".",semanticName,m_GpuProgramBatch->getGpuProgramByStage(GpuProgram::PS_VERTEX)->getName().c_str());
				}
				else
				{
					//_LogDebug("Enabling vertex attribute %i for semantic \"%s\".",index,semanticName);
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,element.size,VertexDeclarationImpl::GLVertexElementTypeConverter[element.type],GL_FALSE,p_Declaration->getVertexSize(),(void*)element.offset);
				}	
			}
		}

		_checkForError();
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

			glBlendColor((float)p_AlphaState.getConstantColor().R / 255.f,
				(float)p_AlphaState.getConstantColor().G / 255.f,
				(float)p_AlphaState.getConstantColor().B / 255.f,
				(float)p_AlphaState.getConstantColor().A / 255.f);

			glBlendEquation(GLBlendOperationConverter[p_AlphaState.getBlendOperation()]);
		}

		_checkForError();
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
	void Renderer::_bindProgramBatch(GpuProgramBatch* p_Program)
	{
		GLint program = ((GpuProgramBatchImpl*)p_Program->getImpl())->getProgram();

		//K15_ASSERT(program > 0,"GpuProgramBatch has no valid OpenGL program object");

		glUseProgram(program);

		//rebound declaration
		_setVertexDeclaration(getVertexDeclaration());

		_checkForError();
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

		_checkForError();
	}
	/*********************************************************************************/
	void Renderer::_updateGpuProgramParameter(const GpuProgramParameter& p_Parameter)
	{
		if(p_Parameter.getType() == GpuProgramParameter::VT_INT ||
	            p_Parameter.getType() == GpuProgramParameter::VT_BOOL)
	    {
	      GLint value = p_Parameter.getType() == GpuProgramParameter::VT_INT ? 
	        *(GLint*)p_Parameter.getData() :
	        (*(bool*)p_Parameter.getData()) ? GL_TRUE : GL_FALSE;

	      glUniform1i(p_Parameter.getRegisterIndex(),value);
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_FLOAT)
	    {
	      GLfloat value = *(GLfloat*)p_Parameter.getData();

	      glUniform1f(p_Parameter.getRegisterIndex(),value);
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR2)
	    {
	      Vector2 value = *(Vector2*)p_Parameter.getData();
	      
	      glUniform2f(p_Parameter.getRegisterIndex(),value.x,value.y);
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR3)
	    {
	      Vector3 value = *(Vector3*)p_Parameter.getData();

	      glUniform3f(p_Parameter.getRegisterIndex(),value.x,value.y,value.z);
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR4)
	    {
	      Vector4 value = *(Vector4*)p_Parameter.getData();

	      glUniform4f(p_Parameter.getRegisterIndex(),value.x,value.y,value.z,value.w);
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX2)
	    {
	      glUniformMatrix2fv(p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX3)
	    {
	      glUniformMatrix3fv(p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
	    }
	    else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX4)
	    {
	      glUniformMatrix4fv(p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
	    }

		_checkForError();
	}
	/*********************************************************************************/
	void Renderer::_checkForError()
	{
		#if defined K15_DEBUG
			GLenum error = glGetError();
			if(error != GL_NO_ERROR)
			{
				_LogError("GLES2 Error:\"%d\".",error);
			}
		#endif //K15_DEBUG
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::GLES2 namespace