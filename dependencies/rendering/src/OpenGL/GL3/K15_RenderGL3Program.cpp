
#define K15_INVALID_GL_PROGRAM_INDEX 0xffffffff
#define K15_RENDER_GL_MAX_PROGRAM_UNIFORMS 64

enum K15_GLUniformType
{
	K15_GL_UNIFORM_TYPE_ATTRIBUTE = 0,
	K15_GL_UNIFORM_TYPE_UNIFORM
};

struct K15_GLUniform
{
	K15_GLUniformType uniformType;
	GLenum valueType;
	GLint size;
	GLint registerIndex; 
};

struct K15_GLProgramDesc
{
	K15_GLUniform uniforms[K15_RENDER_GL_MAX_PROGRAM_UNIFORMS];
	uint32 uniformCount;
	GLuint program;
};

K15_GLProgramDesc internOpenGLPrograms[K15_RENDER_MAX_GPU_PROGRAMS] = {};

/*********************************************************************************/
intern void K15_InternGLReflectProgram(K15_GLProgramDesc* p_Program)
{
	uint32 uniformCounter = 0;
	GLuint glProgram = p_Program->program;

	GLint amountActiveUniforms = 0;
	GLint amountActiveAttributes = 0;

	GLint maxUniformNameSize = 0;
	GLint maxAttributeNameSize = 0;

	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_ACTIVE_ATTRIBUTES, &amountActiveAttributes));
	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_ACTIVE_UNIFORMS, &amountActiveUniforms));
	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameSize));
	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameSize));

	GLint nameBufferSize = maxUniformNameSize > maxAttributeNameSize ? maxUniformNameSize : maxAttributeNameSize;
	GLint uniformRegister = 0;
	GLint currentValueSize = 0;
	GLenum currentValueType = 0;
	char* nameBuffer = (char*)alloca(nameBufferSize);

	//uniforms
	for (GLint activeUniformIndex = 0;
		 activeUniformIndex < amountActiveUniforms;
		 ++activeUniformIndex)
	{
		kglGetActiveUniform(glProgram, activeUniformIndex, nameBufferSize, 0, &currentValueSize, &currentValueType, nameBuffer);

		//get register location using uniform name
		uniformRegister = kglGetUniformLocation(glProgram, nameBuffer);

		p_Program->uniforms[activeUniformIndex].registerIndex = uniformRegister;
		p_Program->uniforms[activeUniformIndex].size = currentValueSize;
		p_Program->uniforms[activeUniformIndex].valueType = currentValueType;
		p_Program->uniforms[activeUniformIndex].uniformType = K15_GL_UNIFORM_TYPE_UNIFORM;

		//clear name buffer
		memset(nameBuffer, 0, nameBufferSize);
	}

	//attributes
	for (GLint activeAttributeIndex = 0;
		activeAttributeIndex < amountActiveAttributes;
		++activeAttributeIndex)
	{
		kglGetActiveAttrib(glProgram, activeAttributeIndex, nameBufferSize, 0, &currentValueSize, &currentValueType, nameBuffer);

		//get register location using uniform name
		uniformRegister = kglGetAttribLocation(glProgram, nameBuffer);

		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].registerIndex = uniformRegister;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].size = currentValueSize;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].valueType = currentValueType;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].uniformType = K15_GL_UNIFORM_TYPE_ATTRIBUTE;

		//clear name buffer
		memset(nameBuffer, 0, nameBufferSize);
	}


	p_Program->uniformCount = (uint32)(amountActiveUniforms + amountActiveAttributes);

}
/*********************************************************************************/
uint8 K15_GLCreateProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;

	K15_RenderProgramType renderProgramType = p_RenderProgramDesc->type;

	GLenum glProgramType = K15_GLConvertProgramType(renderProgramType);
	const char* programCode = 0;

	//check where we have to get the shader code from
	if (p_RenderProgramDesc->source == K15_RENDER_PROGRAM_SOURCE_FILE)
	{
		const char* programFilePath = p_RenderProgramDesc->file;

		if (K15_FileExists(programFilePath) != K15_SUCCESS)
		{
			result = K15_ERROR_FILE_NOT_FOUND;
		}
		else
		{
			uint32 fileSize = K15_GetFileSize(programFilePath);
			char* codeBuffer = (char*)malloc(fileSize + 1); //+1 for 0 terminator

			FILE* programFile = fopen(programFilePath, "r");
			fread(codeBuffer, sizeof(char), fileSize, programFile);
			fclose(programFile);

			p_RenderProgramDesc->code = codeBuffer;
		}
	}
	else
	{
		programCode = p_RenderProgramDesc->code;
	}

	const char* glslProgramCode[] = {
		"#version 330\n",
		0, //maybe filled later based on the program type
		programCode
	};

	if (glProgramType == GL_VERTEX_SHADER)
	{
		//needs to be manually added for separate vertex OpenGL shader
		glslProgramCode[1] = "out gl_PerVertex {\n"
			"vec4 gl_Position;\n"
			"float gl_PointSize;\n"
			"float gl_ClipDistance[];\n"
			"};\n";
	}

	GLuint glProgram = 0;
	GLint linkStatus = GL_FALSE;

	K15_OPENGL_CALL(glProgram = kglCreateShaderProgramv(glProgramType, K15_ARRAY_COUNT(glslProgramCode), glslProgramCode));
	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus));

	K15_GLProgramDesc* programDesc = &internOpenGLPrograms[*p_RenderProgramHandle];
	programDesc->program = glProgram;

	if (linkStatus == GL_FALSE)
	{
		GLint infoLogLength = 0;
		K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &infoLogLength));

		if (infoLogLength == 0)
		{
			result = K15_ERROR_RENDER_API;
		}
		else
		{
			char* infoLogBuffer = (char*)malloc(infoLogLength);
			K15_OPENGL_CALL(kglGetProgramInfoLog(glProgram, infoLogLength, 0, infoLogBuffer));
			K15_SetRenderContextError(p_RenderContext, infoLogBuffer, infoLogLength);

			result = K15_ERROR_RENDER_PROGRAM_COMPILATION;
		}
	}

	//if everything went smooth so far, try to reflect the program
	if (result == K15_SUCCESS)
	{
		K15_InternGLReflectProgram(programDesc);
	}

	return result;
}
/*********************************************************************************/
uint8 K15_GLDeleteProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;
	K15_GLProgramDesc* glProgramDesc = &internOpenGLPrograms[*p_RenderProgramHandle];

	K15_OPENGL_CALL(kglDeleteProgram(glProgramDesc->program));

	internOpenGLPrograms[*p_RenderProgramHandle].program = K15_INVALID_GL_PROGRAM_INDEX;

	return K15_SUCCESS;
}
/*********************************************************************************/