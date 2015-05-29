#include "win32/K15_HelperWin32.h"

#include "win32/K15_HeaderDefaultWin32.h"

/*********************************************************************************/
uint8 K15_Win32ConvertStringToWString(const char* p_String, size_t p_StringLength, wchar_t* p_OutputString)
{
	int charactersConverted = MultiByteToWideChar(CP_ACP, 0, p_String, -1, p_OutputString, (int)p_StringLength);

	if (charactersConverted == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32ConvertWStringToString(const wchar_t* p_String, size_t p_StringLength, char* p_OutputString)
{
	int charactersConverted = WideCharToMultiByte(CP_ACP, 0, p_String, -1, p_OutputString, (int)p_StringLength, 0, 0);

	if (charactersConverted == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
