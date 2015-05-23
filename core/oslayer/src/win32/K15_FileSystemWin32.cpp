#include "K15_DefaultCLibraries.h"

#include "K15_FileSystem.h"
#include "win32/K15_FileSystemWin32.h"
#include "win32/K15_HelperWin32.h"

#include "win32/K15_HeaderDefaultWin32.h"

/*********************************************************************************/
uint32 K15_Win32GetFileSize(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");

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
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");

	int filePathLength = (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	uint8 result = K15_SUCCESS;

	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	result = fileHandle == INVALID_HANDLE_VALUE ? K15_OS_ERROR_FILE_NOT_FOUND : K15_SUCCESS;

	CloseHandle(fileHandle);
	
	return result;
}
/*********************************************************************************/
byte* K15_Win32ReadWholeFile(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");

	int filePathLength = (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	uint8 result = K15_SUCCESS;

	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD fileSize = INVALID_FILE_SIZE;
	byte* fileContent = 0;

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(fileHandle, NULL);
	}
	else
	{
		result = K15_OS_ERROR_SYSTEM;
	}

	if (fileHandle != INVALID_HANDLE_VALUE &&
		fileSize != INVALID_FILE_SIZE)
	{
		fileContent = (byte*)K15_OS_MALLOC(fileSize + 1); //+1 = 0 terminator
		ReadFile(fileHandle, fileContent, fileSize, 0, 0);
		fileContent[fileSize] = 0;
	}
	
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(fileHandle);
	}

	return fileContent;
}
/*********************************************************************************/
char* K15_Win32GetWorkingDirectory()
{
	DWORD bufferLength = GetCurrentDirectoryW(0, 0);
	wchar_t* wideWorkingDirectoryPath = (wchar_t*)alloca(bufferLength * sizeof(wchar_t));

	DWORD written = GetCurrentDirectoryW(bufferLength, wideWorkingDirectoryPath);
	bufferLength = written + 1;

	char* workingDirectory = (char*)K15_OS_MALLOC(bufferLength);

	K15_Win32ConvertWStringToString(wideWorkingDirectoryPath, bufferLength, workingDirectory);

	workingDirectory[written] = '\\';
	workingDirectory[bufferLength] = 0;


	return K15_ConvertToSystemPathInplace(workingDirectory);
}
/*********************************************************************************/