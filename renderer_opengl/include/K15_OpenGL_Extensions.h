/**
 * @file K15_OpenGL_Extensions.h
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

#ifndef _K15Engine_RendererOGL_Extensions_h_
#define _K15Engine_RendererOGL_Extensions_h_

/*********************************************************************************/
//GL_ARB_debug_output
extern PFNGLDEBUGMESSAGECALLBACKPROC kglDebugMessageCallback;

//GL_ARB_separate_shader_objects//
extern PFNGLGENPROGRAMPIPELINESPROC	kglGenProgramPipelines;
extern PFNGLBINDPROGRAMPIPELINEPROC kglBindProgramPipeline;
extern PFNGLDELETEPROGRAMPIPELINESPROC kglDeleteProgramPipelines;
extern PFNGLUSEPROGRAMSTAGESPROC kglUseProgramStages;
extern PFNGLACTIVESHADERPROGRAMPROC	kglActiveShaderProgram;
extern PFNGLCREATESHADERPROGRAMVPROC kglCreateShaderProgramv;
extern PFNGLPROGRAMPARAMETERIPROC kglProgramParameteri;

extern PFNGLPROGRAMUNIFORM1IPROC kglProgramUniform1i;
extern PFNGLPROGRAMUNIFORM2IPROC kglProgramUniform2i;
extern PFNGLPROGRAMUNIFORM3IPROC kglProgramUniform3i;
extern PFNGLPROGRAMUNIFORM4IPROC kglProgramUniform4i;

extern PFNGLPROGRAMUNIFORM1UIPROC kglProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM2UIPROC kglProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM3UIPROC kglProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM4UIPROC kglProgramUniform4ui;

extern PFNGLPROGRAMUNIFORM1FPROC kglProgramUniform1f;
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
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC	kglProgramUniformMatrix4dv;

extern PFNGLVALIDATEPROGRAMPIPELINEPROC	kglValidateProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC kglGetProgramPipelineInfoLog;

//GL_ARB_vertex_array_object//
extern PFNGLGENVERTEXARRAYSPROC	kglGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC	kglBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC kglDeleteVertexArrays;

//GL_EXT_direct_state_access
extern PFNGLTEXTUREIMAGE1DEXTPROC kglTextureImage1DEXT;
extern PFNGLTEXTUREIMAGE2DEXTPROC kglTextureImage2DEXT;
extern PFNGLTEXTURESUBIMAGE1DEXTPROC kglTextureSubImage1DEXT;
extern PFNGLTEXTURESUBIMAGE2DEXTPROC kglTextureSubImage2DEXT;
extern PFNGLNAMEDBUFFERDATAEXTPROC kglNamedBufferDataEXT;
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC kglNamedBufferSubDataEXT;
extern PFNGLMAPNAMEDBUFFEREXTPROC kglMapNamedBufferEXT;
extern PFNGLUNMAPNAMEDBUFFEREXTPROC kglUnmapNamedBufferEXT;

//GL_ARB_sampler_objects
extern PFNGLGENSAMPLERSPROC	kglGenSamplers;
extern PFNGLDELETESAMPLERSPROC kglDeleteSamplers;
extern PFNGLBINDSAMPLERPROC kglBindSampler;
extern PFNGLSAMPLERPARAMETERIPROC kglSamplerParameteri;
extern PFNGLSAMPLERPARAMETERFPROC kglSamplerParameterf;
extern PFNGLSAMPLERPARAMETERIVPROC kglSamplerParameteriv;
extern PFNGLSAMPLERPARAMETERFVPROC kglSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIIVPROC kglSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC kglSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC kglGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERIVPROC kglGetSamplerParameteriv;
extern PFNGLGETSAMPLERPARAMETERFVPROC kglGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC kglGetSamplerParameterIuiv;
/*********************************************************************************/

#endif //_K15Engine_RendererOGL_Extensions_h_