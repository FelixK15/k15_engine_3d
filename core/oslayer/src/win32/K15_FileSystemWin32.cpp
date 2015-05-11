#include "win32/K15_FileSystemWin32.h"
#include "win32/K15_HelperWin32.h"

/*********************************************************************************/
uint32 K15_Win32GetFileSize(const char* p_FilePath)
{
	assert(p_FilePath);

	uint32 filePathLength = (uint32)strlen(p_FilePath) + 1;

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));  //+1 for 0 terminator

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	uint32 fileSize = 0;
	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(fileHandle, 0);
	}

	CloseHandle(fileHandle);

	return fileSize;
}
/*********************************************************************************/
uint8 K15_Win32FileExists(const char* p_FilePath)
{
	assert(p_FilePath);

	int filePathLength = (uint32)strlen(p_FilePath) + 1;

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));  //+1 for 0 terminator

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	uint8 result = K15_SUCCESS;

	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	result = fileHandle == INVALID_HANDLE_VALUE ? K15_OS_ERROR_FILE_NOT_FOUND : K15_SUCCESS;

	CloseHandle(fileHandle);
	
	return result;
}
/*********************************************************************************/
char* K15_Win32ConvertToSystemPath(const char* p_FilePath)
{
	assert(p_FilePath);

	size_t filePathSize = strlen(p_FilePath);
	char* convertedFilePath = (char*)K15_OS_MALLOC(filePathSize + 1); //+1 for 0 terminator
	const char* filePath = p_FilePath;

	for (size_t fileNameIndex = 0;
		 fileNameIndex < filePathSize;
		 ++fileNameIndex)
	{
		if (filePath[fileNameIndex] == '/')
		{
			convertedFilePath[fileNameIndex] = '\\';
		}
		else
		{
			convertedFilePath[fileNameIndex] = filePath[fileNameIndex];
		}
	}

	convertedFilePath[filePathSize] = 0;

	return convertedFilePath;
}
/*********************************************************************************/