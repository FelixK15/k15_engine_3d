#include "K15_String.h"

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS

#include <cassert>
#include <cstring>
#include <cctype>
#include <memory>
#include <cstdarg>

/*********************************************************************************/
static char** K15_InternalVCreateStringArrayIntoBuffer(char** p_Buffer, unsigned int p_NumStrings, va_list p_ArgumentList)
{
	assert(p_NumStrings);
	assert(p_Buffer);

	char** stringArray = p_Buffer;

	for (unsigned int stringIndex = 0;
		stringIndex < p_NumStrings;
		++stringIndex)
	{
		char* currentString = va_arg(p_ArgumentList, char*);
		stringArray[stringIndex] = K15_CopyString(currentString);
	}

	return stringArray;
}
/*********************************************************************************/



/*********************************************************************************/
unsigned int K15_GetLines(const char* p_String, char** p_Lines)
{
	unsigned int stringLength = 0;
	char* stringBuffer = (char*)p_String; //hacky and dangerous. will be overridden if p_lines is valid. As long as we don't change this buffer, we'll be fine
	
	if (p_Lines)
	{
		//Copy string so we can modify it.
		stringBuffer = K15_CopyString(p_String);
	}

	return K15_GetLinesInplace(stringBuffer, p_Lines);
}
/*********************************************************************************/
unsigned int K15_GetLinesInplace(char* p_String, char** p_Lines)
{
	unsigned int stringLength= (unsigned int)strlen(p_String);
	unsigned int stringIndex = 0;
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
			}

			lineBreakIndex = (unsigned int)stringIndex + 1;

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

	if (stringIndex != lineBreakIndex)
	{
		if (p_Lines)
		{
			p_Lines[lineCount] = p_String + lineBreakIndex;
		}

		++lineCount;
	}

	return lineCount;
}
/*********************************************************************************/
char* K15_CopyString(const char* p_String, unsigned int p_StringLength)
{
	assert(p_String && "Source string is NULL.");

	if (p_StringLength == 0)
	{
		p_StringLength= (unsigned int)strlen(p_String);
	} 

	char* buffer = (char*)malloc(p_StringLength + 1);

	return K15_CopyStringIntoBuffer(p_String, buffer, p_StringLength);
}
/*********************************************************************************/
char* K15_CopyStringIntoBuffer(const char* p_String, char* p_Buffer, unsigned int p_StringLength)
{
	assert(p_String && "Source string is NULL.");
	assert(p_Buffer && "Destination string is NULL.");

	if (p_StringLength == 0)
	{
		p_StringLength= (unsigned int)strlen(p_String);
	}

	memcpy(p_Buffer, p_String, p_StringLength);
	p_Buffer[p_StringLength] = 0;

	return p_Buffer;
}
/*********************************************************************************/
int K15_IsSubstring(const char* p_String, const char* p_Needle)
{
	assert(p_String);
	assert(p_Needle);

	int stringLength= (unsigned int)strlen(p_String);
	int needleLength= (unsigned int)strlen(p_Needle);

	if (stringLength == 0 || needleLength == 0)
	{
		return 0;
	}

	int needleIndex = 0;
	int stringIndex = 0;
	do 
	{
		if (p_String[stringIndex] != p_Needle[needleIndex++])
		{
			needleIndex = 0;
		}

		if (needleIndex == needleLength)
		{
			return 1;
		}

		if (stringLength - stringIndex < needleLength && needleIndex == 0)
		{
			return 0;
		}

	} while (++stringIndex != stringLength);

	return 0;
}
/*********************************************************************************/
int K15_IsSubstringR(const char* p_String, const char* p_Needle)
{
	assert(p_String);
	assert(p_Needle);

	int stringLength= (unsigned int)strlen(p_String) - 1;
	int needleLength= (unsigned int)strlen(p_Needle) - 1;

	if (stringLength == 0 || needleLength == 0)
	{
		return 0;
	}

	int needleIndex = needleLength;
	int stringIndex = stringLength;
	do 
	{
		if (p_String[stringIndex] != p_Needle[needleIndex--])
		{
			needleIndex = needleLength;
		}

		if (needleIndex < 0)
		{
			return 1;
		}

		if (needleLength < stringIndex && needleIndex == needleLength)
		{
			return 0;
		}

	} while (--stringIndex >= 0);

	return 0;
}
/*********************************************************************************/
char* K15_ConcatStrings(const char* p_String1, const char* p_String2)
{
	assert(p_String1 && p_String2);

	unsigned int stringLenght1= (unsigned int)strlen(p_String1);
	unsigned int stringLenght2= (unsigned int)strlen(p_String2);

	unsigned int concatStringLength = stringLenght1 + stringLenght2;

	char* concatString = (char*)malloc(concatStringLength + 1);

	return K15_ConcatStringsIntoBuffer(p_String1, p_String2, concatString);
}
/*********************************************************************************/
char* K15_ConcatStringsIntoBuffer(const char* p_String1, const char* p_String2, char* p_Buffer)
{
	unsigned int stringLenght1= (unsigned int)strlen(p_String1);
	unsigned int stringLenght2= (unsigned int)strlen(p_String2);

	unsigned int concatStringLength = stringLenght1 + stringLenght2;
	
	p_Buffer[concatStringLength] = 0;

	memcpy(p_Buffer, p_String1, stringLenght1);
	memcpy(p_Buffer + stringLenght1, p_String2, stringLenght2);

	return p_Buffer;
}
/*********************************************************************************/
char* K15_GenerateString(const char* p_Format, char* p_Buffer, ...)
{
	va_list list;
	va_start(list, p_Buffer);
	vsprintf(p_Buffer, p_Format, list);
	va_end(list);

	return p_Buffer;
}
/*********************************************************************************/
char** K15_CreateStringArray(unsigned int p_NumStrings, ...)
{
	va_list argumentList;
	va_start(argumentList, p_NumStrings);

	char** stringArray = K15_InternalVCreateStringArrayIntoBuffer((char**)malloc(sizeof(char*) * p_NumStrings), p_NumStrings, argumentList);

	va_end(argumentList);

	return stringArray;
}
/*********************************************************************************/
char** K15_CreateStringArrayIntoBuffer(char** p_Buffer, unsigned int p_NumStrings, ...)
{
	va_list argumentList;
	va_start(argumentList, p_NumStrings);
	
	char** stringArray = K15_InternalVCreateStringArrayIntoBuffer(p_Buffer, p_NumStrings, argumentList);

	va_end(argumentList);

	return stringArray;
}
/*********************************************************************************/
char* K15_ConvertToLower(const char* p_String)
{
	assert(p_String);

	unsigned int stringLength = (unsigned int)strlen(p_String);

	assert(stringLength);

	char* stringBuffer = (char*)malloc(stringLength + 1);

	return K15_ConvertToLowerIntoBuffer(p_String, stringBuffer);
}
/*********************************************************************************/
char* K15_ConvertToLowerIntoBuffer(const char* p_String, char* p_Buffer)
{
	assert(p_String);
	assert(p_Buffer);

	char* stringBuffer = p_Buffer;

	while(*p_String)
	{
		*stringBuffer++ = tolower(*p_String++);
	}

	*stringBuffer = 0;

	return p_Buffer;
}
/*********************************************************************************/
char* K15_ConvertToUpper(const char* p_String)
{
	assert(p_String);

	unsigned int stringLength = (unsigned int)strlen(p_String);

	assert(stringLength);

	char* stringBuffer = (char*)malloc(stringLength + 1);

	return K15_ConvertToUpperIntoBuffer(p_String, stringBuffer);
}
/*********************************************************************************/
char* K15_ConvertToUpperIntoBuffer(const char* p_String, char* p_Buffer)
{
	assert(p_String);
	assert(p_Buffer);

	char* stringBuffer = p_Buffer;

	while(*p_String)
	{
		*stringBuffer++ = toupper(*p_String++);
	}

	*stringBuffer = 0;

	return p_Buffer;
}
/*********************************************************************************/
char* K15_ConcatStringArray(const char** p_StringArray, unsigned int p_ArrayLength)
{
	//get combined string length
	unsigned int stringLength = 0;

	for (unsigned int i = 0;
		i < p_ArrayLength;
		++i)
	{
		stringLength += (unsigned int)strlen(p_StringArray[i]);
	}

	return K15_ConcatStringArrayIntoBuffer(p_StringArray, (char*)malloc(stringLength), p_ArrayLength);
}
/*********************************************************************************/
char* K15_ConcatStringArrayIntoBuffer(const char** p_StringArray, char* p_Buffer, unsigned int p_ArrayLength)
{
	unsigned int offset = 0;

	for (unsigned int i = 0;
		i < p_ArrayLength;
		++i)
	{
		offset += sprintf(p_Buffer + offset, "%s", p_StringArray[i]);
	}

	p_Buffer[offset] = 0;
	
	return p_Buffer;
}
/*********************************************************************************/
unsigned int K15_GenerateDataHash(const unsigned char* p_Data, unsigned int p_DataSize)
{
	unsigned int hash = 0;

	for (unsigned int i = 0; i < p_DataSize; i++) 
	{
		hash = 33*hash + 720 + p_Data[i];
	}

	return hash;
}
/*********************************************************************************/
unsigned int K15_GenerateStringHash(const char* p_String, unsigned int p_StringLength /*= 0*/)
{
	unsigned int size = p_StringLength == 0 ? (unsigned int)strlen(p_String) : 0;

	return K15_GenerateDataHash((const unsigned char*)p_String, size);
}
/*********************************************************************************/
