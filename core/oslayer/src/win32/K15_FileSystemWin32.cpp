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
uint8 K15_Win32CopyFile(const char* p_SourcePath, const char* p_DestinationPath)
{
	K15_ASSERT_TEXT(p_SourcePath, "Source path is NULL.");
	K15_ASSERT_TEXT(p_DestinationPath, "Destination path is NULL.");

	size_t sourcePathLength = strlen(p_SourcePath) + 1;
	size_t destinationPathLength = strlen(p_DestinationPath) + 1;

	wchar_t* sourcePathW = (wchar_t*)alloca(sourcePathLength * sizeof(wchar_t));
	wchar_t* destinationPathW = (wchar_t*)alloca(destinationPathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_SourcePath, sourcePathLength, sourcePathW);
	K15_Win32ConvertStringToWString(p_DestinationPath, destinationPathLength, destinationPathW);

	return CopyFileW(sourcePathW, destinationPathW, FALSE) == 0 ? K15_OS_ERROR_SYSTEM : K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_Win32DirectoryExists(const char* p_Path)
{
	K15_ASSERT_TEXT(p_Path, "Path is NULL.");

	size_t pathLength = strlen(p_Path);
	char* path = (char*)p_Path; //safe, we won't change path

	if (p_Path[pathLength] != K15_DIR_SEPARATOR)
	{
		pathLength++;
		path = (char*)alloca(pathLength + 2); //+2 = missing separator & 0 terminator
		sprintf(path, "%s%c", p_Path, K15_DIR_SEPARATOR);
	}

	pathLength++;
	wchar_t* pathW = (wchar_t*)alloca(pathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(path, pathLength, pathW);

	DWORD fileAttributes = GetFileAttributesW(pathW);

	if (fileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	return (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0 ? K15_SUCCESS : K15_OS_ERROR_DIRECTORY_NOT_FOUND;
}
/*********************************************************************************/
byte* K15_Win32ReadWholeFileIntoBuffer(const char* p_FilePath, byte* p_Buffer)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");
	K15_ASSERT_TEXT(p_Buffer, "buffer is NULL.");

	int filePathLength = (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	uint8 result = K15_SUCCESS;

	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD fileSize = INVALID_FILE_SIZE;

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(fileHandle, NULL);
	}

	if (fileHandle != INVALID_HANDLE_VALUE &&
		fileSize != INVALID_FILE_SIZE)
	{
		ReadFile(fileHandle, p_Buffer, fileSize, 0, 0);
	}
	
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(fileHandle);
	}

	return p_Buffer;
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