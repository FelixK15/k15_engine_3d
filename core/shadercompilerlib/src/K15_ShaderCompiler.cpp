#include "K15_ShaderCompiler.h"
#include "K15_String.h"

#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum K15_ShaderArgumentParseState : uint8
{
	K15_PARSER_STATE_TYPENAME = 0,
	K15_PARSER_STATE_VARIABLENAME,
	K15_PARSER_STATE_QUALIFIER,
	K15_PARSER_STATE_SEMANTIC
};

/*********************************************************************************/
intern void K15_InternalAddProcessResultError(K15_ShaderProcessResult* p_ProcessResult, char* p_Error, ...)
{
	uint32 oldErrorCount = p_ProcessResult->numErrors;
	uint32 newErrorCount = oldErrorCount + 1;

	va_list list;
	va_start(list, p_Error);

	char* errorBuffer = p_ProcessResult->errors[oldErrorCount];
	vsprintf(errorBuffer, p_Error, list);

	va_end(list);
	
	p_ProcessResult->numErrors = newErrorCount;
}
/*********************************************************************************/
intern K15_SemanticEntry* K15_InternalGetSemanticEntryUsingID(K15_SemanticTable* p_SemanticTable, uint32 p_SemanticID)
{
	K15_SemanticEntry* entry = 0;

	for (uint32 semanticIndex = 0;
		semanticIndex < p_SemanticTable->numEntries;
		++semanticIndex)
	{
		if (p_SemanticTable->entries[semanticIndex].semanticID == p_SemanticID)
		{
			entry = &p_SemanticTable->entries[semanticIndex];
			break;
		}
	}

	return entry;
}
/*********************************************************************************/
intern K15_SemanticEntry* K15_InternalGetSemanticEntryUsingSemanticName(K15_SemanticTable* p_SemanticTable, const char* p_SymanticName)
{
	K15_SemanticEntry* entry = 0;

	for (uint32 semanticIndex = 0;
		semanticIndex < p_SemanticTable->numEntries;
		++semanticIndex)
	{
		if (strcmp(p_SymanticName, p_SemanticTable->entries[semanticIndex].semanticName) == 0)
		{
			entry = &p_SemanticTable->entries[semanticIndex];
			break;
		}
	}

	return entry;
}
/*********************************************************************************/
intern K15_TypeEntry* K15_InternalGetTypeEntryUsingID(K15_TypeTable* p_TypeTable, uint32 p_TypeID)
{
	K15_TypeEntry* entry = 0;

	for (uint32 semanticIndex = 0;
		semanticIndex < p_TypeTable->numEntries;
		++semanticIndex)
	{
		if (p_TypeTable->entries[semanticIndex].typeID == p_TypeID)
		{
			entry = &p_TypeTable->entries[semanticIndex];
			break;
		}
	}

	return entry;
}
/*********************************************************************************/
intern K15_TypeEntry* K15_InternalGetTypeEntryUsingTypeName(K15_TypeTable* p_TypeTable, const char* p_TypeName)
{
	K15_TypeEntry* entry = 0;

	for (uint32 typeIndex = 0;
		typeIndex < p_TypeTable->numEntries;
		++typeIndex)
	{
		if (strcmp(p_TypeName, p_TypeTable->entries[typeIndex].typeName) == 0)
		{
			entry = &p_TypeTable->entries[typeIndex];
			break;
		}
	}

	return entry;
}
/*********************************************************************************/
K15_ShaderArgument* K15_GetShaderArgumentByName(K15_ShaderInformation* p_ShaderInformation, const char* p_ArgumentName)
{
	for (uint32 argIndex = 0;
		argIndex < p_ShaderInformation->numArguments;
		++argIndex)
	{
		K15_ShaderArgument* argument = &p_ShaderInformation->arguments[argIndex];

		if (strcmp(argument->name, p_ArgumentName) == 0)
		{
			return argument;
		}
	}

	return 0;
}
/*********************************************************************************/
uint32 K15_GetTypeSizeInBytesByTypeID(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_TypeID)
{
	K15_TypeTable* typeTable = &p_ShaderProcessorContext->typeTable;
	K15_TypeEntry* entry = K15_InternalGetTypeEntryUsingID(typeTable, p_TypeID);

	return entry ? entry->typeSizeInBytes : 0;
}
/*********************************************************************************/
const char* K15_GetSemanticVariableName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_SemanticID)
{
	K15_SemanticTable* semanticTable = &p_ShaderProcessorContext->semanticTable;
	K15_SemanticEntry* entry = K15_InternalGetSemanticEntryUsingID(semanticTable, p_SemanticID);

	return entry ? entry->variableName : 0;
}
/*********************************************************************************/
const char* K15_GetSemanticName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_SemanticID)
{
	K15_SemanticTable* semanticTable = &p_ShaderProcessorContext->semanticTable;
	K15_SemanticEntry* entry = K15_InternalGetSemanticEntryUsingID(semanticTable, p_SemanticID);

	return entry ? entry->semanticName : 0;
}
/*********************************************************************************/
const char* K15_GetTypeName(K15_ShaderProcessorContext* p_ShaderProcessorContext, uint32 p_TypeID)
{
	K15_TypeTable* typeTable = &p_ShaderProcessorContext->typeTable;
	K15_TypeEntry* entry = K15_InternalGetTypeEntryUsingID(typeTable, p_TypeID);

	return entry ? entry->typeName : 0;
}
/*********************************************************************************/
uint32 K15_GetSemanticGroupID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_Semantic)
{
	K15_SemanticTable* semanticTable = &p_ShaderProcessorContext->semanticTable;
	K15_SemanticEntry* entry = K15_InternalGetSemanticEntryUsingSemanticName(semanticTable, p_Semantic);

	return entry ? entry->semanticGroupID : K15_INVALID_ID;
}
/*********************************************************************************/
uint32 K15_GetSemanticID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_Semantic)
{
	K15_SemanticTable* semanticTable = &p_ShaderProcessorContext->semanticTable;
	K15_SemanticEntry* entry = K15_InternalGetSemanticEntryUsingSemanticName(semanticTable, p_Semantic);

	return entry ? entry->semanticID : K15_INVALID_ID;
}
/*********************************************************************************/
uint32 K15_GetTypeID(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_TypeName)
{
	K15_TypeTable* typeTable = &p_ShaderProcessorContext->typeTable;
	K15_TypeEntry* entry = K15_InternalGetTypeEntryUsingTypeName(typeTable, p_TypeName);

	return entry ? entry->typeID : K15_INVALID_ID;
}
/*********************************************************************************/
uint32 K15_GetTypeSizeInBytes(K15_ShaderProcessorContext* p_ShaderProcessorContext, const char* p_TypeName)
{
	K15_TypeTable* typeTable = &p_ShaderProcessorContext->typeTable;
	K15_TypeEntry* entry = K15_InternalGetTypeEntryUsingTypeName(typeTable, p_TypeName);

	return entry ? entry->typeSizeInBytes : 0;
}
/*********************************************************************************/
void K15_ProcessShaderCode(K15_ShaderProcessorContext* p_ShaderProcessorContext, K15_ShaderInformation* p_ShaderInformation,
						   const char* p_MainFunctionIdentifier, 
						   const char* p_InputShaderCode, uint32 p_InputShaderCodeLength, 
						   char* p_OutputShaderCodeBuffer, uint32 p_OutputShaderCodeBufferSize)
{
	K15_ASSERT_TEXT(p_ShaderProcessorContext, "Shader processor context is NULL.");
	K15_ASSERT_TEXT(p_MainFunctionIdentifier, "Main function identifier is NULL.");
	K15_ASSERT_TEXT(p_InputShaderCode, "Shader code is NULL.");
	K15_ASSERT_TEXT(p_InputShaderCodeLength, "Shader code length is 0.");
	K15_ASSERT_TEXT(p_OutputShaderCodeBuffer, "Output buffer is NULL.");
	K15_ASSERT_TEXT(p_OutputShaderCodeBufferSize, "Output buffer size is 0.");
	K15_ASSERT_TEXT(p_OutputShaderCodeBufferSize >= p_InputShaderCodeLength , "Output buffer size is too small. (Should be at least as big as the input shader length.");

	//clear previous errors
	K15_ShaderProcessResult* processResult = &p_ShaderProcessorContext->lastProcessResult;
	processResult->numErrors = 0;
	
	bool8 shaderIsValid = K15_TRUE;
	bool8 eatChar = K15_FALSE;
	bool8 processedMainArguments = K15_FALSE;
	bool8 seekNextAlpha = K15_TRUE;

	uint32 outputCharIndex = 0;
	uint32 argumentIndex = 0;
	uint32 tokenIndex = 0;
	uint32 lineCount = 0;
	uint32 lineChar = 0;

	K15_ShaderArgumentParseState parserState = K15_PARSER_STATE_QUALIFIER;

	const char* inputShaderMainFunction = strstr(p_InputShaderCode, p_MainFunctionIdentifier);
	char token[64] = {};

	//check if the main function is present
	if (!inputShaderMainFunction)
	{
		K15_InternalAddProcessResultError(processResult, "Could not find main function '%s' in shader.", p_MainFunctionIdentifier);
		shaderIsValid = K15_FALSE;
		goto set_process_result;
	}

	uint64 mainFunctionArgumentOffset = ((uint64)inputShaderMainFunction - (uint64)p_InputShaderCode) + (uint64)strlen(p_MainFunctionIdentifier);

	//iterate input shader char by char
	for (uint32 charIndex = 0;
		charIndex < p_InputShaderCodeLength;
		++charIndex, ++lineChar)
	{
		char shaderChar = p_InputShaderCode[charIndex];

		if (charIndex > mainFunctionArgumentOffset &&
			!processedMainArguments)
		{
			if (isalnum(shaderChar) ||
				shaderChar == '_')
			{
				token[tokenIndex++] = shaderChar;
				token[tokenIndex] = 0;

				seekNextAlpha = K15_FALSE;
			}
			else if(isspace(shaderChar))
			{
				if(!seekNextAlpha)
				{
					seekNextAlpha = K15_TRUE;

					if (parserState == K15_PARSER_STATE_QUALIFIER)
					{
						parserState = K15_PARSER_STATE_TYPENAME;
						K15_CopyStringIntoBuffer(token, p_ShaderInformation->arguments[argumentIndex].qualifier);
						tokenIndex = 0;
					}
					else if (parserState == K15_PARSER_STATE_TYPENAME)
					{
						parserState = K15_PARSER_STATE_VARIABLENAME;
						tokenIndex = 0;

						uint32 typeID = K15_GetTypeID(p_ShaderProcessorContext, token);

						if (typeID == 0xFFFFFFFF)
						{
							K15_InternalAddProcessResultError(processResult, "Could not resolve typename '%s' (missing in the shader context type table).", token);
							shaderIsValid = K15_FALSE;
							goto set_process_result;
						}
						else
						{
							p_ShaderInformation->arguments[argumentIndex].typeID = K15_GetTypeID(p_ShaderProcessorContext, token);
						}
					}
					else if (parserState == K15_PARSER_STATE_VARIABLENAME)
					{
						parserState = K15_PARSER_STATE_SEMANTIC;
						K15_CopyStringIntoBuffer(token, p_ShaderInformation->arguments[argumentIndex].name);
						tokenIndex = 0;
					}
				}
			}
			else if (shaderChar == ':') //semantic
			{
				eatChar = K15_TRUE; //don't output semantic
				seekNextAlpha = K15_TRUE;
			}
			else if (shaderChar == ',' ||
					 shaderChar == ')')
			{
				if (parserState == K15_PARSER_STATE_SEMANTIC)
				{
					uint32 semanticID = K15_GetSemanticID(p_ShaderProcessorContext, token);
					uint32 semanticGroupID = K15_GetSemanticGroupID(p_ShaderProcessorContext, token);

					if (semanticID == K15_INVALID_ID)
					{
						K15_InternalAddProcessResultError(processResult, "Could not resolve semantic '%s' (missing in the shader context semantic table).", token);
						shaderIsValid = K15_FALSE;
						goto set_process_result;
					}
					else
					{
						//overwrite normal variable name if we've got a specific semantic variable instead
						const char* semanticVariableName = K15_GetSemanticVariableName(p_ShaderProcessorContext, semanticID);
						if (semanticVariableName)
						{
							K15_CopyStringIntoBuffer(semanticVariableName, p_ShaderInformation->arguments[argumentIndex].name);
						}

						p_ShaderInformation->arguments[argumentIndex].semanticID = semanticID;
						p_ShaderInformation->arguments[argumentIndex].semanticGroupID = semanticGroupID;
					}

					++argumentIndex;
					++p_ShaderInformation->numArguments;
					parserState = K15_PARSER_STATE_QUALIFIER;
				}

				tokenIndex = 0;
				eatChar = K15_FALSE;
				seekNextAlpha = K15_TRUE;

				if (shaderChar == ')')
				{
					processedMainArguments = K15_TRUE;
				}
			}
			else if (shaderChar == '\n')
			{
				lineCount++;
				lineChar = 0;
			}
			else
			{
				K15_InternalAddProcessResultError(processResult, "Unexpected symbol '%c' in line %d | character %d.", shaderChar, lineCount, lineChar);
				shaderIsValid = K15_FALSE;
			}
		}

		if (!eatChar)
		{
			p_OutputShaderCodeBuffer[outputCharIndex++] = shaderChar;
		}
	}

set_process_result:
	processResult->valid = shaderIsValid;
	p_OutputShaderCodeBuffer[outputCharIndex] = 0;

	return;
}
/*********************************************************************************/