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

#include "K15_OpenGL_Renderer.h"
#include "K15_OpenGL_GpuBufferImpl.h"
#include "K15_OpenGL_GpuProgramImpl.h"
#include "K15_OpenGL_GpuProgramBatchImpl.h"
#include "K15_OpenGL_TextureImpl.h"
#include "K15_OpenGL_TextureSamplerImpl.h"
#include "K15_OpenGL_VertexDeclarationImpl.h"

#include "K15_IndexBuffer.h"
#include "K15_VertexBuffer.h"

#include "K15_RenderWindow.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
	/*********************************************************************************/
	const GLenum Renderer::GLFunctionTestConverter[RendererBase::FT_COUNT] = {
		GL_NONE, 		//FT_NONE
		GL_LESS,		//FT_LESS
		GL_LEQUAL,		//FT_LESS_EQUAL
		GL_EQUAL,		//FT_EQUAL
		GL_GEQUAL,		//FT_GREATER_EQUAL
		GL_GREATER		//FT_GREATER
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
        kglSwapBuffers();
	}
	/*********************************************************************************/
	bool Renderer::_initialize()
	{
		if(kglInit(32,24,8))
    {
      PFNGLDEBUGMESSAGECALLBACKARBPROC debugfunc = (PFNGLDEBUGMESSAGECALLBACKARBPROC)kglGetProcAddress("glDebugMessageCallbackARB");

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
        K15_LOG_ERROR("Can't use gl extension GL_ARB_separate_shader_objects");
        return false;
      }

      //force viewport resizing
      _resolutionChanged(RenderWindow::getResolution());
      glEnable(GL_STENCIL_TEST); //enable stencil testing
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);

      glEnable(GL_POLYGON_OFFSET_FILL); //enable glPolygonOffset for filled polys
      return m_LastError.empty();
    }

    return true;
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
		
		kglShutdown();
	}
	/*********************************************************************************/
	void Renderer::_resolutionChanged(const Resolution& p_Resolution)
	{
		glViewport(0,0,p_Resolution.width,p_Resolution.height);
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
		RenderWindow::getHasFocus();
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
				glPolygonOffset(0.0f,p_DepthState.getBias());
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
				glPolygonOffset(0.0f,p_DepthState.getBias());
			}
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
	void Renderer::_setVertexDeclaration(VertexDeclaration* p_Declaration)
	{
		VertexDeclarationImpl* impl = static_cast<VertexDeclarationImpl*>(p_Declaration->getImpl());
		int index = 0;
		const char* semanticname = 0;
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
			
			semanticname = VertexElement::SemanticNames[element.semantic];
			GpuProgramImpl* impl = static_cast<GpuProgramImpl*>(m_GpuPrograms[GpuProgram::PS_VERTEX]->getImpl());
			GLuint program = impl->getProgramGL();
			if((index = glGetAttribLocation(program,semanticname)) != -1)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,element.size,VertexDeclarationImpl::GLVertexElementTypeConverter[element.type],GL_FALSE,p_Declaration->getVertexSize(),(void*)element.offset);
			}
			else
			{
				K15_LOG_ERROR("Could not set vertex attribute \"%s\" for GpuProgram \"%s\".",
					semanticname,m_GpuPrograms[GpuProgram::PS_VERTEX]->getName().c_str());
			}
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
	void Renderer::_updateGpuProgramParameter(const GpuProgramParameter& p_Parameter)
	{
		const GpuProgramImpl* glProgram =  static_cast<const GpuProgramImpl*>(p_Parameter.getGpuProgram()->getImpl());
		GLuint program = glProgram->getProgramGL();
		if(p_Parameter.getType() == GpuProgramParameter::VT_UNKNOW)
		{
		  K15_LOG_ERROR("GpuProgramParameter \"%s\" from GpuProgram \"%s\" is unknown.",
			p_Parameter.getName().c_str(),
			p_Parameter.getGpuProgram()->getName().c_str());
		}
		else if(!p_Parameter.getData())
		{
			K15_LOG_ERROR("GpuProgramParameter \"%s\" from GpuProgram \"%s\" has no data.",
				p_Parameter.getName().c_str(),
				p_Parameter.getGpuProgram()->getName().c_str());
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_INT  ||
				p_Parameter.getType() == GpuProgramParameter::VT_BOOL ||
				p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_1D ||
				p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_2D ||
				p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_3D)
		{
		  GLint value = p_Parameter.getType() != GpuProgramParameter::VT_BOOL ? 
			*(GLint*)p_Parameter.getData() :
			(*(bool*)p_Parameter.getData()) ? GL_TRUE : GL_FALSE;

		  glProgramUniform1i(program, p_Parameter.getRegisterIndex(), value);

		  if(p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_1D ||
			p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_2D ||
			p_Parameter.getType() == GpuProgramParameter::VT_SAMPLER_3D)
		  {
			TextureImpl* glTex = static_cast<TextureImpl*>(m_BoundTextures[value]->getImpl());
			TextureSamplerImpl* glSampler = static_cast<TextureSamplerImpl*>(m_BoundSamplers[glTex->getTexture()->getTextureSamplerSlot()]->getImpl());

			glBindSampler(value,glSampler->getHandle());
		  }
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_FLOAT)
		{
		  GLfloat value = *(GLfloat*)p_Parameter.getData();

		  glProgramUniform1f(program, p_Parameter.getRegisterIndex(),value);
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR2)
		{
		  Vector2 value = *(Vector2*)p_Parameter.getData();
      
		  glProgramUniform2f(program, p_Parameter.getRegisterIndex(),value.x,value.y);
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR3)
		{
		  Vector3 value = *(Vector3*)p_Parameter.getData();

		  glProgramUniform3f(program, p_Parameter.getRegisterIndex(),value.x,value.y,value.z);
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_VECTOR4)
		{
		  Vector4 value = *(Vector4*)p_Parameter.getData();

		  glProgramUniform4f(program, p_Parameter.getRegisterIndex(),value.x,value.y,value.z,value.w);
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX2)
		{
		  glProgramUniformMatrix2fv(program, p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX3)
		{
		  glProgramUniformMatrix3fv(program, p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
		}
		else if(p_Parameter.getType() == GpuProgramParameter::VT_MATRIX4)
		{
		  glProgramUniformMatrix4fv(program, p_Parameter.getRegisterIndex(),1,GL_FALSE,(float*)p_Parameter.getData());
		}
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Rendering::WGL
