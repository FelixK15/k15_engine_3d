#include "K15_StringHelper.h"

#include "K15_Prerequisites.h"
#include "K15_DefaultCLibraries.h"

/*********************************************************************************/
char* K15_OS_CopyString(const char* p_String, unsigned int p_StringLength, char* p_OutputBuffer)
{
	char* outputBuffer = p_OutputBuffer;

	if (p_StringLength == 0)
	{
		p_StringLength = (unsigned int)strlen(p_String);
	}

	if (!outputBuffer)
	{
		outputBuffer = (char*)K15_OS_MALLOC(p_StringLength + 1);
	}

	//set 0 terminator
	outputBuffer[p_StringLength] = 0;

	memcpy(outputBuffer, p_String, p_StringLength);

	return outputBuffer;
}
/*********************************************************************************/