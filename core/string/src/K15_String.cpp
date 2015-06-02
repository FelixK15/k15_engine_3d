#include "K15_String.h"

#include <cassert>
#include <cstring>
#include <cctype>
#include <memory>

/*********************************************************************************/
unsigned int K15_GetLines(const char* p_String, char** p_Lines)
{
	size_t stringLength = 0;
	char* stringBuffer = (char*)p_String; //hacky and dangerous. will be overridden if p_lines is valid.
	
	if (p_Lines)
	{
		stringLength = strlen(p_String);

		stringBuffer = (char*)malloc(stringLength + 1);
		memcpy(stringBuffer, p_String, stringLength);

		stringBuffer[stringLength] = 0;
	}

	return K15_GetLinesInplace(stringBuffer, p_Lines);
}
/*********************************************************************************/
unsigned int K15_GetLinesInplace(char* p_String, char** p_Lines)
{
	size_t stringLength = strlen(p_String);
	size_t stringIndex = 0;
	unsigned int lineCount = 0;
	unsigned int lineIndex = 0;
	unsigned int lineBreakIndex = 0;

	//one line for counting the loops
	while (stringIndex != stringLength)
	{
		char stringChar = p_String[stringIndex];
		if (stringChar == '\n')
		{
			if (p_Lines)
			{
				p_String[stringIndex] = 0;
				p_Lines[lineCount] = p_String + lineBreakIndex;

				lineBreakIndex = (unsigned int)stringIndex + 1;
			}

			++lineCount;
		}
		else if (stringChar == '\r')
		{
			if (p_Lines)
			{
				p_String[stringIndex] = 0;
			}
		}

		++stringIndex;
	}

	return lineCount;
}
/*********************************************************************************/
char* K15_CopyString(const char* p_String, unsigned int p_StringLength)
{
	assert(p_String && "Source string is NULL.");

	if (p_StringLength == 0)
	{
		p_StringLength = (unsigned int)strlen(p_String);
	} 

	char* buffer = (char*)malloc(p_StringLength);

	return K15_CopyStringIntoBuffer(p_String, buffer, p_StringLength);
}
/*********************************************************************************/
char* K15_CopyStringIntoBuffer(const char* p_String, char* p_Buffer, unsigned int p_StringLength)
{
	assert(p_String && "Source string is NULL.");
	assert(p_Buffer && "Destination string is NULL.");

	if (p_StringLength == 0)
	{
		p_StringLength = (unsigned int)strlen(p_String);
	}

	memcpy(p_Buffer, p_String, p_StringLength);
	p_Buffer[p_StringLength] = 0;

	return p_Buffer;
}
/*********************************************************************************/