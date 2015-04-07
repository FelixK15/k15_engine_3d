#include "win32/K15_FileSystemWin32.h"

/*********************************************************************************/
uint32 K15_Win32GetFileSize(const char* p_FilePath)
{
	assert(p_FilePath);

	uint32 fileSize = 0;
	HANDLE fileHandle = CreateFile(p_FilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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

	uint8 result = K15_SUCCESS;

	HANDLE fileHandle = CreateFile(p_FilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	result = fileHandle == INVALID_HANDLE_VALUE ? K15_ERROR_FILE_NOT_FOUND : K15_SUCCESS;

	CloseHandle(fileHandle);
	
	return result;
}
/*********************************************************************************/
char* K15_Win32ConvertToSystemPath(const char* p_FilePath)
{
	assert(p_FilePath);

	size_t filePathSize = strlen(p_FilePath);
	char* convertedFilePath = (char*)malloc(filePathSize + 1); //+1 for 0 terminator
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