#ifndef _K15_Rendering_GL_Types_h_
#define _K15_Rendering_GL_Types_h_

//EGL_KHR_create_context
#define EGL_CONTEXT_MAJOR_VERSION_KHR           0x3098
#define EGL_CONTEXT_MINOR_VERSION_KHR           0x30FB
#define EGL_CONTEXT_FLAGS_KHR                   0x30FC
#define EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR     0x30FD
#define EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR  0x31BD
#define EGL_NO_RESET_NOTIFICATION_KHR           0x31BE
#define EGL_LOSE_CONTEXT_ON_RESET_KHR           0x31BF
#define EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR               0x00000001
#define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR  0x00000002
#define EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR       0x00000004
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR          0x00000001
#define EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR 0x00000002

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

//GL_KHR_debug
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0

//GL_EXT_texture_filter_anisotropic
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE 
//GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#include "K15_RenderPrerequisites.h"
#include "K15_RenderGLConfig.h"

#define K15_INVALID_GL_BUFFER_INDEX 0xffffffff
#define K15_INVALID_GL_PROGRAM_INDEX 0xffffffff

#ifdef K15_OPENGL_ENABLE_ERROR_CHECK_CALLS
	#define K15_OPENGL_CALL(x) {x; GLenum errorEnum = kglGetError(); assert(errorEnum == GL_NO_ERROR && #x);}
#else
	#define K15_OPENGL_CALL(x) x;
#endif //K15_OPENGL_ENABLE_ERROR_CHECK_CALLS


#ifdef K15_OS_WINDOWS
	#include <gl/GL.h>
	#include "OpenGL/WGL/wglext.h"
	#include "OpenGL/glcorearb.h"
#elif defined K15_OS_LINUX
	#include "GL/GLX/glxext.h"
#elif defined K15_OS_ANDROID
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2platform.h>
#ifndef APIENTRY
	#define APIENTRY
#endif //APIENTRY
	//#define APIENTRY GL_APIENTRYP
#endif // K15_OS_WINDOWS

typedef char GLchar;
struct K15_GLRenderContext;

/*********************************************************************************/
enum K15_GLUniformUsage
{
	K15_GL_UNIFORM_USAGE_ATTRIBUTE = 0,
	K15_GL_UNIFORM_USAGE_UNIFORM,
	K15_GL_UNIFORM_USAGE_UNKNOWN
};
/*********************************************************************************/
struct K15_GLBuffer
{
	byte* data;
	uint32 size;
	GLuint buffer;
	GLenum bufferType;
};
/*********************************************************************************/
struct K15_GLBufferAccessData
{
	uint32 glBufferIndex;
	uint32 offset;
	uint32 size;
};
/*********************************************************************************/
struct K15_GLRenderTarget
{
	GLuint glFramebuffer;
	GLuint glTextures[K15_RENDER_GL_MAX_COLOR_ATTACHMENTS];
	GLuint glRenderbuffer;

	uint32 textureCount;
};
/*********************************************************************************/
struct K15_GLUniform
{
	K15_GLUniformUsage uniformUsage;
	K15_UniformType uniformType;
	GLenum internalGLType;
	GLint size;
	GLint registerIndex; 
	GLchar* name;
};
/*********************************************************************************/
struct K15_GLProgram
{
	K15_GLUniform uniforms[K15_RENDER_GL_MAX_PROGRAM_UNIFORMS];
	uint32 uniformCount;
	GLuint program;
};
/*********************************************************************************/
struct K15_GLTexture
{
	uint32 width;
	uint32 height;
	uint32 depth;

	GLenum glTextureTarget;
	GLenum glFormat;
	GLenum glInternalFormat;
	GLenum glFormatType;
	GLuint glTexture;
};
/*********************************************************************************/
struct K15_GLSampler
{
	GLuint glSampler;
};
/*********************************************************************************/

// default gl functions
typedef GLvoid (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef GLvoid (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint*);
typedef GLvoid (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum, GLsizei, GLenum, const GLvoid*);
typedef GLvoid (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei, GLuint*);

//windows gl
typedef GLvoid* (WINAPI *PFNWGLGETPROCADDRESSPROC)(const char*);
typedef HGLRC	(WINAPI *PFNWGLCREATECONTEXTPROC)(HDC);
typedef BOOL	(WINAPI *PFNWGLDELETECONTEXTPROC)(HGLRC);
typedef BOOL	(WINAPI *PFNWGLMAKECURRENTPROC)(HDC, HGLRC);

//custom gl function typedefs
typedef GLboolean(*PFNKGLSWAPBUFFERSPROC)(K15_GLRenderContext*);
typedef GLvoid*(*PFNKGLGETPROCADDRESSPROC)(const char*);

//GL_AMD_debug_output typedefs
typedef void(APIENTRY *GLDEBUGPROCAMD)(GLuint, GLenum, GLenum, GLsizei, const GLchar*, GLvoid*);
typedef void(APIENTRY *PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, GLvoid *userParam);

//GL_ARB_direct_state_access
typedef GLvoid (APIENTRY *PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef GLvoid (APIENTRY *PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef GLvoid (APIENTRY *PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef GLvoid (APIENTRY *PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef GLvoid (APIENTRY *PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef GLvoid (APIENTRY *PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef GLvoid* (APIENTRY *PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid* (APIENTRY *PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean (APIENTRY *PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);

//GL_KHR_debug
typedef GLvoid (APIENTRY* GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef GLvoid (APIENTRY* PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
/*********************************************************************************/

#ifdef K15_OS_WINDOWS

//wgl
extern PFNWGLCHOOSEPIXELFORMATARBPROC			kwglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC		kwglCreateContextAttribsARB;
extern PFNWGLGETPROCADDRESSPROC					kwglGetProcAddress;
extern PFNWGLCREATECONTEXTPROC					kwglCreateContext;
extern PFNWGLDELETECONTEXTPROC					kwglDeleteContext;
extern PFNWGLMAKECURRENTPROC					kwglMakeCurrent;

//default gl functions
extern PFNGLGENBUFFERSPROC						kglGenBuffers; 
extern PFNGLBINDBUFFERPROC						kglBindBuffer;
extern PFNGLBINDFRAGDATALOCATIONPROC			kglBindFragDataLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC				kglVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC			kglEnableVertexAttribArray;	
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC		kglDisableVertexAttribArray;
extern PFNGLDELETEBUFFERSPROC					kglDeleteBuffers;
extern PFNGLGETPROGRAMIVPROC					kglGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC				kglGetProgramInfoLog;
extern PFNGLDELETEPROGRAMPROC					kglDeleteProgram;
extern PFNGLGETACTIVEUNIFORMPROC				kglGetActiveUniform;
extern PFNGLGETACTIVEATTRIBPROC					kglGetActiveAttrib;
extern PFNGLGETUNIFORMLOCATIONPROC				kglGetUniformLocation;
extern PFNGLGETATTRIBLOCATIONPROC				kglGetAttribLocation;
extern PFNGLBLENDEQUATIONSEPARATEPROC			kglBlendEquationSeparate;
extern PFNGLBLENDFUNCSEPARATEPROC				kglBlendFuncSeparate;
extern PFNGLGENFRAMEBUFFERSPROC					kglGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC					kglBindFramebuffer;	
extern PFNGLDELETEFRAMEBUFFERSPROC				kglDeleteFramebuffers;	
extern PFNGLGENRENDERBUFFERSPROC				kglGenRenderbuffers;		
extern PFNGLBINDRENDERBUFFERPROC				kglBindRenderbuffer;		
extern PFNGLDELETERENDERBUFFERSPROC				kglDeleteRenderbuffers;	
extern PFNGLRENDERBUFFERSTORAGEPROC				kglRenderbufferStorage;	
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC	kglRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC			kglFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC				kglFramebufferTexture;	
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC			kglCheckFramebufferStatus;
extern PFNGLDRAWBUFFERSPROC						kglDrawBuffers;
extern PFNGLBINDTEXTUREPROC						kglBindTexture;
extern PFNGLCLEARPROC							kglClear;
extern PFNGLCLEARCOLORPROC						kglClearColor;
extern PFNGLCULLFACEPROC						kglCullFace;
extern PFNGLDELETETEXTURESPROC					kglDeleteTextures;
extern PFNGLDEPTHFUNCPROC						kglDepthFunc;
extern PFNGLDISABLEPROC							kglDisable;
extern PFNGLDRAWELEMENTSPROC					kglDrawElements;
extern PFNGLENABLEPROC							kglEnable;
extern PFNGLFRONTFACEPROC						kglFrontFace;
extern PFNGLGENTEXTURESPROC						kglGenTextures;
extern PFNGLGETERRORPROC						kglGetError;
extern PFNGLGETFLOATVPROC						kglGetFloatv;
extern PFNGLGETINTEGERVPROC						kglGetIntegerv;
extern PFNGLGETSTRINGPROC						kglGetString;
extern PFNGLGETSTRINGIPROC						kglGetStringi;
extern PFNGLPOLYGONMODEPROC						kglPolygonMode;
extern PFNGLVIEWPORTPROC						kglViewport;

//GL_ARB_vertex_array_object//
extern PFNGLGENVERTEXARRAYSPROC	kglGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC	kglBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC kglDeleteVertexArrays;

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
#endif //K15_OS_WINDOWS

//GL_KHR_debug
extern PFNGLDEBUGMESSAGECALLBACKPROC kglDebugMessageCallback;

//GL_AMD_debug_output
extern PFNGLDEBUGMESSAGECALLBACKAMDPROC kglDebugMessageCallbackAMD;

//GL_ARB_direct_state_access
extern PFNGLTEXTUREIMAGE1DEXTPROC kglTextureImage1DEXT;
extern PFNGLTEXTUREIMAGE2DEXTPROC kglTextureImage2DEXT;
extern PFNGLTEXTUREIMAGE3DEXTPROC kglTextureImage3DEXT;
extern PFNGLTEXTURESUBIMAGE1DEXTPROC kglTextureSubImage1DEXT;
extern PFNGLTEXTURESUBIMAGE2DEXTPROC kglTextureSubImage2DEXT;
extern PFNGLTEXTURESUBIMAGE3DEXTPROC kglTextureSubImage3DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC kglCompressedTextureImage1DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC kglCompressedTextureImage2DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC kglCompressedTextureImage3DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC kglCompressedTextureSubImage1DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC kglCompressedTextureSubImage2DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC kglCompressedTextureSubImage3DEXT;
extern PFNGLGENERATETEXTUREMIPMAPEXTPROC kglGenerateTextureMipmapEXT;
extern PFNGLNAMEDBUFFERDATAEXTPROC kglNamedBufferDataEXT;
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC kglNamedBufferSubDataEXT;
extern PFNGLMAPNAMEDBUFFEREXTPROC kglMapNamedBufferEXT;
extern PFNGLMAPNAMEDBUFFERRANGEEXTPROC kglMapNamedBufferRangeEXT;
extern PFNGLUNMAPNAMEDBUFFEREXTPROC kglUnmapNamedBufferEXT;

//custom gl functions
extern PFNKGLSWAPBUFFERSPROC kglSwapBuffers;
extern PFNKGLGETPROCADDRESSPROC kglGetProcAddress;

//GL_EXT_direct_state_access
extern PFNGLTEXTUREIMAGE1DEXTPROC kglTextureImage1DEXT;
extern PFNGLTEXTUREIMAGE2DEXTPROC kglTextureImage2DEXT;
extern PFNGLTEXTURESUBIMAGE1DEXTPROC kglTextureSubImage1DEXT;
extern PFNGLTEXTURESUBIMAGE2DEXTPROC kglTextureSubImage2DEXT;
extern PFNGLNAMEDBUFFERDATAEXTPROC kglNamedBufferDataEXT;
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC kglNamedBufferSubDataEXT;
extern PFNGLMAPNAMEDBUFFEREXTPROC kglMapNamedBufferEXT;
extern PFNGLUNMAPNAMEDBUFFEREXTPROC kglUnmapNamedBufferEXT;


/*********************************************************************************/


#endif //_K15_Renderin_GL_Types_h_