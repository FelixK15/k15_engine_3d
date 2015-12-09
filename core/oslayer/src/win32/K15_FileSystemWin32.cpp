#include "K15_DefaultCLibraries.h"

#include "K15_FileSystem.h"
#include "K15_String.h"
#include "win32/K15_FileSystemWin32.h"
#include "win32/K15_HelperWin32.h"

#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HeaderExtensionsWin32.h"

typedef void (*K15_InternalWin32FindFileFnc)(WIN32_FIND_DATAW*, const char*, void**);

struct K15_InternalWin32FindFile
{
	K15_InternalWin32FindFileFnc function;
	void* userData;
};


/*********************************************************************************/
intern void K15_InternalCountFiles(WIN32_FIND_DATAW* p_FindData, const char* p_DefaultPath, void** p_UserData)
{
	uint32* counter = (uint32*)(*p_UserData);
	
	(*counter)++;
}
/*********************************************************************************/
intern void K15_InternalSaveFiles(WIN32_FIND_DATAW* p_FindData, const char* p_DefaultPath, void** p_UserData)
{
	char*** savedFilesPtr = (char***)p_UserData;
	char** savedFiles = (char**)(*p_UserData);

	wchar_t* wideFileName = p_FindData->cFileName;
	uint32 fileNameLength = (uint32)wcslen(wideFileName);
	uint32 pathLength= (uint32)strlen(p_DefaultPath);

	uint32 completePathLength = fileNameLength + pathLength + 1;

	char* filePath = (char*)K15_OS_MALLOC(completePathLength);
	char* convertedFilePath = (char*)alloca(fileNameLength + 1);

	K15_Win32ConvertWStringToString(wideFileName, fileNameLength + 1, convertedFilePath);
	memcpy(filePath, p_DefaultPath, pathLength);
	memcpy(filePath + pathLength, convertedFilePath, fileNameLength + 1);

	*savedFiles = filePath;
	++(*savedFilesPtr);
}
/*********************************************************************************/
intern void K15_InternalRecursiveWin32GetFiles(K15_InternalWin32FindFile* findFile, const char* p_DefaultPath, wchar_t* p_WidePath, bool8 p_Recursively)
{
	WIN32_FIND_DATAW findData = {};
	HANDLE findFileHandle = FindFirstFileW(p_WidePath, &findData);

	while (findFileHandle != INVALID_HANDLE_VALUE)
	{
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0
			&& p_Recursively == K15_TRUE)
		{
			K15_InternalRecursiveWin32GetFiles(findFile, p_DefaultPath, findData.cFileName, p_Recursively);
		}

		findFile->function(&findData, p_DefaultPath, &findFile->userData);

		if (FindNextFileW(findFileHandle, &findData) == FALSE)
		{
			FindClose(findFileHandle);
			findFileHandle = INVALID_HANDLE_VALUE;
			break;
		}
	}
}
/*********************************************************************************/
bool8 K15_Win32IsRelativePath(const char* p_Path)
{
	K15_ASSERT_TEXT(p_Path, "path is NULL.");

	bool8 isRelative = K15_FALSE;

	if (p_Path[0] != 0 && p_Path[1] != 0 && p_Path[2] != 0)
	{
		isRelative = !(isalpha(p_Path[0]) && p_Path[1] == ':' && p_Path[2] == '/');
	}

	return isRelative;
}
/*********************************************************************************/
char** K15_Win32GetFilesInDirectory(const char* p_Path, uint32* p_FileCounter, const char* p_Filter, bool8 p_Recursively)
{
	K15_ASSERT_TEXT(p_Path, "path is NULL.");
	K15_ASSERT_TEXT(p_Filter, "filter is NULL.");
	K15_ASSERT_TEXT(p_FileCounter, "filecounter is NULL.");

	uint32 fileCount = 0;
	uint32 pathLength= (uint32)strlen(p_Path) + 1;  //+1 for 0 terminator
	uint32 filterLength= (uint32)strlen(p_Filter);

	char* completeSearchPath = (char*)alloca(pathLength + filterLength);
	sprintf(completeSearchPath, "%s%s", p_Path, p_Filter);
	
	uint32 completePathLength= (uint32)strlen(completeSearchPath) + 1;

	wchar_t* widePath = (wchar_t*)alloca(completePathLength * sizeof(wchar_t));
	char** filesInDirectory = 0;

	K15_Win32ConvertStringToWString(completeSearchPath, completePathLength, widePath);

	K15_InternalWin32FindFile countFiles = {};
	countFiles.function = K15_InternalCountFiles;
	countFiles.userData = &fileCount;

	K15_InternalRecursiveWin32GetFiles(&countFiles, p_Path, widePath, p_Recursively);

	*p_FileCounter = fileCount;

	if (fileCount == 0)
	{
		return 0;
	}

	filesInDirectory = (char**)K15_OS_MALLOC(K15_PTR_SIZE * fileCount);

	K15_InternalWin32FindFile saveFiles = {};
	saveFiles.function = K15_InternalSaveFiles;
	saveFiles.userData = filesInDirectory;

	K15_InternalRecursiveWin32GetFiles(&saveFiles, p_Path, widePath, p_Recursively);

	return filesInDirectory;
}
/*********************************************************************************/
uint64 K15_Win32GetFileLastAccessTimeStamp(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");

	uint32 filePathLength= (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);
	
	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	uint64 lastAccessTimeStamp = 0;

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		FILETIME lastAccessTime = {};
		GetFileTime(fileHandle, 0, 0, &lastAccessTime);

		lastAccessTimeStamp = (lastAccessTime.dwHighDateTime << lastAccessTime.dwLowDateTime);
		CloseHandle(fileHandle);
	}
	
	return lastAccessTimeStamp;
}
/*********************************************************************************/
uint32 K15_Win32GetFileSize(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");

	uint32 filePathLength= (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

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

	uint32 filePathLength= (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

	wchar_t* wideFilePath = (wchar_t*)alloca(filePathLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_FilePath, filePathLength, wideFilePath);

	HANDLE fileHandle = CreateFileW(wideFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	uint8 result = fileHandle == INVALID_HANDLE_VALUE ? K15_FALSE : K15_TRUE;

	CloseHandle(fileHandle);
	
	return result;
}
/*********************************************************************************/
uint8 K15_Win32CopyFile(const char* p_SourcePath, const char* p_DestinationPath)
{
	K15_ASSERT_TEXT(p_SourcePath, "Source path is NULL.");
	K15_ASSERT_TEXT(p_DestinationPath, "Destination path is NULL.");

	uint32 sourcePathLength= (uint32)strlen(p_SourcePath) + 1;
	uint32 destinationPathLength= (uint32)strlen(p_DestinationPath) + 1;

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

	uint32 pathLength= (uint32)strlen(p_Path);
	char* path = (char*)p_Path; //safe, we won't change path

	if (p_Path[pathLength - 1] != K15_DIR_SEPARATOR)
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
		return K15_FALSE;
	}

	return (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0 ? K15_TRUE : K15_FALSE;
}
/*********************************************************************************/
byte* K15_Win32ReadWholeFileIntoBuffer(const char* p_FilePath, byte* p_Buffer)
{
	K15_ASSERT_TEXT(p_FilePath, "filepath is NULL.");
	K15_ASSERT_TEXT(p_Buffer, "buffer is NULL.");

	uint32 filePathLength= (uint32)strlen(p_FilePath) + 1;  //+1 for 0 terminator

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
    DWORD bytesRead = 0;
		ReadFile(fileHandle, p_Buffer, fileSize, &bytesRead, 0);
    fileSize = bytesRead;
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
	bufferLength += 1; //GetCurrentDirectoryW does not return the size including the 0 terminator

	wchar_t* wideWorkingDirectoryPath = (wchar_t*)alloca(bufferLength * sizeof(wchar_t));

	DWORD currentDirectoryLength = GetCurrentDirectoryW(bufferLength, wideWorkingDirectoryPath);
	char* workingDirectoryBuffer = (char*)alloca(currentDirectoryLength + 1);

	K15_Win32ConvertWStringToString(wideWorkingDirectoryPath, bufferLength, workingDirectoryBuffer);
	workingDirectoryBuffer[currentDirectoryLength] = 0;

	return  K15_ConvertToDirectoryPath(workingDirectoryBuffer);
}
/*********************************************************************************/