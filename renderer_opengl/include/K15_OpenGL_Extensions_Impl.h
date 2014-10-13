/**
 * @file K15_OpenGL_Extensions.h
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

/*********************************************************************************/
//GL_ARB_debug_output
PFNGLDEBUGMESSAGECALLBACKPROC kglDebugMessageCallback = 0;

//GL_ARB_separate_shader_objects//
PFNGLGENPROGRAMPIPELINESPROC	kglGenProgramPipelines= 0;
PFNGLBINDPROGRAMPIPELINEPROC kglBindProgramPipeline= 0;
PFNGLDELETEPROGRAMPIPELINESPROC kglDeleteProgramPipelines= 0;
PFNGLUSEPROGRAMSTAGESPROC kglUseProgramStages= 0;
PFNGLACTIVESHADERPROGRAMPROC	kglActiveShaderProgram= 0;
PFNGLCREATESHADERPROGRAMVPROC kglCreateShaderProgramv= 0;
PFNGLPROGRAMPARAMETERIPROC kglProgramParameteri= 0;

PFNGLPROGRAMUNIFORM1IPROC kglProgramUniform1i= 0;
PFNGLPROGRAMUNIFORM2IPROC kglProgramUniform2i= 0;
PFNGLPROGRAMUNIFORM3IPROC kglProgramUniform3i= 0;
PFNGLPROGRAMUNIFORM4IPROC kglProgramUniform4i= 0;

PFNGLPROGRAMUNIFORM1UIPROC kglProgramUniform1ui= 0;
PFNGLPROGRAMUNIFORM2UIPROC kglProgramUniform2ui= 0;
PFNGLPROGRAMUNIFORM3UIPROC kglProgramUniform3ui= 0;
PFNGLPROGRAMUNIFORM4UIPROC kglProgramUniform4ui= 0;

PFNGLPROGRAMUNIFORM1FPROC kglProgramUniform1f= 0;
PFNGLPROGRAMUNIFORM2FPROC kglProgramUniform2f= 0;
PFNGLPROGRAMUNIFORM3FPROC kglProgramUniform3f= 0;
PFNGLPROGRAMUNIFORM4FPROC kglProgramUniform4f= 0;

PFNGLPROGRAMUNIFORM1DPROC kglProgramUniform1d= 0;
PFNGLPROGRAMUNIFORM2DPROC kglProgramUniform2d= 0;
PFNGLPROGRAMUNIFORM3DPROC kglProgramUniform3d= 0;
PFNGLPROGRAMUNIFORM4DPROC kglProgramUniform4d= 0;

PFNGLPROGRAMUNIFORM1IVPROC kglProgramUniform1iv= 0;
PFNGLPROGRAMUNIFORM2IVPROC kglProgramUniform2iv= 0;
PFNGLPROGRAMUNIFORM3IVPROC kglProgramUniform3iv= 0;
PFNGLPROGRAMUNIFORM4IVPROC kglProgramUniform4iv= 0;

PFNGLPROGRAMUNIFORM1UIVPROC kglProgramUniform1uiv= 0;
PFNGLPROGRAMUNIFORM2UIVPROC kglProgramUniform2uiv= 0;
PFNGLPROGRAMUNIFORM3UIVPROC kglProgramUniform3uiv= 0;
PFNGLPROGRAMUNIFORM4UIVPROC kglProgramUniform4uiv= 0;

PFNGLPROGRAMUNIFORM1FVPROC kglProgramUniform1fv= 0;
PFNGLPROGRAMUNIFORM2FVPROC kglProgramUniform2fv= 0;
PFNGLPROGRAMUNIFORM3FVPROC kglProgramUniform3fv= 0;
PFNGLPROGRAMUNIFORM4FVPROC kglProgramUniform4fv= 0;

PFNGLPROGRAMUNIFORM1DVPROC kglProgramUniform1dv= 0;
PFNGLPROGRAMUNIFORM2DVPROC kglProgramUniform2dv= 0;
PFNGLPROGRAMUNIFORM3DVPROC kglProgramUniform3dv= 0;
PFNGLPROGRAMUNIFORM4DVPROC kglProgramUniform4dv= 0;

PFNGLPROGRAMUNIFORMMATRIX2FVPROC	kglProgramUniformMatrix2fv= 0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC	kglProgramUniformMatrix3fv= 0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC	kglProgramUniformMatrix4fv= 0;

PFNGLPROGRAMUNIFORMMATRIX2DVPROC	kglProgramUniformMatrix2dv= 0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC	kglProgramUniformMatrix3dv= 0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC	kglProgramUniformMatrix4dv= 0;

PFNGLVALIDATEPROGRAMPIPELINEPROC	kglValidateProgramPipeline= 0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC kglGetProgramPipelineInfoLog= 0;

//GL_ARB_vertex_array_object//
PFNGLGENVERTEXARRAYSPROC	kglGenVertexArrays= 0;
PFNGLBINDVERTEXARRAYPROC	kglBindVertexArray= 0;
PFNGLDELETEVERTEXARRAYSPROC kglDeleteVertexArrays= 0;

//GL_EXT_direct_state_access
PFNGLTEXTUREIMAGE1DEXTPROC kglTextureImage1DEXT= 0;
PFNGLTEXTUREIMAGE2DEXTPROC kglTextureImage2DEXT= 0;
PFNGLTEXTURESUBIMAGE1DEXTPROC kglTextureSubImage1DEXT= 0;
PFNGLTEXTURESUBIMAGE2DEXTPROC kglTextureSubImage2DEXT= 0;
PFNGLNAMEDBUFFERDATAEXTPROC kglNamedBufferDataEXT= 0;
PFNGLNAMEDBUFFERSUBDATAEXTPROC kglNamedBufferSubDataEXT= 0;
PFNGLMAPNAMEDBUFFEREXTPROC kglMapNamedBufferEXT= 0;
PFNGLUNMAPNAMEDBUFFEREXTPROC kglUnmapNamedBufferEXT= 0;

//GL_ARB_sampler_objects
PFNGLGENSAMPLERSPROC kglGenSamplers= 0;
PFNGLDELETESAMPLERSPROC kglDeleteSamplers= 0;
PFNGLBINDSAMPLERPROC kglBindSampler= 0;
PFNGLSAMPLERPARAMETERIPROC kglSamplerParameteri= 0;
PFNGLSAMPLERPARAMETERFPROC kglSamplerParameterf= 0;
PFNGLSAMPLERPARAMETERIVPROC kglSamplerParameteriv= 0;
PFNGLSAMPLERPARAMETERFVPROC kglSamplerParameterfv= 0;
PFNGLSAMPLERPARAMETERIIVPROC kglSamplerParameterIiv= 0;
PFNGLSAMPLERPARAMETERIUIVPROC kglSamplerParameterIuiv= 0;
PFNGLGETSAMPLERPARAMETERIIVPROC kglGetSamplerParameterIiv= 0;
PFNGLGETSAMPLERPARAMETERIVPROC kglGetSamplerParameteriv= 0;
PFNGLGETSAMPLERPARAMETERFVPROC kglGetSamplerParameterfv= 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC kglGetSamplerParameterIuiv= 0;
/*********************************************************************************/