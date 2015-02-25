#ifndef _K15_Rendering_GL_Types_h_
#define _K15_Rendering_GL_Types_h_

//GL_AMD_debug_output
#define GL_MAX_DEBUG_MESSAGE_LENGTH_AMD 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_AMD 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_AMD 0x9145
#define GL_DEBUG_SEVERITY_HIGH_AMD 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_AMD 0x9147
#define GL_DEBUG_SEVERITY_LOW_AMD 0x9148
#define GL_DEBUG_CATEGORY_API_ERROR_AMD 0x9149
#define GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD 0x914A
#define GL_DEBUG_CATEGORY_DEPRECATION_AMD 0x914B
#define GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD 0x914C
#define GL_DEBUG_CATEGORY_PERFORMANCE_AMD 0x914D
#define GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD 0x914E
#define GL_DEBUG_CATEGORY_APPLICATION_AMD 0x914F
#define GL_DEBUG_CATEGORY_OTHER_AMD 0x9150

#include "K15_RenderPrerequisites.h"

#include "GL/glcorearb.h"

#ifdef K15_OS_WINDOWS
	#include "GL/WGL/wglext.h"
#elif K15_OS_LINUX
	#include "GL/GLX/glxext.h"
#endif // K15_OS_WINDOWS

typedef char GLchar;

//custom gl function typedefs
typedef GLboolean(*PFNKGLSWAPBUFFERS)(K15_GLRenderContext*);
typedef GLvoid*(*PFNKGLGETPROCADDRESS)(char*);

//GL_AMD_debug_output typedefs
typedef void(APIENTRY *GLDEBUGPROCAMD)(GLuint, GLenum, GLenum, GLsizei, const GLchar*, GLvoid*);
typedef void(APIENTRY *PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, GLvoid *userParam);

//GL_ARB_direct_state_access
typedef void (APIENTRY *PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (APIENTRY *PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY *PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY *PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef GLvoid* (APIENTRY *PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid* (APIENTRY *PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean (APIENTRY *PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);


/*********************************************************************************/

#ifdef K15_OS_WINDOWS
//wgl
extern PFNWGLCHOOSEPIXELFORMATARBPROC kwglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC kwglCreateContextAttribsARB;
#endif //K15_OS_WINDOWS

//default gl functions
extern PFNGLGENBUFFERSPROC kglGenBuffers;

//GL_ARB_debug_output
extern PFNGLDEBUGMESSAGECALLBACKPROC kglDebugMessageCallback;

//GL_AMD_debug_output
extern PFNGLDEBUGMESSAGECALLBACKAMDPROC kglDebugMessageCallbackAMD;

//GL_ARB_vertex_array_object//
extern PFNGLGENVERTEXARRAYSPROC	kglGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC	kglBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC kglDeleteVertexArrays;

//GL_ARB_direct_state_access
extern PFNGLTEXTUREIMAGE1DEXTPROC kglTextureImage1DEXT;
extern PFNGLTEXTUREIMAGE2DEXTPROC kglTextureImage2DEXT;
extern PFNGLTEXTUREIMAGE3DEXTPROC kglTextureImage3DEXT;
extern PFNGLTEXTURESUBIMAGE1DEXTPROC kglTextureSubImage1DEXT;
extern PFNGLTEXTURESUBIMAGE2DEXTPROC kglTextureSubImage2DEXT;
extern PFNGLTEXTURESUBIMAGE3DEXTPROC kglTextureSubImage3DEXT;
extern PFNGLNAMEDBUFFERDATAEXTPROC kglNamedBufferDataEXT;
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC kglNamedBufferSubDataEXT;
extern PFNGLMAPNAMEDBUFFEREXTPROC kglMapNamedBufferEXT;
extern PFNGLUNMAPNAMEDBUFFEREXTPROC kglUnmapNamedBufferEXT;

//custom gl functions
extern PFNKGLSWAPBUFFERS kglSwapBuffers;
extern PFNKGLGETPROCADDRESS kglGetProcAddress;

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

/*

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


#endif //_K15_Renderin_GL_Types_h_