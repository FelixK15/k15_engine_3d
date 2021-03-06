#include "OpenGL/K15_RenderGLTypes.h"

#ifdef K15_OS_WINDOWS
	//wgl
	PFNWGLCHOOSEPIXELFORMATARBPROC		kwglChoosePixelFormatARB			= 0;
	PFNWGLCREATECONTEXTATTRIBSARBPROC	kwglCreateContextAttribsARB			= 0;
	PFNWGLGETPROCADDRESSPROC			kwglGetProcAddress					= 0;
	PFNWGLCREATECONTEXTPROC				kwglCreateContext					= 0;
	PFNWGLDELETECONTEXTPROC				kwglDeleteContext					= 0;
	PFNWGLMAKECURRENTPROC				kwglMakeCurrent						= 0;
	PFNWGLGETCURRENTCONTEXTPROC			kwglGetCurrentContext				= 0;
	PFNWGLGETCURRENTDCPROC				kwglGetCurrentDC					= 0;
#endif //K15_OS_WINDOWS

//default gl functions
PFNGLCREATESHADERPROC					kglCreateShader						= 0;
PFNGLSHADERSOURCEPROC					kglShaderSource						= 0;
PFNGLCOMPILESHADERPROC					kglCompileShader					= 0;
PFNGLCREATEPROGRAMPROC					kglCreateProgram					= 0;
PFNGLGETSHADERIVPROC					kglGetShaderiv						= 0;
PFNGLATTACHSHADERPROC					kglAttachShader						= 0;
PFNGLLINKPROGRAMPROC					kglLinkProgram						= 0;
PFNGLDETACHSHADERPROC					kglDetachShader						= 0;
PFNGLGETSHADERINFOLOGPROC				kglGetShaderInfoLog					= 0;
PFNGLGENBUFFERSPROC						kglGenBuffers						= 0;
PFNGLBINDBUFFERPROC						kglBindBuffer						= 0;
PFNGLBUFFERDATAPROC						kglBufferData						= 0;
PFNGLBUFFERSUBDATAPROC					kglBufferSubData					= 0;
PFNGLVERTEXATTRIBPOINTERPROC			kglVertexAttribPointer				= 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC		kglEnableVertexAttribArray			= 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC		kglDisableVertexAttribArray			= 0;
PFNGLDELETEBUFFERSPROC					kglDeleteBuffers					= 0;
PFNGLGETPROGRAMIVPROC					kglGetProgramiv						= 0;
PFNGLGETPROGRAMINFOLOGPROC				kglGetProgramInfoLog				= 0;
PFNGLBINDFRAGDATALOCATIONPROC			kglBindFragDataLocation				= 0;
PFNGLDELETEPROGRAMPROC					kglDeleteProgram					= 0;
PFNGLGETACTIVEUNIFORMPROC				kglGetActiveUniform					= 0;
PFNGLGETACTIVEATTRIBPROC				kglGetActiveAttrib					= 0;
PFNGLGETUNIFORMLOCATIONPROC				kglGetUniformLocation				= 0;
PFNGLGETATTRIBLOCATIONPROC				kglGetAttribLocation				= 0;
PFNGLBLENDEQUATIONSEPARATEPROC			kglBlendEquationSeparate			= 0;
PFNGLBLENDFUNCSEPARATEPROC				kglBlendFuncSeparate				= 0;
PFNGLGENFRAMEBUFFERSPROC				kglGenFramebuffers					= 0;
PFNGLBINDFRAMEBUFFERPROC				kglBindFramebuffer					= 0;
PFNGLDELETEFRAMEBUFFERSPROC				kglDeleteFramebuffers				= 0;
PFNGLGENRENDERBUFFERSPROC				kglGenRenderbuffers					= 0;
PFNGLBINDRENDERBUFFERPROC				kglBindRenderbuffer					= 0;
PFNGLDELETERENDERBUFFERSPROC			kglDeleteRenderbuffers				= 0;
PFNGLRENDERBUFFERSTORAGEPROC			kglRenderbufferStorage				= 0;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC	kglRenderbufferStorageMultisample	= 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC		kglFramebufferRenderbuffer			= 0;
PFNGLFRAMEBUFFERTEXTUREPROC				kglFramebufferTexture				= 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC			kglCheckFramebufferStatus			= 0;
PFNGLDRAWBUFFERSPROC					kglDrawBuffers						= 0;
PFNGLBINDTEXTUREPROC					kglBindTexture						= 0;
PFNGLTEXPARAMETERIPROC					kglTexParameteri					= 0;
PFNGLCLEARPROC							kglClear							= 0;
PFNGLCLEARCOLORPROC						kglClearColor						= 0;
PFNGLCULLFACEPROC						kglCullFace							= 0;
PFNGLDELETETEXTURESPROC					kglDeleteTextures					= 0;
PFNGLDEPTHFUNCPROC						kglDepthFunc						= 0;
PFNGLDISABLEPROC						kglDisable							= 0;
PFNGLDRAWELEMENTSPROC					kglDrawElements						= 0;
PFNGLDRAWRANGEELEMENTSPROC				kglDrawRangeElements				= 0;
PFNGLDRAWELEMENTSBASEVERTEXPROC			kglDrawElementsBaseVertex			= 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC	kglDrawRangeElementsBaseVertex		= 0;
PFNGLDRAWARRAYSPROC						kglDrawArrays						= 0;
PFNGLENABLEPROC							kglEnable							= 0;
PFNGLFRONTFACEPROC						kglFrontFace						= 0;
PFNGLGENTEXTURESPROC					kglGenTextures						= 0;
PFNGLGETERRORPROC						kglGetError							= 0;
PFNGLGETFLOATVPROC						kglGetFloatv						= 0;
PFNGLGETINTEGERVPROC					kglGetIntegerv						= 0;
PFNGLGETSTRINGIPROC						kglGetStringi						= 0;
PFNGLGETSTRINGPROC						kglGetString						= 0;
PFNGLPOLYGONMODEPROC					kglPolygonMode						= 0;
PFNGLVIEWPORTPROC						kglViewport							= 0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC			kglCompressedTexImage1D				= 0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC			kglCompressedTexImage2D				= 0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC			kglCompressedTexImage3D				= 0;
PFNGLTEXIMAGE1DPROC						kglTexImage1D						= 0;
PFNGLTEXIMAGE2DPROC						kglTexImage2D						= 0;
PFNGLTEXIMAGE3DPROC						kglTexImage3D						= 0;
PFNGLGENERATEMIPMAPPROC					kglGenerateMipmap					= 0;
PFNGLACTIVETEXTUREPROC					kglActiveTexture					= 0;

//GL_ARB_debug_output
PFNGLDEBUGMESSAGECALLBACKPROC		kglDebugMessageCallback			= 0;

//GL_ARB_vertex_array_object//
PFNGLGENVERTEXARRAYSPROC			kglGenVertexArrays				= 0;
PFNGLBINDVERTEXARRAYPROC			kglBindVertexArray				= 0;
PFNGLDELETEVERTEXARRAYSPROC			kglDeleteVertexArrays			= 0;

//GL_ARB_direct_state_access
PFNGLTEXTURESTORAGE1DPROC				kglTextureStorage1D				= 0;
PFNGLTEXTURESTORAGE2DPROC				kglTextureStorage2D				= 0;
PFNGLTEXTURESTORAGE3DPROC				kglTextureStorage3D				= 0;
PFNGLTEXTURESUBIMAGE1DPROC				kglTextureSubImage1D			= 0;
PFNGLTEXTURESUBIMAGE2DPROC				kglTextureSubImage2D			= 0;
PFNGLTEXTURESUBIMAGE3DPROC				kglTextureSubImage3D			= 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC	kglCompressedTextureSubImage1D	= 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC	kglCompressedTextureSubImage2D	= 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC	kglCompressedTextureSubImage3D	= 0;
PFNGLGENERATETEXTUREMIPMAPPROC			kglGenerateTextureMipmap		= 0;
PFNGLNAMEDBUFFERDATAPROC				kglNamedBufferData				= 0;
PFNGLNAMEDBUFFERSUBDATAPROC				kglNamedBufferSubData			= 0;
PFNGLMAPNAMEDBUFFERPROC					kglMapNamedBuffer				= 0;
PFNGLMAPNAMEDBUFFERRANGEPROC			kglMapNamedBufferRange			= 0;
PFNGLUNMAPNAMEDBUFFERPROC				kglUnmapNamedBuffer				= 0;

//GL_ARB_sampler_objects
PFNGLGENSAMPLERSPROC				kglGenSamplers					= 0;
PFNGLDELETESAMPLERSPROC				kglDeleteSamplers				= 0;
PFNGLBINDSAMPLERPROC				kglBindSampler					= 0;
PFNGLSAMPLERPARAMETERIPROC			kglSamplerParameteri			= 0;
PFNGLSAMPLERPARAMETERFPROC			kglSamplerParameterf			= 0;
PFNGLSAMPLERPARAMETERIVPROC			kglSamplerParameteriv			= 0;
PFNGLSAMPLERPARAMETERFVPROC			kglSamplerParameterfv			= 0;
PFNGLSAMPLERPARAMETERIIVPROC		kglSamplerParameterIiv			= 0;
PFNGLSAMPLERPARAMETERIUIVPROC		kglSamplerParameterIuiv			= 0;
PFNGLGETSAMPLERPARAMETERIIVPROC		kglGetSamplerParameterIiv		= 0;
PFNGLGETSAMPLERPARAMETERIVPROC		kglGetSamplerParameteriv		= 0;
PFNGLGETSAMPLERPARAMETERFVPROC		kglGetSamplerParameterfv		= 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC	kglGetSamplerParameterIuiv		= 0;

//GL_ARB_separate_shader_objects//
PFNGLGENPROGRAMPIPELINESPROC		kglGenProgramPipelines			= 0;
PFNGLBINDPROGRAMPIPELINEPROC		kglBindProgramPipeline			= 0;
PFNGLDELETEPROGRAMPIPELINESPROC		kglDeleteProgramPipelines		= 0;
PFNGLUSEPROGRAMSTAGESPROC			kglUseProgramStages				= 0;
PFNGLACTIVESHADERPROGRAMPROC		kglActiveShaderProgram			= 0;
PFNGLCREATESHADERPROGRAMVPROC		kglCreateShaderProgramv			= 0;
PFNGLPROGRAMPARAMETERIPROC			kglProgramParameteri			= 0;

PFNGLPROGRAMUNIFORM1IPROC			kglProgramUniform1i				= 0;
PFNGLPROGRAMUNIFORM2IPROC			kglProgramUniform2i				= 0;
PFNGLPROGRAMUNIFORM3IPROC			kglProgramUniform3i				= 0;
PFNGLPROGRAMUNIFORM4IPROC			kglProgramUniform4i				= 0;

PFNGLPROGRAMUNIFORM1UIPROC			kglProgramUniform1ui			= 0;
PFNGLPROGRAMUNIFORM2UIPROC			kglProgramUniform2ui			= 0;
PFNGLPROGRAMUNIFORM3UIPROC			kglProgramUniform3ui			= 0;
PFNGLPROGRAMUNIFORM4UIPROC			kglProgramUniform4ui			= 0;

PFNGLPROGRAMUNIFORM1FPROC			kglProgramUniform1f				= 0;
PFNGLPROGRAMUNIFORM2FPROC			kglProgramUniform2f				= 0;
PFNGLPROGRAMUNIFORM3FPROC			kglProgramUniform3f				= 0;
PFNGLPROGRAMUNIFORM4FPROC			kglProgramUniform4f				= 0;

PFNGLPROGRAMUNIFORM1DPROC			kglProgramUniform1d				= 0;
PFNGLPROGRAMUNIFORM2DPROC			kglProgramUniform2d				= 0;
PFNGLPROGRAMUNIFORM3DPROC			kglProgramUniform3d				= 0;
PFNGLPROGRAMUNIFORM4DPROC			kglProgramUniform4d				= 0;

PFNGLPROGRAMUNIFORM1IVPROC			kglProgramUniform1iv			= 0;
PFNGLPROGRAMUNIFORM2IVPROC			kglProgramUniform2iv			= 0;
PFNGLPROGRAMUNIFORM3IVPROC			kglProgramUniform3iv			= 0;
PFNGLPROGRAMUNIFORM4IVPROC			kglProgramUniform4iv			= 0;

PFNGLPROGRAMUNIFORM1UIVPROC			kglProgramUniform1uiv			= 0;
PFNGLPROGRAMUNIFORM2UIVPROC			kglProgramUniform2uiv			= 0;
PFNGLPROGRAMUNIFORM3UIVPROC			kglProgramUniform3uiv			= 0;
PFNGLPROGRAMUNIFORM4UIVPROC			kglProgramUniform4uiv			= 0;

PFNGLPROGRAMUNIFORM1FVPROC			kglProgramUniform1fv			= 0;
PFNGLPROGRAMUNIFORM2FVPROC			kglProgramUniform2fv			= 0;
PFNGLPROGRAMUNIFORM3FVPROC			kglProgramUniform3fv			= 0;
PFNGLPROGRAMUNIFORM4FVPROC			kglProgramUniform4fv			= 0;

PFNGLPROGRAMUNIFORM1DVPROC			kglProgramUniform1dv			= 0;
PFNGLPROGRAMUNIFORM2DVPROC			kglProgramUniform2dv			= 0;
PFNGLPROGRAMUNIFORM3DVPROC			kglProgramUniform3dv			= 0;
PFNGLPROGRAMUNIFORM4DVPROC			kglProgramUniform4dv			= 0;

PFNGLPROGRAMUNIFORMMATRIX2FVPROC	kglProgramUniformMatrix2fv		= 0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC	kglProgramUniformMatrix3fv		= 0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC	kglProgramUniformMatrix4fv		= 0;

PFNGLPROGRAMUNIFORMMATRIX2DVPROC	kglProgramUniformMatrix2dv		= 0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC	kglProgramUniformMatrix3dv		= 0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC	kglProgramUniformMatrix4dv		= 0;

PFNGLVALIDATEPROGRAMPIPELINEPROC	kglValidateProgramPipeline		= 0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC	kglGetProgramPipelineInfoLog	= 0;

//custom gl functions
PFNKGLSWAPBUFFERSPROC				kglSwapBuffers					= 0;
PFNKGLGETPROCADDRESSPROC			kglGetProcAddress				= 0;

#ifdef K15_OPENGL_ENABLE_ERROR_CHECK_CALLS
const char* K15_GLConvertErrorCode(GLenum p_ErrorCode)
{
	const char* errorMessage = 0;
	switch(p_ErrorCode)
	{
		case GL_INVALID_ENUM:
		{
			errorMessage = "GL_INVALID_ENUM";
			break;
		}

		case GL_INVALID_VALUE:
		{
			errorMessage = "GL_INVALID_VALUE";
			break;
		}

		case GL_INVALID_OPERATION:
		{
			errorMessage = "GL_INVALID_OPERATION";
			break;
		}

		case GL_STACK_OVERFLOW:
		{
			errorMessage = "GL_STACK_OVERFLOW";
			break;
		}

		case GL_STACK_UNDERFLOW:
		{
			errorMessage = "GL_STACK_UNDERFLOW";
			break;
		}

		case GL_OUT_OF_MEMORY:
		{
			errorMessage = "GL_OUT_OF_MEMORY";
			break;
		}

		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			errorMessage = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		case GL_CONTEXT_LOST:
		{
			errorMessage = "GL_CONTEXT_LOST";
			break;
		}
	}
	return errorMessage;
}
#endif //K15_OPENGL_ENABLE_ERROR_CHECK_CALLS