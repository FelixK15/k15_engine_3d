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

typedef uint8(*K15_InternalGLUpdateUniformFnc)(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc);

/*********************************************************************************/
intern void K15_InternalGLUpdateUniform(uint32 p_TargetSize, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, void* p_UniformData)
{
	byte* uniformData = p_RenderUniformUpdateDesc->data;
	uint32 uniformDataSize = p_RenderUniformUpdateDesc->size;

	assert(uniformDataSize <= p_TargetSize);

	memcpy(p_UniformData, uniformData, uniformDataSize);
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateIntUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	int32 uniformValue = 0;

	K15_InternalGLUpdateUniform(sizeof(int32), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1i(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateUIntUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	uint32 uniformValue = 0;

	K15_InternalGLUpdateUniform(sizeof(uint32), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1ui(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloatUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValue = 0.f;

	K15_InternalGLUpdateUniform(sizeof(float), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1f(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat2Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[2] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 2, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform2f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat3Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[3] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 3, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform3f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1], uniformValues[2]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat4Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[4] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 4, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform4f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1], uniformValues[2], uniformValues[3]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat2x2Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[2*2] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (2*2), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix2fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat3x3Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[3*3] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (3*3), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix3fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat4x4Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->program;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[4*4] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (4*4), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix4fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
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
	size_t preprocessedCodeIndex = 0; //position where in the preprocessed code we currently are

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
			size_t pathLength = strlen(path);
			memcpy(includeFileNameBuffer, path, pathLength);
			K15_InternalGLGetIncludeFileName(tempCurrentLine, includeFileNameBuffer + pathLength);
			
			if (K15_FileExists(includeFileNameBuffer) != K15_SUCCESS)
			{
				//TODO: ERROR
			}
			else
			{
				//replace with content from file
				uint32 fileSize = 0;
				char* includeFileContent = (char*)K15_GetWholeFileContentWithFileSize(includeFileNameBuffer, &fileSize);
				includeFileContent[fileSize] = 0;

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
intern void K15_InternGLReflectProgram(K15_GLProgram* p_Program, K15_RenderProgramDesc* p_RenderProgramDesc)
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
		K15_OPENGL_CALL(kglGetActiveUniform(glProgram, activeUniformIndex, nameBufferSize, 0, &currentValueSize, &currentValueType, nameBuffer));

		//copy name for later use
		GLchar* name = (GLchar*)K15_CopyString(nameBuffer);

		//get register location using uniform name
		K15_OPENGL_CALL(uniformRegister = kglGetUniformLocation(glProgram, nameBuffer));

		p_Program->uniforms[activeUniformIndex].registerIndex = uniformRegister;
		p_Program->uniforms[activeUniformIndex].size = currentValueSize;
		p_Program->uniforms[activeUniformIndex].internalGLType = currentValueType;
		p_Program->uniforms[activeUniformIndex].uniformUsage = K15_GL_UNIFORM_USAGE_UNIFORM;
		p_Program->uniforms[activeUniformIndex].uniformType = K15_GLConvertGLTypeToUniformType(currentValueType, currentValueSize);
		p_Program->uniforms[activeUniformIndex].name = name;

		K15_AutoUniformType autoType = K15_ConvertToAutoType(name);

		p_RenderProgramDesc->uniforms[activeUniformIndex].type = K15_GLConvertGLTypeToUniformType(currentValueType, currentValueSize);
		p_RenderProgramDesc->uniforms[activeUniformIndex].name = name;
		p_RenderProgramDesc->uniforms[activeUniformIndex].autoType = autoType;
		p_RenderProgramDesc->uniforms[activeUniformIndex].autoTypeGroup = K15_ConvertToAutoTypeGroup(autoType);

		//memcpy(p_Program->uniforms[activeUniformIndex].name, nameBuffer, nameLength);

		//clear name buffer
		memset(nameBuffer, 0, nameBufferSize);
	}

	//attributes
	for (GLint activeAttributeIndex = 0;
		activeAttributeIndex < amountActiveAttributes;
		++activeAttributeIndex)
	{
		K15_OPENGL_CALL(kglGetActiveAttrib(glProgram, activeAttributeIndex, nameBufferSize, 0, &currentValueSize, &currentValueType, nameBuffer));

		//copy name for later use
		GLchar* name =  (GLchar*)K15_CopyString(nameBuffer);

		//get register location using uniform name
		K15_OPENGL_CALL(uniformRegister = kglGetAttribLocation(glProgram, nameBuffer));

		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].registerIndex = uniformRegister;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].size = currentValueSize;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].internalGLType = currentValueType;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].uniformUsage = K15_GL_UNIFORM_USAGE_ATTRIBUTE;
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].uniformType = K15_GLConvertGLTypeToUniformType(currentValueType, currentValueSize);
		p_Program->uniforms[activeAttributeIndex + amountActiveUniforms].name = name;

		//clear name buffer
		memset(nameBuffer, 0, nameBufferSize);
	}

	p_Program->uniformCount = (uint32)(amountActiveUniforms + amountActiveAttributes);

}
/*********************************************************************************/
intern K15_InternalGLUpdateUniformFnc K15_InternalGLConvertToUpdateUniformFnc(K15_UniformType p_UniformType)
{
	K15_InternalGLUpdateUniformFnc updateUniformFnc = 0;

	switch(p_UniformType)
	{
		case K15_UNIFORM_TYPE_INT32:
		{
			updateUniformFnc = K15_InternalGLUpdateIntUniform;
			break;
		}

		case K15_UNIFORM_TYPE_UINT32:
		{
			updateUniformFnc = K15_InternalGLUpdateUIntUniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT:
		{
			updateUniformFnc = K15_InternalGLUpdateFloatUniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat2Uniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2x2:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat2x2Uniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat3Uniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3x3:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat3x3Uniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat4Uniform;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4x4:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat4x4Uniform;
			break;
		}

		default:
		{
			assert(false);
			break;
		}
	}

	return updateUniformFnc;
}
/*********************************************************************************/
int K15_GLUniformNameCompare(const void* p_UniformName, const void* p_Uniform)
{
	K15_GLUniform* glUniform = (K15_GLUniform*)(p_Uniform);
	const char* uniformName = *(const char**)(p_UniformName);

	return strcmp(glUniform->name, uniformName);
}
/*********************************************************************************/
const char* K15_GLGenerateGLSLHeaderCode(GLenum p_GLProgramType)
{
	const char* glslHeaderCode = 0;

	if (p_GLProgramType == GL_VERTEX_SHADER)
	{
		glslHeaderCode = "#version 330\n"
			"out gl_PerVertex{\n"
			"vec4 gl_Position;\n"
			"float gl_PointSize;\n"
			"float gl_ClipDistance[];\n"
			"};\n";
	}
	else
	{
		glslHeaderCode = "#version 330\n";
	}
	
	return glslHeaderCode;
}
/*********************************************************************************/
void K15_GLInitPrograms(K15_GLRenderContext* p_GLContext)
{
	K15_GLProgram* currentProgram = 0;
	K15_GLUniform* currentUniform = 0;

	//initialize internal structures
	for (uint32 programIndex = 0;
		 programIndex < K15_RENDER_MAX_GPU_PROGRAMS;
		 ++programIndex)
	{
		currentProgram = &p_GLContext->glObjects.programs[programIndex];

		for (uint32 uniformIndex = 0;
			 uniformIndex < K15_RENDER_GL_MAX_PROGRAM_UNIFORMS;
			 ++uniformIndex)
		{
			currentUniform = &currentProgram->uniforms[uniformIndex];

			currentUniform->registerIndex = -1;
			currentUniform->size = -1;
			currentUniform->uniformUsage = K15_GL_UNIFORM_USAGE_UNKNOWN;
			currentUniform->internalGLType = 0;
		}
	}
}
/*********************************************************************************/
uint8 K15_GLCreateProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;
	K15_RenderProgramType renderProgramType = p_RenderProgramDesc->type;

	GLenum glProgramType = K15_GLConvertProgramType(renderProgramType);
	const char* programCode = 0;
	const char* programFilePath = p_RenderProgramDesc->file;

	//if no path was supplied, just take the root directory
	if (!programFilePath)
	{
		programFilePath = "./";
	}
	
	//check where we have to get the shader code from
	if (p_RenderProgramDesc->source == K15_RENDER_PROGRAM_SOURCE_FILE)
	{
		if (K15_FileExists(programFilePath) != K15_SUCCESS)
		{
			result = K15_OS_ERROR_FILE_NOT_FOUND;
		}
		else
		{
			char* codeBuffer = (char*)K15_GetWholeFileContent(programFilePath);
			programCode = codeBuffer;

			p_RenderProgramDesc->code = codeBuffer;
		}
	}
	else
	{
		programCode = p_RenderProgramDesc->code;
	}

	char* parsedProgramCode = K15_InternalGLPreprocessProgramCode(programFilePath, programCode);
	//free((void*)programCode); //free programcode after it got preprocessed

	const char* glslProgramCode[] = {
		K15_GLGenerateGLSLHeaderCode(glProgramType),
		parsedProgramCode
	};

	GLuint glProgram = 0;
	GLint linkStatus = GL_FALSE;

	K15_OPENGL_CALL(glProgram = kglCreateShaderProgramv(glProgramType, K15_ARRAY_COUNT(glslProgramCode), glslProgramCode));
	K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus));

	K15_GLProgram* program = &glContext->glObjects.programs[*p_RenderProgramHandle];
	program->program = glProgram;
	program->programType = glProgramType;

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
		K15_InternGLReflectProgram(program, p_RenderProgramDesc);
	}

	/*if (glProgramType == GL_FRAGMENT_SHADER)
	{
		K15_OPENGL_CALL(kglBindFragDataLocation(glProgram, 0, "outColor0"));
	}*/

	return result;
}
/*********************************************************************************/
uint8 K15_GLDeleteProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLProgram* glProgram = &glContext->glObjects.programs[*p_RenderProgramHandle];
	GLenum glProgramType = glProgram->programType;
	K15_RenderProgramType programType = K15_ConvertGLProgramType(glProgramType);

	K15_OPENGL_CALL(kglDeleteProgram(glProgram->program));

	glContext->glBoundObjects.boundPrograms[programType] = 0;
	glContext->glObjects.programs[*p_RenderProgramHandle].program = K15_INVALID_GL_PROGRAM_INDEX;

	return result;
}
/*********************************************************************************/
uint8 K15_GLBindProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	GLuint glProgramPipelineHandle = glContext->gl3.programPipelineHandle;
	K15_GLProgram* glProgram = &glContext->glObjects.programs[*p_RenderProgramHandle];
	GLenum glProgramType = glProgram->programType;

	K15_RenderProgramType programType = K15_ConvertGLProgramType(glProgramType);
	if (glContext->glBoundObjects.boundPrograms[programType] == glProgram)
	{
		return K15_SUCCESS;
	}

	GLuint glProgramHandle = glProgram->program;
	GLbitfield glProgramTypeBit = K15_GLConvertProgramTypeBit(glProgramType);

	K15_OPENGL_CALL(kglUseProgramStages(glProgramPipelineHandle, glProgramTypeBit ,glProgramHandle));

	glContext->glBoundObjects.boundPrograms[programType] = glProgram;

	return result;
}
/*********************************************************************************/
uint8 K15_GLUpdateDirtyUniforms(K15_RenderContext* p_RenderContext)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_RenderUniformCache* uniformCache = p_RenderContext->uniformCache;
	//TEST ME
	for (uint32 dirtyUniformIndex = 0;
		dirtyUniformIndex < K15_AUTO_UNIFORM_TYPE_COUNT;
		++dirtyUniformIndex)
	{
		if (uniformCache->dirtyAutoUniforms[dirtyUniformIndex])
		{
			K15_RenderUniformCacheEntry* cachedDirtyUniform = uniformCache->dirtyAutoUniforms[dirtyUniformIndex];
			const char* cachedDirtyUniformName = cachedDirtyUniform->name;

			for (uint32 gpuProgramIndex = 0;
				gpuProgramIndex < K15_RENDER_PROGRAM_TYPE_COUNT;
				++gpuProgramIndex)
			{
				K15_GLProgram* boundProgram = glContext->glBoundObjects.boundPrograms[gpuProgramIndex];

				if (boundProgram)
				{
					uint32 programUniformCount = boundProgram->uniformCount;

					for (uint32 programUniformIndex = 0;
						programUniformIndex < programUniformCount;
						++programUniformIndex)
					{
						K15_GLUniform* glUniform = &boundProgram->uniforms[programUniformIndex];
						char* glUniformName = glUniform->name;
						K15_UniformType uniformType = glUniform->uniformType;

						if (strcmp(glUniformName, cachedDirtyUniformName) == 0)
						{
							K15_InternalGLUpdateUniformFnc updateUniform = K15_InternalGLConvertToUpdateUniformFnc(uniformType);
							K15_RenderUniformUpdateDesc updateUniformDesc = {};

							updateUniformDesc.type = uniformType;
							updateUniformDesc.name = glUniformName;
							updateUniformDesc.size = K15_ConvertToUniformByteSize(uniformType);
							updateUniformDesc.data = cachedDirtyUniform->data;
							updateUniformDesc.flags = 0;

							updateUniform(boundProgram, glUniform->registerIndex, &updateUniformDesc);
							break;
						}
					}
				}
			}

			uniformCache->dirtyAutoUniforms[dirtyUniformIndex] = 0;
		}
	}

	return result;
}
/*********************************************************************************/
uint8 K15_GLUpdateUniform(K15_RenderContext* p_RenderContext, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_ERROR_RENDER_UNIFORM_NOT_FOUND;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLProgram* glProgram = &glContext->glObjects.programs[*p_RenderProgramHandle];

	GLenum uniformType = K15_GLConvertUniformTypeToGLType(p_RenderUniformUpdateDesc->type);

	uint32 uniformCount = glProgram->uniformCount;

	for (uint32 uniformIndex = 0;
		uniformIndex < uniformCount;
		++uniformIndex)
	{
		GLchar* uniformName = glProgram->uniforms[uniformIndex].name;

		if (strcmp(uniformName, p_RenderUniformUpdateDesc->name) == 0)
		{
			switch (uniformType)
			{
				case GL_INT:
				{
					result = K15_InternalGLUpdateIntUniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_UNSIGNED_INT:
				{
					result = K15_InternalGLUpdateUIntUniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT:
				{
					result = K15_InternalGLUpdateFloatUniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_VEC2:
				{
					result = K15_InternalGLUpdateFloat2Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_VEC3:
				{
					result = K15_InternalGLUpdateFloat3Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_VEC4:
				{
					result = K15_InternalGLUpdateFloat4Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_MAT2:
				{
					result = K15_InternalGLUpdateFloat2x2Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_MAT3:
				{
					result = K15_InternalGLUpdateFloat3x3Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				case GL_FLOAT_MAT4:
				{
					result = K15_InternalGLUpdateFloat4x4Uniform(glProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

				default:
				{
					assert(false);
				}
			}

			break;
		}
	}

	return result;
}
/*********************************************************************************/