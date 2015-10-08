/*********************************************************************************/
struct K15_GLProgramInclude
{
	uint32 line;
	char* includeFile;
};
/*********************************************************************************/
struct K15_GLProgramDefine
{
	uint32 line;
	char* define;
};
/*********************************************************************************/
struct K15_GLProgramPreprocessor
{
	K15_GLProgramInclude includes[K15_RENDER_GL_MAX_INCLUDE_FILES];
	K15_GLProgramDefine defines[K15_RENDER_GL_MAX_DEFINES];
	uint32 amountDefines;
	uint32 amountIncludes;
};
/*********************************************************************************/

#include "K15_ShaderCompiler.h"
#include "K15_RenderShaderSemantics.h"

typedef uint8(*K15_InternalGLUpdateUniformFnc)(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc);

/*********************************************************************************/
intern void K15_InternalGLUpdateUniform(uint32 p_TargetSize, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, void* p_UniformData)
{
	byte* uniformData = 0;
	
	if (p_RenderUniformUpdateDesc->typeID == K15_TYPE_SAMPLER_2D_ID)
	{
		uniformData = (byte*)&p_RenderUniformUpdateDesc->data.textureSlot;
	}
	else
	{
		uniformData = p_RenderUniformUpdateDesc->data.rawData;
	}
	uint32 uniformDataSize = p_RenderUniformUpdateDesc->sizeInBytes;

	assert(uniformDataSize <= p_TargetSize);

	memcpy(p_UniformData, uniformData, uniformDataSize);
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateIntUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	int32 uniformValue = 0;

	K15_InternalGLUpdateUniform(sizeof(int32), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1i(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateUIntUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	uint32 uniformValue = 0;

	K15_InternalGLUpdateUniform(sizeof(uint32), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1ui(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloatUniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValue = 0.f;

	K15_InternalGLUpdateUniform(sizeof(float), p_RenderUniformUpdateDesc, &uniformValue);

	K15_OPENGL_CALL(kglProgramUniform1f(glProgramHandle, glUniform->registerIndex, uniformValue));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat2Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[2] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 2, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform2f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat3Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[3] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 3, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform3f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1], uniformValues[2]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat4Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[4] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * 4, p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniform4f(glProgramHandle, glUniform->registerIndex, uniformValues[0], uniformValues[1], uniformValues[2], uniformValues[3]));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat2x2Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[2*2] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (2*2), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix2fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat3x3Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[3*3] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (3*3), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix3fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalGLUpdateFloat4x4Uniform(K15_GLProgram* p_GLProgram, uint32 p_UniformIndex, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	GLuint glProgramHandle = p_GLProgram->glProgramHandle;
	K15_GLUniform* glUniform = &p_GLProgram->uniforms[p_UniformIndex];

	float uniformValues[4*4] = {0.f};

	K15_InternalGLUpdateUniform(sizeof(float) * (4*4), p_RenderUniformUpdateDesc, &uniformValues);

	K15_OPENGL_CALL(kglProgramUniformMatrix4fv(glProgramHandle, glUniform->registerIndex, 1, K15_GL_TRANSPOSE_MATRIX, uniformValues));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern K15_GLUniform* K15_InternalGLGetProgramUniformByNameHash(K15_GLProgram* p_GLProgram, uint32 p_UniformNameHash)
{
	K15_GLUniform* uniform = 0;

	for (uint32 uniformIndex = 0;
		uniformIndex < p_GLProgram->uniformCount;
		++uniformIndex)
	{
		K15_GLUniform* currentUniform = &p_GLProgram->uniforms[uniformIndex];

		if (currentUniform->nameHash == p_UniformNameHash)
		{
			uniform = currentUniform;
			break;
		}
	}

	return uniform;
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
intern uint32 K15_InternalTypeSizeDivisor(uint32 p_TypeID)
{
	uint32 divisor = 1;

	switch(p_TypeID)
	{
		case K15_TYPE_FLOAT_ID:
		case K15_TYPE_FLOAT_VECTOR2_ID:
		case K15_TYPE_FLOAT_VECTOR3_ID:
		case K15_TYPE_FLOAT_VECTOR4_ID:
		{
			divisor = sizeof(float);
			break;
		}

		case K15_TYPE_INT_ID:
		{
			divisor = sizeof(int);
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return divisor;
}
/*********************************************************************************/
intern char* K15_InternalGLPreprocessProgramCode(K15_CustomMemoryAllocator* p_MemoryAllocator, const char* p_FilePath, const char* p_ProgramCode)
{
	char* path = K15_GetPathWithoutFileName(p_FilePath);
	char* preprocessedCode = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, K15_RENDER_GL_MAX_SHADER_CODE_SIZE);
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
			char* includeFileNameBuffer = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, K15_RENDER_GL_MAX_INCLUDE_FILE_SIZE);
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

				char* preprocessedIncludeFileContent = K15_InternalGLPreprocessProgramCode(p_MemoryAllocator, includeFileNameBuffer, includeFileContent);

				//after the content got preprocessed, we don't need the un-preprocessed code anymore.
				free(includeFileContent);

				size_t fileContentSize = K15_GetFileSize(includeFileNameBuffer);

				memcpy(preprocessedCode + preprocessedCodeIndex, preprocessedIncludeFileContent, fileContentSize);
				preprocessedCodeIndex += fileContentSize;

				//after the preprocessed content got added to the current file content, the preprocessed content is not needed anymore
				K15_FreeFromMemoryAllocator(p_MemoryAllocator, preprocessedIncludeFileContent);
			}

			K15_FreeFromMemoryAllocator(p_MemoryAllocator, includeFileNameBuffer);
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
intern void K15_InternGLReflectProgram(K15_RenderContext* p_RenderContext, K15_GLProgram* p_Program, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	K15_CustomMemoryAllocator* memoryAllocator = &p_RenderContext->memoryAllocator;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderContext->backEnd.shaderProcessorContext;

	uint32 uniformCounter = 0;
	uint32 offset = 0;
	GLuint glProgram = p_Program->glProgramHandle;

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
		uint32 nameHash = K15_GenerateStringHash(nameBuffer);

		//get register location using uniform name
		K15_OPENGL_CALL(uniformRegister = kglGetUniformLocation(glProgram, nameBuffer));

		K15_ShaderArgument* argument = K15_GetShaderArgumentByName(&p_Program->shaderInformations, nameBuffer);

		uint32 typeID = argument->typeID;
		uint32 semanticID = argument->semanticID;
		uint32 semanticGroupID = argument->semanticGroupID;

		//should NEVER fail
		K15_ASSERT(typeID != K15_INVALID_ID);
		K15_ASSERT(semanticID != K15_INVALID_ID);

		p_Program->uniforms[activeUniformIndex].registerIndex = uniformRegister;
		p_Program->uniforms[activeUniformIndex].sizeInBytes = currentValueSize;
		p_Program->uniforms[activeUniformIndex].internalGLType = currentValueType;
		p_Program->uniforms[activeUniformIndex].typeID = typeID;
		p_Program->uniforms[activeUniformIndex].name = name;
		p_Program->uniforms[activeUniformIndex].nameHash = nameHash;

		/*p_RenderProgramDesc->uniforms[activeUniformIndex].typeID = typeID;
		p_RenderProgramDesc->uniforms[activeUniformIndex].name = name;
		p_RenderProgramDesc->uniforms[activeUniformIndex].semanticID = semanticID;
		p_RenderProgramDesc->uniforms[activeUniformIndex].semanticGroupID = semanticGroupID;*/

		//memcpy(p_Program->uniforms[activeUniformIndex].name, nameBuffer, nameLength);

		//clear name buffer
		memset(nameBuffer, 0, nameBufferSize);
	}

	p_Program->inputLayout.numInputElements = amountActiveAttributes;

	if (amountActiveAttributes > 0)
	{
		//attributes
		p_Program->inputLayout.inputElements = (K15_GLInputLayoutElement*)K15_AllocateFromMemoryAllocator(memoryAllocator, amountActiveAttributes * sizeof(K15_GLInputLayoutElement));

		//get attribute data
		for (GLint activeAttributeIndex = 0;
			activeAttributeIndex < amountActiveAttributes;
			++activeAttributeIndex)
		{
			//currentValueSize = how often is this value used? (e.g. vec2 == 1, vec2[2] == 2)
			K15_OPENGL_CALL(kglGetActiveAttrib(glProgram, activeAttributeIndex, nameBufferSize, 0, &currentValueSize, &currentValueType, nameBuffer));

			//get register location using attribute name
			K15_OPENGL_CALL(uniformRegister = kglGetAttribLocation(glProgram, nameBuffer));

			K15_ShaderArgument* argument = K15_GetShaderArgumentByName(&p_Program->shaderInformations, nameBuffer);

			uint32 typeID = argument->typeID;
			uint32 semanticID = argument->semanticID;

			//should NEVER fail
			K15_ASSERT(typeID != K15_INVALID_ID);
			K15_ASSERT(semanticID != K15_INVALID_ID);

			uint32 attributeSizeInBytes = K15_GetTypeSizeInBytesByTypeID(shaderProcessorContext, typeID);
			uint32 sizeDivisor = K15_InternalTypeSizeDivisor(typeID);

			p_Program->inputLayout.inputElements[activeAttributeIndex].glRegisterIndex = uniformRegister;
			p_Program->inputLayout.inputElements[activeAttributeIndex].glType = K15_GLConvertToSingleType(currentValueType);
			p_Program->inputLayout.inputElements[activeAttributeIndex].elementSize = attributeSizeInBytes / sizeDivisor;
			p_Program->inputLayout.inputElements[activeAttributeIndex].sizeInBytes = attributeSizeInBytes;
			p_Program->inputLayout.inputElements[activeAttributeIndex].offset = (GLvoid*)offset;
			p_Program->inputLayout.inputElements[activeAttributeIndex].typeID = typeID;
			p_Program->inputLayout.inputElements[activeAttributeIndex].semanticID = semanticID;

			offset += attributeSizeInBytes;
		}
	}

	p_Program->uniformCount = (uint32)(amountActiveUniforms);

}
/*********************************************************************************/
intern K15_InternalGLUpdateUniformFnc K15_InternalGLConvertToUpdateUniformFnc(uint32 p_TypeID)
{
	K15_InternalGLUpdateUniformFnc updateUniformFnc = 0;

	switch(p_TypeID)
	{
		case K15_TYPE_INT_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateIntUniform;
			break;
		}

// 		case K15_UNIFORM_TYPE_UINT32:
// 		{
// 			updateUniformFnc = K15_InternalGLUpdateUIntUniform;
// 			break;
// 		}

		case K15_TYPE_FLOAT_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloatUniform;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR2_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat2Uniform;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX2_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat2x2Uniform;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR3_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat3Uniform;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX3_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat3x3Uniform;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR4_ID:
		{
			updateUniformFnc = K15_InternalGLUpdateFloat4Uniform;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX4_ID:
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
// int K15_GLUniformNameCompare(const void* p_UniformName, const void* p_Uniform)
// {
// 	K15_GLUniform* glUniform = (K15_GLUniform*)(p_Uniform);
// 	const char* uniformName = *(const char**)(p_UniformName);
// 
// 	return strcmp(glUniform->name, uniformName);
// }
/*********************************************************************************/
const char* K15_GLGenerateGLSLHeaderCode(GLenum p_GLProgramType)
{
	const char* glslHeaderCode = 0;

	if (p_GLProgramType == GL_VERTEX_SHADER)
	{
		glslHeaderCode = "#version 330\n"
			"#define K15_Vector2 vec2\n"
			"#define K15_Vector3 vec3\n"
			"#define K15_Vector4 vec4\n"
			"#define K15_Matrix2 mat2\n"
			"#define K15_Matrix3 mat3\n"
			"#define K15_Matrix4 mat4\n"
			"#define K15_2DSampler sampler2D\n"
			"#define K15_2DTexture int\n"
			"#define sampleTex2D(s, t, uv) texture(s, vec2(uv.x, 1.0 - uv.y))\n"
			"out gl_PerVertex{\n"
			"vec4 gl_Position;\n"
			"float gl_PointSize;\n"
			"float gl_ClipDistance[];\n"
			"};\n";
	}
	else
	{
		glslHeaderCode = "#version 330\n"
			"#define K15_Vector2 vec2\n"
			"#define K15_Vector3 vec3\n"
			"#define K15_Vector4 vec4\n"
			"#define K15_Matrix2 mat2\n"
			"#define K15_Matrix3 mat3\n"
			"#define K15_Matrix4 mat4\n"
			"#define K15_2DSampler sampler2D\n"
			"#define K15_2DTexture int\n"
			"#define sampleTex2D(s, t, uv) texture(s, vec2(uv.x, 1.0 - uv.y))\n";
	}
	
	return glslHeaderCode;
}
/*********************************************************************************/
char* K15_GLGenerateGLSLWrapperCode(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderProgramType p_ProgramType, K15_ShaderProcessorContext* p_ShaderProcessorContext,
									   const char* p_ProcessedShaderCode, const char* p_MainFunctionIdentifier, K15_ShaderInformation* p_ShaderInformation)
{
 	char* complianceCode = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, size_kilobyte(10));
 	char* argumentList = (char*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, size_kilobyte(1));
 
	uint32 index = 0;
	uint32 argumentListIndex = 0;

	for (uint32 argIndex = 0;
		argIndex < p_ShaderInformation->numArguments;
		++argIndex)
	{
		K15_ShaderArgument* arg = &p_ShaderInformation->arguments[argIndex];

		//if (arg->typeID != K15_TYPE_SAMPLER_2D_ID)
		{
			const char* semanticVariableName = K15_GetSemanticVariableName(p_ShaderProcessorContext, arg->semanticID);
			const char* typeName = K15_GetTypeName(p_ShaderProcessorContext, arg->typeID);

			if (arg->typeID == K15_TYPE_TEXTURE_2D_ID)
			{
				argumentListIndex += sprintf(argumentList + argumentListIndex, "0");
			}
			else if (arg->semanticID == K15_SEMANTIC_DATA_ID)
			{
				index += sprintf(complianceCode + index, "uniform %s %s;\n", typeName, arg->name);
				argumentListIndex += sprintf(argumentList + argumentListIndex, "%s", arg->name);
			}
			else if (semanticVariableName)
			{
				index += sprintf(complianceCode + index, "uniform %s %s;\n", typeName, semanticVariableName);
				argumentListIndex += sprintf(argumentList + argumentListIndex, "%s", semanticVariableName);
			}
			else
			{
				index += sprintf(complianceCode + index, "%s %s %s;\n", arg->qualifier, typeName, arg->name);
				argumentListIndex += sprintf(argumentList + argumentListIndex, "%s", arg->name);
			}

			//not end?
			if (argIndex + 1 != p_ShaderInformation->numArguments)
			{
				argumentListIndex += sprintf(argumentList + argumentListIndex, ", ");
			}
		}
	}

	const char* glslBodyFormat = 0;

	if (p_ProgramType == K15_RENDER_PROGRAM_TYPE_VERTEX)
	{
		glslBodyFormat = "void main(void)\n"
			"{\n"
			" gl_Position = %s(%s);\n"
			"}";
	}
	else if (p_ProgramType == K15_RENDER_PROGRAM_TYPE_FRAGMENT)
	{
		glslBodyFormat = "void main(void)\n"
			"{\n"
			" gl_FragColor = %s(%s);\n"
			"}";
	}
		

	index += sprintf(complianceCode + index, glslBodyFormat, p_MainFunctionIdentifier, argumentList);

	K15_FreeFromMemoryAllocator(p_MemoryAllocator, argumentList);

	return complianceCode;
}
/*********************************************************************************/
result8 K15_GLCreateProgram(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderResourceHandle* p_RenderProgramHandle)
{
	result8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_RenderProgramType renderProgramType = p_RenderProgramDesc->type;

	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* memoryAllocator = &renderContext->memoryAllocator;

	uint32 hash = p_RenderProgramDesc->nameHash;

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
		if (!K15_FileExists(programFilePath))
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

	char* glslWrapperCode = 0;
	char* parsedProgramCode = K15_InternalGLPreprocessProgramCode(memoryAllocator, programFilePath, programCode);
	const char* mainFunctionIdentifier = K15_GetMainFunctionIdentifierForProgramType(renderProgramType);

	uint32 parserProgramCodeLength = (uint32)strlen(parsedProgramCode);
	
	char* compiledProgramCode = (char*)K15_AllocateFromMemoryAllocator(memoryAllocator, parserProgramCodeLength);

	K15_ShaderInformation shaderInformation = {};
	K15_ProcessShaderCode(p_RenderBackEnd->shaderProcessorContext, &shaderInformation, mainFunctionIdentifier,
		parsedProgramCode, parserProgramCodeLength,
		compiledProgramCode, parserProgramCodeLength);

	//check if the shader is valid
	K15_ShaderProcessResult* shaderProcessResult = &p_RenderBackEnd->shaderProcessorContext->lastProcessResult;
	if (!shaderProcessResult->valid)
	{
		char* error = K15_ConcatStringArray((const char**)shaderProcessResult->errors, shaderProcessResult->numErrors);
		K15_SetRenderContextError(renderContext, error, (uint32)strlen(error));
		result = K15_ERROR_RENDER_PROGRAM_COMPILATION;
	}

	K15_GLProgram program = {};
	GLuint glProgram = 0;

	//gl hack (manually create new sampler for each texture<->sampler combination
	//remove old sampler afterwards as they got replaced by the new sampler
	/*uint32 numArguments = shaderInformation.numArguments;
	uint32 oldNumArguments = shaderInformation.numArguments;
	for (uint32 textureSamplerIndex = 0;
		textureSamplerIndex < shaderInformation.numTextureSamplerDependencies;
		++textureSamplerIndex)
	{
		const char* textureName = shaderInformation.textureSamplerDependencies[textureSamplerIndex].texture;
		const char* samplerName = shaderInformation.textureSamplerDependencies[textureSamplerIndex].sampler;
		
		sprintf(shaderInformation.arguments[numArguments].name, "%s%s", samplerName, textureName);
		sprintf(shaderInformation.arguments[numArguments].qualifier, "%s", "in");
		shaderInformation.arguments[numArguments].typeID = K15_TYPE_SAMPLER_2D_ID;
		shaderInformation.arguments[numArguments].semanticID = K15_SEMANTIC_DATA_ID;
		shaderInformation.arguments[numArguments].semanticGroupID = K15_SEMANTIC_GROUP_ID_PER_MATERIAL;

		++shaderInformation.numArguments;
		++numArguments;
	}

	//remove old sampler
	for (uint32 argumentIndex = 0;
		argumentIndex < oldNumArguments;
		++argumentIndex)
	{
		K15_ShaderArgument* shaderArgument = &shaderInformation.arguments[argumentIndex];

		//remove if sampler
		if (shaderArgument->typeID == K15_TYPE_SAMPLER_2D_ID)
		{
			uint32 remainingElements = numArguments - argumentIndex;
			memmove(shaderInformation.arguments + argumentIndex, shaderInformation.arguments + argumentIndex + 1, sizeof(K15_ShaderArgument) * remainingElements);
			memset(shaderInformation.arguments + shaderInformation.numArguments, 0, sizeof(K15_ShaderArgument));

			--shaderInformation.numArguments;
			--oldNumArguments;
			--argumentIndex;
		}
	}*/

	if (result == K15_SUCCESS)
	{
		glslWrapperCode = K15_GLGenerateGLSLWrapperCode(memoryAllocator, renderProgramType, p_RenderBackEnd->shaderProcessorContext, compiledProgramCode, mainFunctionIdentifier, &shaderInformation);

		const char* glslProgramCode[] = {
			K15_GLGenerateGLSLHeaderCode(glProgramType),
			compiledProgramCode,
			glslWrapperCode
		};

		GLint linkStatus = GL_FALSE;

		GLuint glShaderHandle = GL_INVALID_VALUE;
		K15_OPENGL_CALL(glShaderHandle = kglCreateShader(glProgramType));

		if (!glShaderHandle)
		{
			result = K15_ERROR_RENDER_API;
		}
		else
		{
			K15_OPENGL_CALL(kglShaderSource(glShaderHandle, K15_ARRAY_COUNT(glslProgramCode), glslProgramCode, 0));
			K15_OPENGL_CALL(kglCompileShader(glShaderHandle));

			GLint compileStatus = GL_FALSE;

			K15_OPENGL_CALL(kglGetShaderiv(glShaderHandle, GL_COMPILE_STATUS, &compileStatus));

			if (compileStatus == GL_FALSE)
			{
				GLint infoLogLength = 0;
				K15_OPENGL_CALL(kglGetShaderiv(glShaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength));

				if (infoLogLength == 0)
				{
					result = K15_ERROR_RENDER_API;
				}
				else
				{
					char* infoLogBuffer = (char*)alloca(infoLogLength);
					K15_OPENGL_CALL(kglGetShaderInfoLog(glShaderHandle, infoLogLength, 0, infoLogBuffer));
					K15_SetRenderContextError(renderContext, infoLogBuffer, infoLogLength);

					result = K15_ERROR_RENDER_PROGRAM_COMPILATION;
				}
			}
			else
			{
				K15_OPENGL_CALL(glProgram = kglCreateProgram());
				K15_OPENGL_CALL(kglProgramParameteri(glProgram, GL_PROGRAM_SEPARABLE, GL_TRUE));
				K15_OPENGL_CALL(kglAttachShader(glProgram, glShaderHandle));
				K15_OPENGL_CALL(kglLinkProgram(glProgram));
				//K15_OPENGL_CALL(kglDetachShader(glProgram, glShaderHandle));
			}
		}


		if (result == K15_SUCCESS)
		{
			K15_OPENGL_CALL(kglGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus));

			program.glProgramHandle = glProgram;
			program.glProgramType = glProgramType;
			program.shaderInformations = shaderInformation;

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
					char* infoLogBuffer = (char*)alloca(infoLogLength);
					K15_OPENGL_CALL(kglGetProgramInfoLog(glProgram, infoLogLength, 0, infoLogBuffer));
					K15_SetRenderContextError(renderContext, infoLogBuffer, infoLogLength);

					result = K15_ERROR_RENDER_PROGRAM_COMPILATION;
				}
			}
		}
	}
	
	//after the shader got compiled, delete the program code
	K15_FreeFromMemoryAllocator(memoryAllocator, parsedProgramCode); 
	K15_FreeFromMemoryAllocator(memoryAllocator, compiledProgramCode); 
	K15_FreeFromMemoryAllocator(memoryAllocator, glslWrapperCode);

	//if everything went smooth so far, try to reflect the program
	if (result == K15_SUCCESS)
	{
		K15_InternGLReflectProgram(p_RenderBackEnd->renderContext, &program, p_RenderProgramDesc);
		*p_RenderProgramHandle = K15_InternalAddGLObject(glContext, &program, sizeof(program), hash, K15_GL_TYPE_PROGRAM);
	}

	/*if (glProgramType == GL_FRAGMENT_SHADER)
	{
		K15_OPENGL_CALL(kglBindFragDataLocation(glProgram, 0, "outColor0"));
	}*/

	return result;
}
/*********************************************************************************/
result8 K15_GLDeleteProgram(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderProgramHandle)
{
	result8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	K15_GLProgram* glProgram = (K15_GLProgram*)K15_InternalGetGLObjectData(glContext, *p_RenderProgramHandle, K15_GL_TYPE_PROGRAM);
	GLenum glProgramType = glProgram->glProgramType;
	K15_RenderProgramType programType = K15_ConvertGLProgramType(glProgramType);

	K15_OPENGL_CALL(kglDeleteProgram(glProgram->glProgramHandle));

	if (glContext->glBoundObjects.boundPrograms[programType] == glProgram)
	{
		glContext->glBoundObjects.boundPrograms[programType] = 0;
	}

	K15_InternalRemoveGLObject(glContext, p_RenderProgramHandle, K15_GL_TYPE_PROGRAM);

	return result;
}
/*********************************************************************************/
result8 K15_GLBindProgram(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_RenderProgramHandle)
{
	result8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	GLuint glProgramPipelineHandle = glContext->extensions.separate_shader_objects.programPipelineHandle;
	K15_GLProgram* glProgram = (K15_GLProgram*)K15_InternalGetGLObjectData(glContext, *p_RenderProgramHandle, K15_GL_TYPE_PROGRAM);
	GLenum glProgramType = glProgram->glProgramType;

	K15_RenderProgramType programType = K15_ConvertGLProgramType(glProgramType);
	if (glContext->glBoundObjects.boundPrograms[programType] == glProgram)
	{
		return K15_SUCCESS;
	}

	GLuint glProgramHandle = glProgram->glProgramHandle;
	GLbitfield glProgramTypeBit = K15_GLConvertProgramTypeBit(glProgramType);

	K15_OPENGL_CALL(kglUseProgramStages(glProgramPipelineHandle, glProgramTypeBit ,glProgramHandle));

	glContext->glBoundObjects.boundPrograms[programType] = glProgram;

	return result;
}
/*********************************************************************************/
result8 K15_GLUpdateDirtyUniforms(K15_RenderBackEnd* p_RenderBackEnd)
{
	result8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderBackEnd->shaderProcessorContext;

	K15_RenderUniformCache* uniformCache = &p_RenderBackEnd->uniformCache;

	//TEST ME
	for (uint32 dirtyUniformIndex = 0;
		dirtyUniformIndex < uniformCache->numDirtyUniforms;
		++dirtyUniformIndex)
	{
		if (uniformCache->dirtyAutoUniforms[dirtyUniformIndex])
		{
			K15_RenderUniformCacheEntry* cachedDirtyUniform = uniformCache->dirtyAutoUniforms[dirtyUniformIndex];
//			const char* cachedDirtyUniformName = cachedDirtyUniform->name;
			uint32 chachedDirtyUniformNameHash = cachedDirtyUniform->nameHash;

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
						uint32 glUniformNameHash = glUniform->nameHash;
						uint32 typeID = glUniform->typeID;

						if (glUniformNameHash == chachedDirtyUniformNameHash)
						{
							K15_InternalGLUpdateUniformFnc updateUniform = K15_InternalGLConvertToUpdateUniformFnc(typeID);
							K15_RenderUniformUpdateDesc updateUniformDesc = {};

							updateUniformDesc.typeID = typeID;
							updateUniformDesc.nameHash = glUniformNameHash;
							updateUniformDesc.sizeInBytes = K15_GetTypeSizeInBytesByTypeID(shaderProcessorContext, typeID);
							updateUniformDesc.data.rawData = cachedDirtyUniform->data;
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
result8 K15_GLUpdateUniform(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, K15_GLProgram* p_GLProgram)
{
	result8 result = K15_ERROR_RENDER_UNIFORM_NOT_FOUND;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	GLenum uniformType = K15_GLConvertUniformTypeToGLType(p_RenderUniformUpdateDesc->typeID);
	uint32 uniformCount = p_GLProgram->uniformCount;

	for (uint32 uniformIndex = 0;
		uniformIndex < uniformCount;
		++uniformIndex)
	{
		//GLchar* uniformName = glProgram->uniforms[uniformIndex].name;
		uint32 uniformNameHash = p_GLProgram->uniforms[uniformIndex].nameHash;

		if (uniformNameHash == p_RenderUniformUpdateDesc->nameHash)
		{
			switch (uniformType)
			{
			case GL_INT:
				{
					result = K15_InternalGLUpdateIntUniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_UNSIGNED_INT:
				{
					result = K15_InternalGLUpdateUIntUniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT:
				{
					result = K15_InternalGLUpdateFloatUniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_VEC2:
				{
					result = K15_InternalGLUpdateFloat2Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_VEC3:
				{
					result = K15_InternalGLUpdateFloat3Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_VEC4:
				{
					result = K15_InternalGLUpdateFloat4Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_MAT2:
				{
					result = K15_InternalGLUpdateFloat2x2Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_MAT3:
				{
					result = K15_InternalGLUpdateFloat3x3Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
					break;
				}

			case GL_FLOAT_MAT4:
				{
					result = K15_InternalGLUpdateFloat4x4Uniform(p_GLProgram, uniformIndex, p_RenderUniformUpdateDesc);
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
result8 K15_GLUpdateProgramDataUniforms(K15_RenderBackEnd* p_RenderBackEnd, K15_GLProgram* p_GLProgram, K15_RenderMaterialPassDesc* p_MaterialPassDesc)
{
	result8 result = K15_SUCCESS;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderBackEnd->shaderProcessorContext;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	for (uint32 argumentIndex = 0;
		argumentIndex < p_MaterialPassDesc->materialData.numElements;
		++argumentIndex)
	{
		K15_RenderMaterialDataDesc* materialDataDesc = 
			K15_GetRenderMaterialDataDescStretchBufferElementUnsafe(&p_MaterialPassDesc->materialData, argumentIndex);

		if (materialDataDesc->semanticID == K15_SEMANTIC_DATA_ID)
		{
			if (materialDataDesc->typeID != K15_TYPE_TEXTURE_2D_ID)
			{
				uint32 dataNameHash = materialDataDesc->nameHash;

				//check if the material data is representative in the shader
				K15_GLUniform* uniform = K15_InternalGLGetProgramUniformByNameHash(p_GLProgram, dataNameHash);
				bool8 foundInShader = uniform != 0;

				if (!foundInShader)
				{
					continue;
				}

				K15_RenderUniformUpdateDesc updateDesc = {};
				updateDesc.typeID = materialDataDesc->typeID;
				updateDesc.sizeInBytes = K15_GetTypeSizeInBytesByTypeID(shaderProcessorContext, materialDataDesc->typeID);
				updateDesc.nameHash = dataNameHash;

				if (materialDataDesc->typeID == K15_TYPE_SAMPLER_2D_ID)
				{
					K15_RenderResourceHandle* renderResourceHandle = (K15_RenderResourceHandle*)materialDataDesc->data;
					K15_GLSampler* glSampler = (K15_GLSampler*)K15_InternalGetGLObjectData(glContext, *renderResourceHandle, K15_GL_TYPE_SAMPLER);

					if (glSampler)
					{
						//check which textures should be bound for this sampler
						for (uint32 samTexDependencyIndex = 0;
							samTexDependencyIndex < p_GLProgram->shaderInformations.numTextureSamplerDependencies;
							++samTexDependencyIndex)
						{
							K15_TextureSamplerDependency* dependency = &p_GLProgram->shaderInformations.textureSamplerDependencies[samTexDependencyIndex];

							if (dependency->samplerNameHash == dataNameHash)
							{
								uint32 textureNameHash = dependency->textureNameHash;
								K15_RenderMaterialDataDesc* textureDataDesc = 
									K15_GetRenderMaterialDataDescStretchBufferElementConditional(&p_MaterialPassDesc->materialData, 
									K15_RenderMaterialDescHashCompare, &textureNameHash);

								if (textureDataDesc)
								{
									K15_RenderResourceHandle* textureRenderResourceHandle = (K15_RenderResourceHandle*)textureDataDesc->data;
									K15_GLTexture* glTexture = (K15_GLTexture*)K15_InternalGetGLObjectData(glContext, *textureRenderResourceHandle, K15_GL_TYPE_TEXTURE);

									uint32 uniformRegister = uniform->registerIndex;
									
									if (K15_GLBindTexture(p_RenderBackEnd, textureRenderResourceHandle, renderResourceHandle) == K15_SUCCESS)
									{
										uint32 textureSlot = glTexture->boundSlot;
										updateDesc.data.textureSlot = textureSlot;
									}
									else
									{
										continue;
									}
								}

								break;
							}
						}
					}
				}
				else
				{
					updateDesc.data.rawData = materialDataDesc->data;
				}

				result = K15_GLUpdateUniform(p_RenderBackEnd, &updateDesc, p_GLProgram);
			}
		}
	}

	return result;
}
/*********************************************************************************/