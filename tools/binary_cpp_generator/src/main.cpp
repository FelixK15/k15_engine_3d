#include "K15_Common.h"
#include "K15_String.h"
#include "K15_FileSystem.h"
#include "K15_Logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

enum K15_ArgumentParserState
{
	STATE_NORMAL = 0,
	STATE_INPUT,
	STATE_OUTPUT
};

struct K15_ArgumentInformation
{
	const char* inputFile;
	const char* outputFile;
};

/*********************************************************************************/
K15_ArgumentParserState K15_InternalHandleState(const char* p_Argument, K15_ArgumentParserState p_State, K15_ArgumentInformation* p_ArgumentInformation)
{
	K15_ArgumentParserState returnState = STATE_NORMAL;
	
	if (p_State == STATE_NORMAL)
	{
		if (strcmp(p_Argument, "-i") == 0)
		{
			returnState = STATE_INPUT;
		}

		if (strcmp(p_Argument, "-o") == 0)
		{
			returnState = STATE_OUTPUT;
		}
	}
	else if (p_State == STATE_INPUT)
	{
		p_ArgumentInformation->inputFile = p_Argument;
		returnState = STATE_NORMAL;
	}
	else if (p_State == STATE_OUTPUT)
	{
		p_ArgumentInformation->outputFile = p_Argument;
		returnState = STATE_NORMAL;
	}

	return returnState;
}
/*********************************************************************************/
bool8 K15_InternalValidateArgumentInformation(K15_ArgumentInformation* p_ArgumentInformation)
{
	const char* outputFile = p_ArgumentInformation->outputFile;
	const char* inputFile = p_ArgumentInformation->inputFile;
	bool8 valid = K15_TRUE;

	if (!K15_FileExists(inputFile))
	{
		valid = K15_FALSE;
		K15_LOG_ERROR_MESSAGE("Could not find file '%s'.", outputFile);
	}

	return valid;
}
/*********************************************************************************/
bool8 K15_InternalWriteBinaryToCPP(K15_ArgumentInformation* p_ArgumentInformation)
{
	const char* inputFilePath = p_ArgumentInformation->inputFile;
	const char* outputFilePath = p_ArgumentInformation->outputFile;

	FILE* outputFile = fopen(outputFilePath, "w");

	if (!outputFile)
	{
		K15_LOG_ERROR_MESSAGE("Could not access file '%s'.", outputFilePath);
		return K15_FALSE;
	}

	char* tempBuffer = (char*)alloca(20);
	sprintf(tempBuffer, "0x");

	char* outputFileName = K15_GetFileNameWithoutExtension(outputFilePath);
	K15_Replace(outputFileName, ";: ", '_');

	uint32 fileSize = K15_GetFileSize(inputFilePath);
	byte* fileContent = K15_GetWholeFileContent(inputFilePath);

	fwrite("byte ", 5, 1, outputFile);	
	fwrite(outputFileName, strlen(outputFileName), 1, outputFile);
	fwrite("[] = {\n\t", 8, 1, outputFile);
	
	for (uint32 fileContentIndex = 0;
		fileContentIndex < fileSize;
		++fileContentIndex)
	{
		itoa(fileContent[fileContentIndex], tempBuffer+2, 16);

		if (strlen(tempBuffer) == 3)
		{
			tempBuffer[3] = tempBuffer[2];
			tempBuffer[2] = '0';
		}

		fwrite(tempBuffer, 4, 1, outputFile);

		if (fileContentIndex + 1 < fileSize)
		{
			fwrite(", ", 2, 1, outputFile);
		
			if ((fileContentIndex+1) % 15 == 0)
			{
				fwrite("\n\t", 2, 1, outputFile);
			}
		}
	}

	itoa((int)fileSize, tempBuffer, 10);

	fwrite("\n};\n\n", 5, 1, outputFile);
	fwrite("unsigned int ", 13, 1, outputFile);
	fwrite(outputFileName, strlen(outputFileName), 1, outputFile);
	fwrite("_size = ", 8, 1, outputFile);
	fwrite(tempBuffer, strlen(tempBuffer), 1, outputFile);
	fwrite(";", 1, 1, outputFile);

	fflush(outputFile);
	fclose(outputFile);

	return K15_TRUE;
}
/*********************************************************************************/
int main(int argc, const char** argv)
{
	K15_ArgumentParserState state = STATE_NORMAL;
	K15_ArgumentInformation argumentInformation = {};

	for (int argIndex = 1;
		argIndex < argc;
		++argIndex)
	{
		const char* argument = argv[argIndex];
		state = K15_InternalHandleState(argument, state, &argumentInformation);
	}

	if (!K15_InternalValidateArgumentInformation(&argumentInformation))
	{
		return -1;
	}

	K15_InternalWriteBinaryToCPP(&argumentInformation);

	return 0;
}
/*********************************************************************************/