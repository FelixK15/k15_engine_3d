/**
 * @file K15_OpenGL_Emulation.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_OpenGL_Emulation_h_
#define _K15Engine_Renderer_OpenGL_Emulation_h_

#include "K15_OpenGL_Prerequisites.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
/*********************************************************************************/
//GL_EXT_direct_state_access
void GLAPIENTRY _kglTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void GLAPIENTRY _kglNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
void GLAPIENTRY _kglNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void* GLAPIENTRY _kglMapNamedBufferEXT(GLuint buffer, GLenum access);
GLboolean GLAPIENTRY _kglUnmapNamedBufferEXT(GLuint buffer);

//GL_ARB_separate_shader_objects
void GLAPIENTRY _kglGenProgramPipelines(GLsizei n, GLuint* pipelines);
void GLAPIENTRY _kglBindProgramPipeline(GLuint pipeline);
void GLAPIENTRY _kglDeleteProgramPipelines(GLsizei n, const GLuint* pipelines);
void GLAPIENTRY _kglUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);
void GLAPIENTRY _kglActiveShaderProgram(GLuint pipeline, GLuint program);
GLuint GLAPIENTRY _kglCreateShaderProgramv(GLenum type, GLsizei count, const GLchar ** strings);
void GLAPIENTRY _kglProgramParemteri(GLuint program, GLenum pname, GLint value);

void GLAPIENTRY _kglProgramUniform1i(GLuint program, GLint location, GLint x);
void GLAPIENTRY _kglProgramUniform2i(GLuint program, GLint location, GLint x, GLint y);
void GLAPIENTRY _kglProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z);
void GLAPIENTRY _kglProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);

void GLAPIENTRY _kglProgramUniform1ui(GLuint program, GLint location, GLuint x);
void GLAPIENTRY _kglProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y);
void GLAPIENTRY _kglProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
void GLAPIENTRY _kglProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);

void GLAPIENTRY _kglValidateProgramPipeline(GLuint pipeline);
void GLAPIENTRY _kglGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);

/*extern PFNGLPROGRAMUNIFORM1FPROC kglProgramUniform1f;
extern PFNGLPROGRAMUNIFORM2FPROC kglProgramUniform2f;
extern PFNGLPROGRAMUNIFORM3FPROC kglProgramUniform3f;
extern PFNGLPROGRAMUNIFORM4FPROC kglProgramUniform4f;

extern PFNGLPROGRAMUNIFORM1DPROC kglProgramUniform1d;
extern PFNGLPROGRAMUNIFORM2DPROC kglProgramUniform2d;
extern PFNGLPROGRAMUNIFORM3DPROC kglProgramUniform3d;
extern PFNGLPROGRAMUNIFORM4DPROC kglProgramUniform4d;

extern PFNGLPROGRAMUNIFORM1IVPROC kglProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM2IVPROC kglProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM3IVPROC kglProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM4IVPROC kglProgramUniform4iv;

extern PFNGLPROGRAMUNIFORM1UIVPROC kglProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2UIVPROC kglProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3UIVPROC kglProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4UIVPROC kglProgramUniform4uiv;

extern PFNGLPROGRAMUNIFORM1FVPROC kglProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM2FVPROC kglProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM3FVPROC kglProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM4FVPROC kglProgramUniform4fv;

extern PFNGLPROGRAMUNIFORM1DVPROC kglProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM2DVPROC kglProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM3DVPROC kglProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM4DVPROC kglProgramUniform4dv;

extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC	kglProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC	kglProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC	kglProgramUniformMatrix4fv;

extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC	kglProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC	kglProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC	kglProgramUniformMatrix4dv;*/



/*********************************************************************************/
}}} //end of K15_Engine::Rendering::OpenGL namespace
#endif //_K15Engine_Renderer_OpenGL_Emulation_h_
