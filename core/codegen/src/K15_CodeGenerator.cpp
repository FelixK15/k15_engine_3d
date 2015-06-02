#include "K15_CodeGenerator.h"
#include "K15_ArgumentParser.h"
#include "K15_FileSystem.h"
#include "K15_System.h"
#include "K15_Logging.h"


#include <cstring>
#include <memory>

/*********************************************************************************/
static int K15_GetNextDollar(const char* p_String)
{
	int counter = 0;
	int stringIndex = 0;
	char currentChar = p_String[stringIndex];

	while (currentChar != 0)
	{
		if (currentChar != '$')
		{
			++counter;
		}
		else
		{
			break;
		}

		currentChar = p_String[++stringIndex];
	}

	return counter;
}
/*********************************************************************************/
const char* K15_GetTemplateValue(K15_ArgumentParserContext* p_ParserContext, const char* p_TemplateName)
{
	const char* value = 0;
	K15_TemplateDefine* currentTemplateDefine = 0;

	unsigned int numTemplateDefines = p_ParserContext->numTemplateDefines;

	for (unsigned int templateIndex = 0;
		templateIndex < numTemplateDefines;
		++templateIndex)
	{
		currentTemplateDefine = &p_ParserContext->templateDefines[templateIndex];

		if (strcmp(currentTemplateDefine->name, p_TemplateName) == 0)
		{
			value = currentTemplateDefine->define;
			break;
		}
	}

	return value;
}
/*********************************************************************************/
// const char* K15_ResolveTemplateName(K15_ArgumentParserContext* p_Context, const char* p_TemplateName)
// {
// 	int nameIndex = 0;
// 	int outputIndex = 0;
// 	char* outputName = (char*)malloc(256);
// 	char* currentTemplateName = 0;
// 	char currentChar = p_TemplateName[nameIndex];
// 
// 	while (currentChar != 0)
// 	{
// 		if (currentChar == '$')
// 		{
// 			if (currentTemplateName)
// 			{
// 				currentTemplateName = 0;
// 			}
// 			else
// 			{
// 				int offset = nameIndex + 1;
// 				int templateNameSize = K15_GetNextDollar(p_TemplateName + offset);
// 
// 				currentTemplateName = (char*)malloc(templateNameSize + 1);
// 
// 				memcpy(currentTemplateName, p_TemplateName + offset, templateNameSize);
// 				currentTemplateName[templateNameSize] = 0;
// 
// 				const char* currentTemplateValue = K15_GetTemplateValue(p_Context, currentTemplateName);
// 
// 				if (currentTemplateValue)
// 				{
// 					//put template value
// 					size_t templateValueLength = strlen(currentTemplateValue);
// 					memcpy(outputName + outputIndex, currentTemplateValue, templateValueLength);
// 
// 					outputIndex += templateValueLength;
// 					nameIndex += templateNameSize;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			outputName[outputIndex++] = currentChar;
// 		}
// 
// 		currentChar = p_TemplateName[++nameIndex];
// 	}
// 
// 	outputName[outputIndex] = 0;
// 
// 	return outputName;
// }
/*********************************************************************************/
uint8 K15_FilterChar(char p_Character)
{
	return p_Character == '\r';
}
/*********************************************************************************/
void K15_GenerateCode(K15_ArgumentParserContext* p_ParserContext)
{
	//const char* inputFile = K15_ResolveTemplateName(p_ParserContext, p_ParserContext->inputFile);
	const char* inputFile = p_ParserContext->inputFile;
	const char* outputFile = p_ParserContext->outputFile;

	char* inputPathBuffer = (char*)alloca(512);
	char* outputPathBuffer = (char*)malloc(512);
	sprintf(inputPathBuffer, "%s/%s", p_ParserContext->workingDir, inputFile);
	sprintf(outputPathBuffer, "%s/%s", p_ParserContext->workingDir, outputFile);

	K15_LOG_VERBOSE(p_ParserContext, "Trying to read '%s'...", inputPathBuffer);

	uint32 inputFileSize = K15_GetFileSize(inputPathBuffer);
	char* inputFileContent = (char*)malloc(inputFileSize + 1);
	K15_CopyWholeFileContentIntoBuffer(inputPathBuffer, (byte*)inputFileContent);
	inputFileContent[inputFileSize] = 0;

	if (!inputFileContent)
	{
		char* errorMessage = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("Could not open file '%s' (%s).", inputPathBuffer, errorMessage);
		K15_OS_FREE(errorMessage);

		return;
	}

	int contentIndex = 0;
	
	char* currentTemplateName = 0;
	char contentChar = inputFileContent[contentIndex];

	K15_LOG_VERBOSE(p_ParserContext, "Trying to open '%s' for writing...", outputPathBuffer);

	FILE* outputFileHandle = fopen(outputPathBuffer, "w");

	if (!outputFileHandle)
	{
		char* errorMessage = K15_GetSystemErrorMessage();
		K15_LOG_ERROR_MESSAGE("Could not open file '%s' for writing (%s).", outputPathBuffer, errorMessage);
		K15_OS_FREE(errorMessage);
		K15_OS_FREE(inputFileContent);

		return;
	}

	uint32 lineNumber = 0;

	//iterate input file content char by char
	while(contentChar != 0)
	{
		if (contentChar == '$')
		{
			if (currentTemplateName)
			{
				currentTemplateName = 0;
			}
			else
			{
				int offset = contentIndex + 1;
				int templateNameSize = K15_GetNextDollar(inputFileContent + offset);

				currentTemplateName = (char*)malloc(templateNameSize + 1);

				memcpy(currentTemplateName, inputFileContent + offset, templateNameSize);
				currentTemplateName[templateNameSize] = 0;

				K15_LOG_VERBOSE(p_ParserContext, "Found template define '%s' in file '%s' at line %d.", currentTemplateName, inputFile, lineNumber);

				const char* currentTemplateValue = K15_GetTemplateValue(p_ParserContext, currentTemplateName);

				if (currentTemplateValue)
				{
					//put template value
					fputs(currentTemplateValue, outputFileHandle);
					free(currentTemplateName);

					contentIndex += templateNameSize;
				}
			}
		}
		else if(!K15_FilterChar(contentChar))
		{
			//keep track of line number.
			if (contentChar == '\n')
			{
				++lineNumber;
			}

			//put char by char
			fputc(contentChar, outputFileHandle);
		}

		contentChar = inputFileContent[++contentIndex];
	}

	fclose(outputFileHandle);
}
/*********************************************************************************/