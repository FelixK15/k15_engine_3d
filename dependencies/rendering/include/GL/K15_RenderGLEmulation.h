#ifndef _K15_Rendering_GL_Emulation_h_
#define _K15_Rendering_GL_Emulation_h_

#ifndef GLdouble
	typedef double GLdouble;
#endif //GLdouble




#ifndef GL_MINOR_VERSION
	#define GL_MINOR_VERSION 0x821C
#endif //GL_MINOR_VERSION

#ifndef GL_MAJOR_VERSION
	#define GL_MAJOR_VERSION 0x821B
#endif //GL_MAJOR_VERSION

#ifndef GL_UNIFORM_BUFFER
	#define GL_UNIFORM_BUFFER 0x8A11
#endif //GL_UNIFORM_BUFFER

#ifndef GL_NUM_EXTENSIONS
	#define GL_NUM_EXTENSIONS 0x821D
#endif //GL_NUM_EXTENSIONS

#ifndef GL_DEBUG_SOURCE_API_ARB
	#define GL_DEBUG_SOURCE_API_ARB 0x8246
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_SOURCE_WINDOW_SYSTEM
	#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_SOURCE_SHADER_COMPILER
	#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_SOURCE_THIRD_PARTY
	#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_SOURCE_APPLICATION
	#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_TYPE_ERROR
	#define GL_DEBUG_TYPE_ERROR 0x824C
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
	#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
	#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_TYPE_PORTABILITY
	#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#endif //GL_DEBUG_SOURCE_API_ARB

#ifndef GL_DEBUG_TYPE_PERFORMANCE
	#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#endif //GL_DEBUG_SOURCE_API_ARB


#ifndef PFNGLGETSTRINGIPROC
	typedef const GLubyte* (*PFNGLGETSTRINGIPROC) (GLenum, GLuint);
#endif //PFNGLGETSTRINGIPROC



#ifndef PFNGLGENVERTEXARRAYSPROC
	typedef void (*PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
#endif //PFNGLGENVERTEXARRAYSPROC

#ifndef PFNGLBINDVERTEXARRAYPROC
	typedef void (*PFNGLBINDVERTEXARRAYPROC) (GLuint array);
#endif //PFNGLGENVERTEXARRAYSPROC

#ifndef PFNGLDELETEVERTEXARRAYSPROC
	typedef void (*PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
#endif //PFNGLGENVERTEXARRAYSPROC




#ifndef PFNGLBINDSAMPLERPROC
	typedef void (*PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
#endif //PFNGLBINDSAMPLERPROC

#ifndef PFNGLDELETESAMPLERSPROC
	typedef void (*PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint * samplers);
#endif //PFNGLDELETESAMPLERSPROC

#ifndef PFNGLGENSAMPLERSPROC
	typedef void (*PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint* samplers);
#endif //PFNGLGENSAMPLERSPROC

#ifndef PFNGLGETSAMPLERPARAMETERIIVPROC
	typedef void (*PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint* params);
#endif //PFNGLGETSAMPLERPARAMETERIIVPROC

#ifndef PFNGLGETSAMPLERPARAMETERIUIVPROC
	typedef void (*PFNGLGETSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, GLuint* params);
#endif //PFNGLGETSAMPLERPARAMETERIUIVPROC

#ifndef PFNGLGETSAMPLERPARAMETERFVPROC
	typedef void (*PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat* params);
#endif //PFNGLGETSAMPLERPARAMETERFVPROC

#ifndef PFNGLGETSAMPLERPARAMETERIVPROC
	typedef void (*PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint* params);
#endif //PFNGLGETSAMPLERPARAMETERIVPROC

#ifndef PFNGLISSAMPLERPROC
	typedef GLboolean (*PFNGLISSAMPLERPROC) (GLuint sampler);
#endif //PFNGLISSAMPLERPROC

#ifndef PFNGLSAMPLERPARAMETERIIVPROC
	typedef void (*PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
#endif //PFNGLSAMPLERPARAMETERIIVPROC

#ifndef PFNGLSAMPLERPARAMETERIUIVPROC
	typedef void (*PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint* params);
#endif //PFNGLSAMPLERPARAMETERIUIVPROC

#ifndef PFNGLSAMPLERPARAMETERFPROC
	typedef void (*PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
#endif //PFNGLSAMPLERPARAMETERFPROC

#ifndef PFNGLSAMPLERPARAMETERFVPROC
	typedef void (*PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat* params);
#endif //PFNGLSAMPLERPARAMETERFVPROC

#ifndef PFNGLSAMPLERPARAMETERIPROC
	typedef void (*PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
#endif //PFNGLSAMPLERPARAMETERIPROC

#ifndef PFNGLSAMPLERPARAMETERIVPROC
	typedef void (*PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
#endif //PFNGLSAMPLERPARAMETERIVPROC

#ifndef PFNGLDEBUGMESSAGECALLBACKPROC
	typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
	typedef void (*PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const GLvoid *userParam);
#endif //PFNGLDEBUGMESSAGECALLBACKPROC




#ifndef PFNGLACTIVESHADERPROGRAMPROC
	typedef void (*PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
#endif //PFNGLACTIVESHADERPROGRAMPROC

#ifndef PFNGLBINDPROGRAMPIPELINEPROC
	typedef void (*PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
#endif //PFNGLBINDPROGRAMPIPELINEPROC

#ifndef PFNGLCREATESHADERPROGRAMVPROC
	typedef GLuint (*PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar ** strings);
#endif //PFNGLCREATESHADERPROGRAMVPROC

#ifndef PFNGLGENPROGRAMPIPELINESPROC
	typedef void (*PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
#endif //PFNGLGENPROGRAMPIPELINESPROC

#ifndef PFNGLGETPROGRAMPIPELINEINFOLOGPROC
	typedef void (*PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
#endif //PFNGLGETPROGRAMPIPELINEINFOLOGPROC

#ifndef PFNGLGETPROGRAMPIPELINEIVPROC
	typedef void (*PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
#endif //PFNGLGETPROGRAMPIPELINEIVPROC

#ifndef PFNGLISPROGRAMPIPELINEPROC
	typedef GLboolean (*PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
#endif //PFNGLACTIVESHADERPROGRAMPROC

#ifndef PFNGLPROGRAMUNIFORM1DPROC
	typedef void (*PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble x);
#endif //PFNGLPROGRAMUNIFORM1DPROC

#ifndef PFNGLPROGRAMUNIFORM1DVPROC
	typedef void (*PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORM1DVPROC

#ifndef PFNGLPROGRAMUNIFORM1FPROC
	typedef void (*PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat x);
#endif //PFNGLPROGRAMUNIFORM1FPROC

#ifndef PFNGLPROGRAMUNIFORM1FVPROC
	typedef void (*PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORM1FVPROC

#ifndef PFNGLPROGRAMUNIFORM1IPROC
	typedef void (*PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint x);
#endif //PFNGLPROGRAMUNIFORM1IPROC

#ifndef PFNGLPROGRAMUNIFORM1IVPROC
	typedef void (*PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
#endif //PFNGLPROGRAMUNIFORM1IVPROC

#ifndef PFNGLPROGRAMUNIFORM1UIPROC
	typedef void (*PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint x);
#endif //PFNGLPROGRAMUNIFORM1UIPROC

#ifndef PFNGLPROGRAMUNIFORM1UIVPROC
	typedef void (*PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
#endif //PFNGLPROGRAMUNIFORM1UIVPROC

#ifndef PFNGLPROGRAMUNIFORM2DPROC
	typedef void (*PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
#endif //PFNGLPROGRAMUNIFORM2DPROC

#ifndef PFNGLPROGRAMUNIFORM2DVPROC
	typedef void (*PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORM2DVPROC

#ifndef PFNGLPROGRAMUNIFORM2FPROC
	typedef void (*PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y);
#endif //PFNGLPROGRAMUNIFORM2FPROC

#ifndef PFNGLPROGRAMUNIFORM2FVPROC
	typedef void (*PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORM2FVPROC

#ifndef PFNGLPROGRAMUNIFORM2IPROC
	typedef void (*PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
#endif //PFNGLPROGRAMUNIFORM2IPROC

#ifndef PFNGLPROGRAMUNIFORM2IVPROC
	typedef void (*PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
#endif //PFNGLPROGRAMUNIFORM2IVPROC

#ifndef PFNGLPROGRAMUNIFORM2UIPROC
	typedef void (*PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint x, GLuint y);
#endif //PFNGLPROGRAMUNIFORM2UIPROC

#ifndef PFNGLPROGRAMUNIFORM2UIVPROC
	typedef void (*PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
#endif //PFNGLPROGRAMUNIFORM2UIVPROC

#ifndef PFNGLPROGRAMUNIFORM3DPROC
	typedef void (*PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
#endif //PFNGLPROGRAMUNIFORM3DPROC

#ifndef PFNGLPROGRAMUNIFORM3DVPROC
	typedef void (*PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORM3DVPROC

#ifndef PFNGLPROGRAMUNIFORM3FPROC
	typedef void (*PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
#endif //PFNGLPROGRAMUNIFORM3FPROC

#ifndef PFNGLPROGRAMUNIFORM3FVPROC
	typedef void (*PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORM3FVPROC

#ifndef PFNGLPROGRAMUNIFORM3IVPROC
	typedef void (*PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
#endif //PFNGLPROGRAMUNIFORM3IVPROC

#ifndef PFNGLPROGRAMUNIFORM2IPROC
	typedef void (*PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
#endif //PFNGLPROGRAMUNIFORM2IPROC

#ifndef PFNGLPROGRAMUNIFORM3IPROC
	typedef void (*PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z);
#endif //PFNGLPROGRAMUNIFORM3IPROC

#ifndef PFNGLPROGRAMUNIFORM3UIPROC
	typedef void (*PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
#endif //PFNGLPROGRAMUNIFORM3UIPROC

#ifndef PFNGLPROGRAMUNIFORM3UIVPROC
	typedef void (*PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
#endif //PFNGLPROGRAMUNIFORM3UIVPROC

#ifndef PFNGLPROGRAMUNIFORM4DPROC
	typedef void (*PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
#endif //PFNGLPROGRAMUNIFORM4DPROC

#ifndef PFNGLPROGRAMUNIFORM4DVPROC
	typedef void (*PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORM4DVPROC

#ifndef PFNGLPROGRAMUNIFORM4FPROC
	typedef void (*PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
#endif //PFNGLPROGRAMUNIFORM4FPROC

#ifndef PFNGLPROGRAMUNIFORM4FVPROC
	typedef void (*PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORM4FVPROC

#ifndef PFNGLPROGRAMUNIFORM4IPROC
	typedef void (*PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
#endif //PFNGLPROGRAMUNIFORM4IPROC

#ifndef PFNGLPROGRAMUNIFORM4IVPROC
	typedef void (*PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
#endif //PFNGLPROGRAMUNIFORM4IVPROC

#ifndef PFNGLPROGRAMUNIFORM4UIPROC
	typedef void (*PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
#endif //PFNGLPROGRAMUNIFORM4UIPROC

#ifndef PFNGLPROGRAMUNIFORM4UIVPROC
	typedef void (*PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
#endif //PFNGLPROGRAMUNIFORM4UIVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC

#ifndef PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC
	typedef void (*PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif //PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC

#ifndef PFNGLUSEPROGRAMSTAGESPROC
	typedef void (*PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
#endif //PFNGLUSEPROGRAMSTAGESPROC

#ifndef PFNGLVALIDATEPROGRAMPIPELINEPROC
	typedef void (*PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
#endif //PFNGLVALIDATEPROGRAMPIPELINEPROC

#ifndef PFNGLDELETEPROGRAMPIPELINESPROC
	typedef void (*PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
#endif //PFNGLDELETEPROGRAMPIPELINESPROC

#ifndef PFNGLPROGRAMPARAMETERIPROC
	typedef void (*PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
#endif //PFNGLPROGRAMPARAMETERIPROC

#endif //_K15_Rendering_GL_Emulation_h_