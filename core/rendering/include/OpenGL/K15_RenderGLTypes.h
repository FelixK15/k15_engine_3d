#ifndef _K15_Rendering_GL_Types_h_
#define _K15_Rendering_GL_Types_h_

//EGL_KHR_create_context
#define EGL_CONTEXT_MAJOR_VERSION_KHR						0x3098
#define EGL_CONTEXT_MINOR_VERSION_KHR						0x30FB
#define EGL_CONTEXT_FLAGS_KHR								0x30FC
#define EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR					0x30FD
#define EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR  0x31BD
#define EGL_NO_RESET_NOTIFICATION_KHR						0x31BE
#define EGL_LOSE_CONTEXT_ON_RESET_KHR						0x31BF
#define EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR					0x00000001
#define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR		0x00000002
#define EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR			0x00000004
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR				0x00000001
#define EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR	0x00000002

//GL_AMD_debug_output
#define GL_MAX_DEBUG_MESSAGE_LENGTH_AMD				0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_AMD			0x9144
#define GL_DEBUG_LOGGED_MESSAGES_AMD				0x9145
#define GL_DEBUG_SEVERITY_HIGH_AMD					0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_AMD				0x9147
#define GL_DEBUG_SEVERITY_LOW_AMD					0x9148
#define GL_DEBUG_CATEGORY_API_ERROR_AMD				0x9149
#define GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD			0x914A
#define GL_DEBUG_CATEGORY_DEPRECATION_AMD			0x914B
#define GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD	0x914C
#define GL_DEBUG_CATEGORY_PERFORMANCE_AMD			0x914D
#define GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD		0x914E
#define GL_DEBUG_CATEGORY_APPLICATION_AMD			0x914F
#define GL_DEBUG_CATEGORY_OTHER_AMD					0x9150

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
#include "K15_ShaderCompiler.h"

#define K15_INVALID_GL_BUFFER_INDEX 0xffffffff
#define K15_INVALID_GL_PROGRAM_INDEX 0xffffffff

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

#ifdef K15_OPENGL_ENABLE_ERROR_CHECK_CALLS
	const char* K15_GLConvertErrorCode(GLenum p_ErrorCode);
	#define K15_OPENGL_CALL(x) {{x; GLenum errorEnum = kglGetError(); K15_ASSERT_TEXT(errorEnum == GL_NO_ERROR, "OpenGL Error on calling '%s' (Error: %s)", #x, K15_GLConvertErrorCode(errorEnum));}};
#else
	#define K15_OPENGL_CALL(x) x;
#endif //K15_OPENGL_ENABLE_ERROR_CHECK_CALLS

typedef char GLchar;
struct K15_GLRenderContext;

enum K15_RenderBufferType : uint32;

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
	GLubyte* data;
	GLuint bufferSizeInBytes;
	GLuint glBufferHandle;
	GLenum glBufferType;
	K15_RenderBufferType bufferType;
};
/*********************************************************************************/
struct K15_GLRenderTarget
{
	GLuint glFramebufferHandle;
	GLuint glTextureHandles[K15_RENDER_GL_MAX_COLOR_ATTACHMENTS];
	GLuint glRenderbufferHandle;

	uint32 textureCount;
};
/*********************************************************************************/
struct K15_GLUniform
{
	uint32 typeID;
	uint32 nameHash;
	GLenum internalGLType;
	GLint sizeInBytes;
	GLint registerIndex; 
	GLchar* name;
};
/*********************************************************************************/
struct K15_GLInputLayoutElement
{
	uint32 typeID;
	uint32 semanticID;
	GLint elementSize;
	GLint sizeInBytes;
	GLint glRegisterIndex;
	GLenum glType;
	const GLvoid* offset;
};
/*********************************************************************************/
struct K15_GLInputLayout
{
	K15_GLInputLayoutElement* inputElements;
	uint32 numInputElements;
	uint32 stride;
};
/*********************************************************************************/
struct K15_GLMaterialPass
{

};
/*********************************************************************************/
struct K15_GLMaterial
{

};
/*********************************************************************************/
struct K15_GLProgram
{
	K15_GLUniform uniforms[K15_RENDER_GL_MAX_PROGRAM_UNIFORMS];
	K15_GLInputLayout inputLayout;
	K15_ShaderInformation shaderInformations;
	uint32 uniformCount;
	GLuint glProgramHandle;
	GLenum glProgramType;
};
/*********************************************************************************/
struct K15_GLVertexFormatElement
{
	GLuint index;
	GLint numValues;
	GLenum glValueType;
};
/*********************************************************************************/
struct K15_GLVertexFormat
{
	uint32 numInputElements;
	uint32 stride;
};
/*********************************************************************************/
struct K15_GLTexture
{
	uint32 width;
	uint32 height;
	uint32 depth;
	uint32 boundSlot;

	GLboolean useAutoMipMaps;

	GLuint numMipMaps;
	GLubyte** textureData;
	GLuint* textureDataSizeInBytes;

	GLenum glTextureTarget;
	GLenum glFormat;
	GLenum glInternalFormat;
	GLenum glFormatType;
	GLuint glTextureHandle;
};
/*********************************************************************************/
struct K15_GLSampler
{
	uint32 nameHash;
	uint32 boundSlot;

	GLuint glSamplerHandle;
	GLenum glMignificationFilter;
	GLenum glMagnificationFilter;
	GLenum glAddressModeU;
	GLenum glAddressModeV;
	GLenum glAddressModeW;
};
/*********************************************************************************/

// default gl functions
typedef GLvoid (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef GLvoid (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint*);
typedef GLvoid (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum, GLsizei, GLenum, const GLvoid*);
typedef GLvoid (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei, GLuint*);
typedef GLvoid (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);

//windows gl
typedef GLvoid* (WINAPI *PFNWGLGETPROCADDRESSPROC)(const char*);
typedef HGLRC	(WINAPI *PFNWGLCREATECONTEXTPROC)(HDC);
typedef BOOL	(WINAPI *PFNWGLDELETECONTEXTPROC)(HGLRC);
typedef BOOL	(WINAPI *PFNWGLMAKECURRENTPROC)(HDC, HGLRC);
typedef HGLRC	(WINAPI *PFNWGLGETCURRENTCONTEXTPROC)(void);
typedef HDC		(WINAPI* PFNWGLGETCURRENTDCPROC)(void);

//custom gl function typedefs
typedef GLboolean(*PFNKGLSWAPBUFFERSPROC)(K15_GLRenderContext*);
typedef GLvoid*(*PFNKGLGETPROCADDRESSPROC)(const char*);

//GL_AMD_debug_output typedefs
typedef void(APIENTRY *GLDEBUGPROCAMD)(GLuint, GLenum, GLenum, GLsizei, const GLchar*, GLvoid*);
typedef void(APIENTRY *PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, GLvoid *userParam);

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
extern PFNWGLGETCURRENTCONTEXTPROC				kwglGetCurrentContext;
extern PFNWGLGETCURRENTDCPROC					kwglGetCurrentDC;

//default gl functions
extern PFNGLCREATESHADERPROC					kglCreateShader;
extern PFNGLSHADERSOURCEPROC					kglShaderSource;
extern PFNGLCOMPILESHADERPROC					kglCompileShader;
extern PFNGLCREATEPROGRAMPROC					kglCreateProgram;
extern PFNGLGETSHADERIVPROC						kglGetShaderiv;
extern PFNGLATTACHSHADERPROC					kglAttachShader;
extern PFNGLLINKPROGRAMPROC						kglLinkProgram;
extern PFNGLDETACHSHADERPROC					kglDetachShader;
extern PFNGLGETSHADERINFOLOGPROC				kglGetShaderInfoLog;
extern PFNGLGENBUFFERSPROC						kglGenBuffers; 
extern PFNGLBINDBUFFERPROC						kglBindBuffer;
extern PFNGLBUFFERDATAPROC						kglBufferData;
extern PFNGLBUFFERSUBDATAPROC					kglBufferSubData;
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
extern PFNGLDRAWRANGEELEMENTSPROC				kglDrawRangeElements;
extern PFNGLDRAWELEMENTSBASEVERTEXPROC			kglDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC		kglDrawRangeElementsBaseVertex;
extern PFNGLDRAWARRAYSPROC						kglDrawArrays;
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
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC			kglCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC			kglCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC			kglCompressedTexImage3D;
extern PFNGLTEXIMAGE1DPROC						kglTexImage1D;
extern PFNGLTEXIMAGE2DPROC						kglTexImage2D;
extern PFNGLTEXIMAGE3DPROC						kglTexImage3D;
extern PFNGLGENERATEMIPMAPPROC					kglGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC					kglActiveTexture;

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

//GL_ARB_direct_state_access
extern PFNGLTEXTURESTORAGE1DPROC kglTextureStorage1D;
extern PFNGLTEXTURESTORAGE2DPROC kglTextureStorage2D;
extern PFNGLTEXTURESTORAGE3DPROC kglTextureStorage3D;
extern PFNGLTEXTURESUBIMAGE1DPROC kglTextureSubImage1D;
extern PFNGLTEXTURESUBIMAGE2DPROC kglTextureSubImage2D;
extern PFNGLTEXTURESUBIMAGE3DPROC kglTextureSubImage3D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC kglCompressedTextureSubImage1D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC kglCompressedTextureSubImage2D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC kglCompressedTextureSubImage3D;
extern PFNGLGENERATETEXTUREMIPMAPPROC kglGenerateTextureMipmap;
extern PFNGLNAMEDBUFFERDATAPROC kglNamedBufferData;
extern PFNGLNAMEDBUFFERSUBDATAPROC kglNamedBufferSubData;
extern PFNGLMAPNAMEDBUFFERPROC kglMapNamedBuffer;
extern PFNGLMAPNAMEDBUFFERRANGEPROC kglMapNamedBufferRange;
extern PFNGLUNMAPNAMEDBUFFERPROC kglUnmapNamedBuffer;

//custom gl functions
extern PFNKGLSWAPBUFFERSPROC kglSwapBuffers;
extern PFNKGLGETPROCADDRESSPROC kglGetProcAddress;

/*********************************************************************************/


#endif //_K15_Renderin_GL_Types_h_