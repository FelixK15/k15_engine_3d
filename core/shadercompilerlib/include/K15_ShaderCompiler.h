#ifndef _K15_ShaderCompiler_h_
#define _K15_ShaderCompiler_h_

#include "K15_Common.h"

#define K15_MAX_SHADER_SEMANTICS 64
#define K15_MAX_SHADER_TYPES 64
#define K15_MAX_SHADER_ARGUMENTS 64
#define K15_MAX_SHADER_PROCESS_RESULT_ERRORS 128
#define K15_MAX_SHADER_PROCESS_RESULT_ERROR_LENGTH 256

#define K15_INVALID_ID 0xFFFFFFFF

struct K15_SemanticEntry 
{
	const char* variableName;
	const char* semanticName;
	uint32 semanticID;
	uint32 semanticGroupID;
};

struct K15_TypeEntry
{
	const char* typeName;
	uint32 typeID;
	uint32 typeSizeInBytes;
};

struct K15_SemanticTable
{
	uint32 numEntries;
	K15_SemanticEntry entries[K15_MAX_SHADER_SEMANTICS];
};

struct K15_TypeTable
{
	uint32 numEntries;
	K15_TypeEntry entries[K15_MAX_SHADER_TYPES];
};

struct K15_ShaderProcessResult
{
	bool8 valid;

	uint32 numErrors;
	char errors[K15_MAX_SHADER_PROCESS_RESULT_ERRORS][K15_MAX_SHADER_PROCESS_RESULT_ERROR_LENGTH];
};

struct K15_ShaderProcessorContext
{
	K15_TypeTable typeTable;
	K15_SemanticTable semanticTable;

	K15_ShaderProcessResult lastProcessResult;
};

struct K15_ShaderArgument
{
	char name[64];
	char qualifier[10];
	uint32 semanticID;
	uint32 semanticGroupID;
	uint32 typeID;
};

struct K15_ShaderInformation
{
	K15_ShaderArgument arguments[K15_MAX_SHADER_ARGUMENTS];
	uint32 numArguments;
};

K15_ShaderArgument* K15_GetShaderArgumentByName(K15_ShaderInformation* p_ShaderInformation, const char* p_ArgumentName);

uint32 K15_GetTypeSizeInBytesByTypeID(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_TypeID);

const char* K15_GetSemanticVariableName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_SemanticID);
const char* K15_GetSemanticName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_SemanticID);
const char* K15_GetTypeName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_TypeID);

uint32 K15_GetSemanticGroupID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_Semantic);
uint32 K15_GetSemanticID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_Semantic);
uint32 K15_GetTypeID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_TypeName);
uint32 K15_GetTypeSizeInBytesByTypeName(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_TypeName);

void K15_ProcessShaderCode(K15_ShaderProcessorContext* p_ShaderProcessorContext, K15_ShaderInformation* p_ShaderInformation,
						   const char* p_MainFunctionIdentifier, 
						   const char* p_InputShaderCode, uint32 p_InputShaderCodeLength, 
						   char* p_OutputShaderCodeBuffer, uint32 p_OutputShaderCodeBufferSize);

#endif //_K15_ShaderCompiler_h_