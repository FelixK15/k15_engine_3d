#include "win32/K15_FileWatchWin32.h"
#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HelperWin32.h"

#include "K15_System.h"
#include "K15_Logging.h"
#include "K15_String.h"

#include "K15_DefaultCLibraries.h"
#include "K15_FileSystem.h"
#include "K15_FileWatch.h"

#include "K15_Thread.h"
#include "K15_OSContext.h"

/*********************************************************************************/
intern uint8 K15_InternalWin32FileWatchEntryComparer(K15_FileWatchEntry* p_FileWatchEntry, void* p_UserData)
{
	char* filePath = (char*)p_UserData;
	return strcmp(p_FileWatchEntry->fileName, ".") == 0 ? 0 : strcmp(p_FileWatchEntry->fileName, filePath);
}
/*********************************************************************************/
intern inline VOID WINAPI K15_Win32InternalFileChangeCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	K15_DirectoryWatchOverlapped* overlapped = (K15_DirectoryWatchOverlapped*)lpOverlapped;
	K15_DirectoryWatchEntry* watchEntry = overlapped->directoryWatchEntry;
	K15_DirectoryWatchEntryWin32* watchEntryWin32 = (K15_DirectoryWatchEntryWin32*)watchEntry->userData;

	K15_FileWatchEntryStretchBuffer* fileWatchBuffer = &watchEntry->fileWatchEntries;

	FILE_NOTIFY_INFORMATION* notificationBuffer = watchEntryWin32->notificationBuffer;
	HANDLE directoryHandle = watchEntryWin32->directoryHandle;

	FILE_NOTIFY_INFORMATION* currentNotification = &notificationBuffer[0];

	for(;;)
	{
		char* dirPath = watchEntry->dirPath;
		wchar_t* fileNameW = currentNotification->FileName;
		size_t fileNameLength = wcslen(fileNameW) + 1;
		size_t directoryLength = strlen(watchEntry->dirPath);
		//uint32 fileNameLength = currentNotification->FileNameLength + 1; //apparently not 0 terminated.

		char* fileName = (char*)alloca(fileNameLength);
		char* filePath = (char*)alloca(fileNameLength + directoryLength);
		wchar_t* filePathW = (wchar_t*)alloca((fileNameLength + directoryLength) * sizeof(wchar_t));

		K15_Win32ConvertWStringToString(fileNameW, fileNameLength, fileName);
		
		filePath = K15_ConcatStringsIntoBuffer(dirPath, fileName, filePath);

		K15_Win32ConvertStringToWString(filePath, fileNameLength + directoryLength, filePathW);

		K15_FileWatchEntry* fileWatchEntry = K15_GetFileWatchEntryStretchBufferElementConditional(fileWatchBuffer, K15_InternalWin32FileWatchEntryComparer, fileName);

		//check if file is being watched
		if (fileWatchEntry &&
			(currentNotification->Action == FILE_ACTION_MODIFIED ||
			currentNotification->Action == FILE_ACTION_ADDED))
		{
			//automatically notify on file added
			bool8 notify = currentNotification->Action == FILE_ACTION_ADDED;

			//check the time the file got touched last. (so we can avoid sending change events more than once)
			HANDLE fileHandle = CreateFileW(filePathW, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, 0);

			if (fileHandle != INVALID_HANDLE_VALUE)
			{
				//only check last written time on modified file actions
				if (currentNotification->Action == FILE_ACTION_MODIFIED)
				{
					FILETIME lastWrittenTimeWin32 = {};
					GetFileTime(fileHandle, 0, 0, &lastWrittenTimeWin32);

					CloseHandle(fileHandle);

					LARGE_INTEGER largeLastWrittenTime = {};
					largeLastWrittenTime.HighPart = lastWrittenTimeWin32.dwHighDateTime;
					largeLastWrittenTime.LowPart = lastWrittenTimeWin32.dwLowDateTime;

					unsigned long long lastWrittenTime = largeLastWrittenTime.QuadPart;

					if (fileWatchEntry->lastWriteTime < lastWrittenTime)
					{
						fileWatchEntry->lastWriteTime = lastWrittenTime;
						notify = K15_TRUE;
					}
				}

				if (notify)
				{
					void* fileWatchUserData = fileWatchEntry->userParamter;
					char* fullFilePath = K15_ConcatStringsIntoBuffer(watchEntry->dirPath, fileName, (char*)alloca(512));
					fileWatchEntry->notification(fullFilePath, fileWatchUserData);
				}
			}
		}

		if (currentNotification->NextEntryOffset == 0)
		{
			break;
		}

		currentNotification = (FILE_NOTIFY_INFORMATION*)(((byte*)currentNotification) + currentNotification->NextEntryOffset);
	}

	K15_Win32RegisterChangeNotification(directoryHandle, watchEntry, watchEntryWin32);
}
/*********************************************************************************/
intern result8 K15_Win32InternalDeferRegisterChangeNotification(HANDLE p_DirectoryHandle, K15_DirectoryWatchEntry* p_DirectoryWatchEntry, K15_DirectoryWatchEntryWin32* p_DirectoryWatchEntryWin32)
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_Win32Context* win32Context = (K15_Win32Context*)osContext->userData;

	K15_Win32DeferedFileWatchRegistration deferedFileWatch = {};

	deferedFileWatch.directoryHandle = p_DirectoryHandle;
	deferedFileWatch.directoryWatchEntry = p_DirectoryWatchEntry;
	deferedFileWatch.directoryWatchEntryWin32 = p_DirectoryWatchEntryWin32;

	uint32 currentIndex = win32Context->numDeferedFileWatch;
	uint32 nextIndex = currentIndex + 1;

	for(;;)
	{
		if (win32Context->numDeferedFileWatch == nextIndex)
		{
			break;
		}
		else
		{
			currentIndex = win32Context->numDeferedFileWatch;
			nextIndex = currentIndex + 1;
		}
		K15_InterlockedCompareExchange(&win32Context->numDeferedFileWatch, nextIndex, currentIndex);
	}
	
	win32Context->deferedFileWatch[currentIndex] = deferedFileWatch;

	return K15_SUCCESS;
}
/*********************************************************************************/
result8 K15_Win32RegisterChangeNotification(HANDLE p_DirectoryHandle, K15_DirectoryWatchEntry* p_DirectoryWatchEntry, K15_DirectoryWatchEntryWin32* p_DirectoryWatchEntryWin32)
{
	uint32 notificationBufferSize = sizeof(FILE_NOTIFY_INFORMATION) * K15_WIN32_READDIRECTORYCHANGES_NOTIFICATION_COUNT;
	uint32 notificationFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE/* | FILE_NOTIFY_CHANGE_LAST_WRITE*/;

	K15_DirectoryWatchOverlapped* overlapped = &p_DirectoryWatchEntryWin32->overlapped;
	overlapped->directoryWatchEntry = p_DirectoryWatchEntry;
	memset(&overlapped->overlapped, 0, sizeof(OVERLAPPED));

	FILE_NOTIFY_INFORMATION* notificationBuffer = p_DirectoryWatchEntryWin32->notificationBuffer;
	memset(notificationBuffer, 0, notificationBufferSize);

	BOOL result = ReadDirectoryChangesW(p_DirectoryHandle, notificationBuffer, notificationBufferSize, FALSE, notificationFilter, 0, (LPOVERLAPPED)overlapped, K15_Win32InternalFileChangeCallback);

	return result == TRUE ? K15_SUCCESS : K15_OS_ERROR_SYSTEM;
}
/*********************************************************************************/
uint8 K15_Win32RegisterFileWatch(K15_OSContext* p_OSContext, K15_FileWatchEntry* p_FileWatchEntry, K15_DirectoryWatchEntry* p_DirectoryWatchEntry)
{
	K15_DirectoryWatchEntryWin32* win32DirWatchEntry = (K15_DirectoryWatchEntryWin32*)p_DirectoryWatchEntry->userData;
	uint8 result = K15_SUCCESS;

	if (!win32DirWatchEntry)
	{
		char* dirPath = p_DirectoryWatchEntry->dirPath;
		uint32 dirPathLength= (uint32)strlen(dirPath) + 1;
		wchar_t* dirPathW = (wchar_t*)alloca(dirPathLength * sizeof(wchar_t));

		K15_Win32ConvertStringToWString(dirPath, dirPathLength, dirPathW);

		HANDLE directoryHandle = CreateFileW(dirPathW, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, 0);

		if (directoryHandle == INVALID_HANDLE_VALUE)
		{
			return K15_OS_ERROR_SYSTEM;
		}

		win32DirWatchEntry = (K15_DirectoryWatchEntryWin32*)K15_OS_MALLOC(sizeof(K15_DirectoryWatchEntryWin32));
		win32DirWatchEntry->directoryHandle = directoryHandle;

		p_DirectoryWatchEntry->userData = win32DirWatchEntry;

		//defer registration if we're currently not in the main thread
		bool8 mainThread = K15_GetCurrentThread() == K15_MAIN_THREAD;

		if (mainThread)
		{
			result = K15_Win32RegisterChangeNotification(directoryHandle, p_DirectoryWatchEntry, win32DirWatchEntry);
		}
		else
		{
			result = K15_Win32InternalDeferRegisterChangeNotification(directoryHandle, p_DirectoryWatchEntry, win32DirWatchEntry);
		}
	}

	return result;
}
/*********************************************************************************/