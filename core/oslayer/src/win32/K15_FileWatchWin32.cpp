#include "win32/K15_FileWatchWin32.h"
#include "win32/K15_HeaderDefaultWin32.h"
#include "win32/K15_HelperWin32.h"

#include "K15_System.h"
#include "K15_Logging.h"

#include "K15_DefaultCLibraries.h"
#include "K15_FileSystem.h"
#include "K15_FileWatch.h"

#define K15_WIN32_READDIRECTORYCHANGES_NOTIFICATION_COUNT 8

/*********************************************************************************/
struct K15_DirectoryWatchOverlapped
{
	OVERLAPPED overlapped;
	K15_DirectoryWatchEntry* directoryWatchEntry;
};
/*********************************************************************************/
struct K15_DirectoryWatchEntryWin32
{
	FILE_NOTIFY_INFORMATION notificationBuffer[K15_WIN32_READDIRECTORYCHANGES_NOTIFICATION_COUNT];
	K15_DirectoryWatchOverlapped overlapped;
	HANDLE directoryHandle;
};
/*********************************************************************************/

//forward declaration for the callback
intern inline uint8 K15_Win32InternalRegisterChangeNotification(HANDLE, K15_DirectoryWatchEntry*, K15_DirectoryWatchEntryWin32*);

/*********************************************************************************/
intern uint8 K15_InternalWin32FileWatchEntryComparer(K15_FileWatchEntry* p_FileWatchEntry, void* p_UserData)
{
	char* filePath = (char*)p_UserData;
	return strcmp(p_FileWatchEntry->fileName, filePath);
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
		wchar_t* fileNameW = currentNotification->FileName;
		size_t fileNameLength = wcslen(fileNameW) + 1;
		//uint32 fileNameLength = currentNotification->FileNameLength + 1; //apparently not 0 terminated.

		char* fileName = (char*)alloca(fileNameLength);

		K15_Win32ConvertWStringToString(fileNameW, fileNameLength, fileName);
		
		K15_FileWatchEntry* fileWatchEntry = K15_GetFileWatchEntryElementConditional(fileWatchBuffer, K15_InternalWin32FileWatchEntryComparer, fileName);

		//check if file is being watched
		if (fileWatchEntry)
		{
			if (currentNotification->Action == FILE_ACTION_MODIFIED)
			{
				void* fileWatchUserData = fileWatchEntry->userParamter;
				fileWatchEntry->notification(fileWatchUserData);
			}
		}

		if (currentNotification->NextEntryOffset == 0)
		{
			break;
		}

		currentNotification = (FILE_NOTIFY_INFORMATION*)(((byte*)currentNotification) + currentNotification->NextEntryOffset);
	}

	K15_Win32InternalRegisterChangeNotification(directoryHandle, watchEntry, watchEntryWin32);
}
/*********************************************************************************/
intern inline uint8 K15_Win32InternalRegisterChangeNotification(HANDLE p_DirectoryHandle, K15_DirectoryWatchEntry* p_DirectoryWatchEntry, K15_DirectoryWatchEntryWin32* p_DirectoryWatchEntryWin32)
{
	uint32 notificationBufferSize = sizeof(FILE_NOTIFY_INFORMATION) * K15_WIN32_READDIRECTORYCHANGES_NOTIFICATION_COUNT;
	uint32 notificationFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE;

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
		size_t dirPathLength = strlen(dirPath) + 1;
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

		result = K15_Win32InternalRegisterChangeNotification(directoryHandle, p_DirectoryWatchEntry, win32DirWatchEntry);
	}

	return result;
}
/*********************************************************************************/