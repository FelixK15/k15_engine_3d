/**
 * @file K15_OpenGL_Emulation.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
 * @section LICENSE
 *
 * This program is free software= 0; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation= 0; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY= 0; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_OpenGL_Emulation.h"
#include "K15_OpenGL_GpuProgramBatchImpl.h"
#include "K15_OpenGL_GpuProgramImpl.h"

#include "K15_Application.h"
#include "K15_OpenGL_Renderer.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
    /*********************************************************************************/
    struct internal
    {
        struct ARB_separate_shader_objects
        {
            static uint32 MAX_PIPELINE_PROGRAMS = 32;
            static GpuProgramBatch* pipelinePrograms[MAX_PIPELINE_PROGRAMS] = {0};
            static uint32 shader_pipelines = 0;
            static uint32 active_pipeline = 0;
        };
    };
    /*********************************************************************************/

	/*********************************************************************************/
	void GLAPIENTRY _kglTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
	{
		glBindTexture(target, texture);
		glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
		glBindTexture(target, 0);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage)
	{
        //doesn't care where we bind the buffer to (http://www.opengl.org/wiki/Buffer_Object)
        Renderer* renderer = static_cast<Renderer*>(g_Application->getRenderer());
        GLuint previousBoundBuffer = renderer ? renderer->getBoundGLBuffer(GpuBuffer::BT_VERTEX_BUFFER) :
                                                GL_NONE;
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        glBindBuffer(GL_ARRAY_BUFFER, previousBoundBuffer);
	}
	/*********************************************************************************/
	void GLAPIENTRY _kglNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data)
	{
        //doesn't care where we bind the buffer to (http://www.opengl.org/wiki/Buffer_Object)
        Renderer* renderer = static_cast<Renderer*>(g_Application->getRenderer());
        GLuint previousBoundBuffer = renderer ? renderer->getBoundGLBuffer(GpuBuffer::BT_VERTEX_BUFFER) :
                                                GL_NONE;

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        glBindBuffer(GL_ARRAY_BUFFER, previousBoundBuffer);
    }
	/*********************************************************************************/
	void* GLAPIENTRY _kglMapNamedBufferEXT(GLuint buffer, GLenum access)
	{
        Renderer* renderer = static_cast<Renderer*>(g_Application->getRenderer());
        GLuint previousBoundBuffer = renderer ? renderer->getBoundGLBuffer(GpuBuffer::BT_VERTEX_BUFFER) :
                                                GL_NONE;

        // we can undbind the buffer after mapping it. (http://www.opengl.org/wiki/Buffer_Object#Mapping)
		void* bufferData = 0;
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        bufferData = glMapBuffer(GL_ARRAY_BUFFER, access);
        glBindBuffer(GL_ARRAY_BUFFER, previousBoundBuffer);
		return bufferData;
	}
	/*********************************************************************************/
	GLboolean GLAPIENTRY _kglUnmapNamedBufferEXT(GLuint buffer)
    {
        Renderer* renderer = static_cast<Renderer*>(g_Application->getRenderer());
        GLuint previousBoundBuffer = renderer ? renderer->getBoundGLBuffer(GpuBuffer::BT_VERTEX_BUFFER) :
                                                GL_NONE;

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        GLboolean unmapSuccessfully = glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, previousBoundBuffer);

		return unmapSuccessfully;
	}
	/*********************************************************************************/

    //GL_ARB_separate_shader_objects
    /*********************************************************************************/
    void GLAPIENTRY _kglGenProgramPipelines(GLsizei n, GLuint* pipelines)
    {
        K15_ASSERT(n + internal::ARB_separate_shader_objects::shader_pipelines
                   <= internal::ARB_separate_shader_objects::MAX_PIPELINE_PROGRAMS,
                   "Max pipeline programs reached.");

        for(int i = 0; i < n; ++i)
        {
            for(uint32 j = internal::ARB_separate_shader_objects::shader_pipelines;
                j < internal::ARB_separate_shader_objects::MAX_PIPELINE_PROGRAMS; ++j)
            {
                if(!internal::ARB_separate_shader_objects::pipelinePrograms[j])
                {
                    internal::ARB_separate_shader_objects::pipelinePrograms[j] =
                            K15_NEW GpuProgramBatch();

                    pipelines[i] = j;
                }
            }
        }
    }
    /*********************************************************************************/
    void GLAPIENTRY _kglBindProgramPipeline(GLuint pipeline)
    {
        K15_ASSERT(pipeline < internal::ARB_separate_shader_objects::MAX_PIPELINE_PROGRAMS,
                   "passes pipeline object out of bounds.");

        GpuProgramBatch* pipelineProgram =
                internal::ARB_separate_shader_objects::pipelinePrograms[pipeline];

        RendererBase* renderer = g_Application->getRenderer();
        renderer->bindGpuProgramBatch(pipelineProgram);
    }
    /*********************************************************************************/
    void GLAPIENTRY _kglDeleteProgramPipelines(GLsizei n, const GLuint* pipelines)
    {
        GpuProgramBatch* pipelineProgram = 0;
        for(int i = 0; i < n; ++i)
        {
            K15_ASSERT(pipelines[i] <= internal::ARB_separate_shader_objects::MAX_PIPELINE_PROGRAMS,
                       StringUtil::format("pipelines index %d out of bounds.", i));

            pipelineProgram = internal::ARB_separate_shader_objects::pipelinePrograms[pipelines[i]];
            K15_DELETE pipelineProgram;
            internal::ARB_separate_shader_objects::pipelinePrograms[pipelines[i]] = 0;
        }
    }
    /*********************************************************************************/
    void GLAPIENTRY _kglUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program)
    {
        K15_ASSERT(pipeline <= internal::ARB_separate_shader_objects::MAX_PIPELINE_PROGRAMS,
                   StringUtil::format("pipeline index %d out of bounds", pipeline));

        GpuProgramBatch* pipeline = internal::ARB_separate_shader_objects::pipelinePrograms[pipeline];

        K15_ASSERT(pipeline, StringUtil::format("pipeline index %d is invalid.", pipeline));

        Enum shaderType = 0xFFFFFFFF;

        if(stages == GL_VERTEX_SHADER_BIT)
        {
            shaderType = GpuProgram::PS_VERTEX;
        }
        else if(stages == GL_FRAGMENT_SHADER_BIT)
        {
            shaderTy[e = GpuProgram::PS_FRAGMENT;
        }

        K15_ASSERT(shaderType < 0xFFFFFFFF, StringUtil::format("invalid shader stage %d", stages));


        //stages will only contain one shader stage in the case of the k15 engine
        Renderer* renderer = static_cast<Renderer*>(g_Application->getRenderer());
        GpuProgram* shaderProgram = g_Application->getRenderer()->getBoundGpuProgram(shaderType);
        GpuProgramImpl* programImpl = static_cast<GpuProgramImpl*>(shaderProgram->getImpl());

        GLuint glShader = programImpl->getShaderGL();

        //delete old program and attach new program with new gl shader
        if(GpuProgram* previousProgram = pipeline->getGpuProgramByStage(shaderType))
        {
            pipeline->removeGpuProgram(shaderType);
            K15_DELETE previousProgram;
        }

        GpuProgram* newProgram = K15_NEW GpuProgram(shaderProgram->getName(), shaderType);
        GpuProgramImpl* newProgramImpl = static_cast<GpuProgramImpl*>(newProgram->getImpl());

        newProgramImpl->setShaderGL(glShader);
        pipeline->addGpuProgram(newProgram, true);
    }
    /*********************************************************************************/
    void GLAPIENTRY _kglActiveShaderProgram(GLuint pipeline, GLuint program)
    {

    }
    /*********************************************************************************/
    GLuint GLAPIENTRY _kglCreateShaderProgramv(GLenum type, GLsizei count, const GLchar ** strings)
    {

        return 0;
    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramParemteri(GLuint program, GLenum pname, GLint value)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform1i(GLuint program, GLint location, GLint x)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform2i(GLuint program, GLint location, GLint x, GLint y)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform1ui(GLuint program, GLint location, GLuint x)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglValidateProgramPipeline(GLuint pipeline)
    {

    }
    /*********************************************************************************/
    void GLAPIENTRY _kglGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog)
    {

    }
    /*********************************************************************************/

}}} //end of K15_Engine::Rendering::OpenGL namespace
