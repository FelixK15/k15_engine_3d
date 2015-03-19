
#define K15_INVALID_GL_PROGRAM_INDEX 0xffffffff
#define K15_RENDER_GL_MAX_PROGRAM_UNIFORMS 64
#define K15_RENDER_GL_MAX_INCLUDE_FILE_SIZE 32
#define K15_RENDER_GL_MAX_SHADER_CODE_SIZE size_kilobyte(512)
#define K15_RENDER_GL_MAX_INCLUDE_FILES 64
#define K15_RENDER_GL_MAX_DEFINES 64
#define K15_RENDER_GL_MAX_DEFINE_NAME_SIZE 128

enum K15_GLUniformType
{
	K15_GL_UNIFORM_TYPE_ATTRIBUTE = 0,
	K15_GL_UNIFORM_TYPE_UNIFORM,
	K15_GL_UNIFORM_TYPE_UNKNOWN
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

struct K15_GLProgramInclude
{
	uint32 line;
	char* includeFile;
};

struct K15_GLProgramDefine
{
	uint32 line;
	char* define;
};

struct K15_GLProgramPreprocessor
{
	K15_GLProgramInclude includes[K15_RENDER_GL_MAX_INCLUDE_FILES];
	K15_GLProgramDefine defines[K15_RENDER_GL_MAX_DEFINES];
	uint32 amountDefines;
	uint32 amountIncludes;
};

K15_GLProgramDesc internOpenGLPrograms[K15_RENDER_MAX_GPU_PROGRAMS] = {};

/*********************************************************************************/
intern void K15_InternalGLGetIncludeFileName(const char* p_IncludeLine, char* p_IncludeFileBuffer)
{
	const char* start = strchr(p_IncludeLine, '\"') + 1;
	const char* end = strrchr(p_IncludeLine, '\"');

	ptrdiff_t startSize = (ptrdiff_t)start;
	ptrdiff_t endSize = (ptrdiff_t)end;

	ptrdiff_t includeFileNameSize = endSize - startSize;
	ptrdiff_t startIndex = startSize - (ptrdiff_t)p_IncludeLine;

	memcpy(p_IncludeFileBuffer, p_IncludeLine + startIndex, includeFileNameSize);

	p_IncludeFileBuffer[includeFileNameSize] = 0;
}
/*********************************************************************************/
intern char* K15_InternalGLPreprocessProgramCode(const char* p_FilePath, const char* p_ProgramCode)
{
	char* path = K15_GetPathWithoutFileName(p_FilePath);
	char* preprocessedCode = (char*)malloc(K15_RENDER_GL_MAX_SHADER_CODE_SIZE);
	uint32 preprocessedCodeIndex = 0; //position where in the preprocessed code we currently are

	const char* currentLine = p_ProgramCode;
	uint32 lineCount = 0;

	while (currentLine)
	{
		lineCount += 1;

		const char* nextLine = strchr(currentLine, '\n');
		size_t lineLength = nextLine ? (nextLine-currentLine) : strlen(currentLine);
		char* tempCurrentLine = (char*)alloca(lineLength + 1);
		memcpy(tempCurrentLine, currentLine, lineLength);
		tempCurrentLine[lineLength] = 0;

		if (lineLength > 2
			&& (tempCurrentLine[0] == '/' 
			&& tempCurrentLine[1] == '/'))
		{
			//skip comments
			continue;
		}


		if (strstr(tempCurrentLine, "#include") != 0)
		{
			//include found
			char* includeFileNameBuffer = (char*)malloc(K15_RENDER_GL_MAX_INCLUDE_FILE_SIZE);
			uint32 pathLength = strlen(path);
			memcpy(includeFileNameBuffer, path, pathLength);
			K15_InternalGLGetIncludeFileName(tempCurrentLine, includeFileNameBuffer + pathLength);
			
			if (K15_FileExists(includeFileNameBuffer) != K15_SUCCESS)
			{
				//TODO: ERROR
			}
			else
			{
				//replace with content from file
				char* includeFileContent = K15_GetWholeFileContent(includeFileNameBuffer);
				char* preprocessedIncludeFileContent = K15_InternalGLPreprocessProgramCode(includeFileNameBuffer, includeFileContent);

				//after the content got preprocessed, we don't need the un-preprocessed code anymore.
				free(includeFileContent);

				size_t fileContentSize = K15_GetFileSize(includeFileNameBuffer);

				memcpy(preprocessedCode + preprocessedCodeIndex, preprocessedIncludeFileContent, fileContentSize);
				preprocessedCodeIndex += fileContentSize;

				//after the preprocessed content got added to the current file content, the preprocessed content is not needed anymore
				free(preprocessedIncludeFileContent);
			}

			free(includeFileNameBuffer);
		}
		else
		{
			//add unmodified line
			memcpy(preprocessedCode + preprocessedCodeIndex, currentLine, lineLength);
			preprocessedCodeIndex += lineLength;
		}

		currentLine = nextLine ? (nextLine + 1) : 0;
		preprocessedCode[preprocessedCodeIndex++] = '\n';
	}

	preprocessedCode[preprocessedCodeIndex] = 0;

	return preprocessedCode;
}
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
void K15_GLInitPrograms()
{
	K15_GLProgramDesc* currentProgramDesc = 0;
	K15_GLUniform* currentUniform = 0;

	//initialize internal structures
	for (uint32 programIndex = 0;
		 programIndex < K15_RENDER_MAX_GPU_PROGRAMS;
		 ++programIndex)
	{
		currentProgramDesc = &internOpenGLPrograms[programIndex];

		for (uint32 uniformIndex = 0;
			 uniformIndex < K15_RENDER_GL_MAX_PROGRAM_UNIFORMS;
			 ++uniformIndex)
		{
			currentUniform = &currentProgramDesc->uniforms[uniformIndex];

			currentUniform->registerIndex = -1;
			currentUniform->size = -1;
			currentUniform->uniformType = K15_GL_UNIFORM_TYPE_UNKNOWN;
			currentUniform->valueType = 0;
		}
	}
}
/*********************************************************************************/
uint8 K15_GLCreateProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;

	K15_RenderProgramType renderProgramType = p_RenderProgramDesc->type;

	GLenum glProgramType = K15_GLConvertProgramType(renderProgramType);
	const char* programCode = 0;
	const char* programFilePath = p_RenderProgramDesc->file;
	
	//check where we have to get the shader code from
	if (p_RenderProgramDesc->source == K15_RENDER_PROGRAM_SOURCE_FILE)
	{
		if (K15_FileExists(programFilePath) != K15_SUCCESS)
		{
			result = K15_ERROR_FILE_NOT_FOUND;
		}
		else
		{
			char* codeBuffer = K15_GetWholeFileContent(programFilePath);
			programCode = codeBuffer;

			p_RenderProgramDesc->code = codeBuffer;
		}
	}
	else
	{
		programCode = p_RenderProgramDesc->code;
	}

	char* parsedProgramCode = K15_InternalGLPreprocessProgramCode(programFilePath, programCode);
	free((void*)programCode); //free programcode after it got preprocessed

	const char* glslProgramCode[] = {
		"#version 330\n",
		0, //maybe filled later based on the program type
		parsedProgramCode
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
	
	free(parsedProgramCode); //after the shader got compiled, delete the program code

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