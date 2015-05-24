#include "K15_FileWatch.h"
#include "K15_Prerequisites.h"
#include "K15_DefaultCLibraries.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileWatchWin32.h"
#endif //K15_OS_WINDOWS

#include "generated/K15_FileWatchEntryStretchBuffer.cpp"
#include "generated/K15_DirectoryWatchEntryStretchBuffer.cpp"

#include "K15_FileSystem.h"
#include "K15_Logging.h"
#include "K15_System.h"
#include "K15_OSContext.h"

/*********************************************************************************/
intern uint8 K15_InternalFileWatchComparer(K15_FileWatchEntry* p_FileWatchEntry, void* p_UserData)
{
	const char* fileName = (char*)p_UserData;

	return strcmp(fileName, p_FileWatchEntry->filePath);
}
/*********************************************************************************/
intern uint8 K15_InternalDirectoryWatchComparer(K15_DirectoryWatchEntry* p_DirectoryWatchEntry, void* p_UserData)
{
	const char* dirPath = (char*)p_UserData;

	return strcmp(dirPath, p_DirectoryWatchEntry->dirPath);
}
/*********************************************************************************/
K15_FileWatchEntry* K15_AddFileWatch(const char* p_FilePath, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_Flags)
{
	K15_ASSERT_TEXT(p_FilePath, "File Path is NULL.");
	K15_ASSERT_TEXT(p_NotificationFnc, "Notification Function is NULL.");

	if (K15_FileExists(p_FilePath) != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not add file '%s' to the file watcher (%s).", p_FilePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		return 0;
	}

	char* dirPath = K15_GetPathWithoutFileName(p_FilePath);

	K15_OSContext* osContext = K15_GetOSLayerContext();

	K15_DirectoryWatchEntryStretchBuffer* directoryWatchBuffer = &osContext->system.directoryWatchEntries;

	K15_DirectoryWatchEntry* directoryWatchEntry = K15_GetDirectoryWatchEntryElementConditional(directoryWatchBuffer, K15_InternalDirectoryWatchComparer, dirPath);

	if (!directoryWatchEntry)
	{
		K15_DirectoryWatchEntry* newDirectoryWatchEntry = (K15_DirectoryWatchEntry*)K15_OS_MALLOC(sizeof(K15_DirectoryWatchEntry));
		memset(newDirectoryWatchEntry, 0, sizeof(K15_DirectoryWatchEntry));
		newDirectoryWatchEntry->dirPath = dirPath;
		
		K15_CreateFileWatchEntryStretchBuffer(&newDirectoryWatchEntry->fileWatchEntries);
		K15_PushDirectoryWatchEntry(directoryWatchBuffer, *newDirectoryWatchEntry);

		directoryWatchEntry = newDirectoryWatchEntry;
	}
	else
	{
		K15_OS_FREE(dirPath);
	}

	K15_FileWatchEntryStretchBuffer* fileWatchBuffer = &directoryWatchEntry->fileWatchEntries;

	//check if file is already being watched
	K15_FileWatchEntry* fileWatchEntry = K15_GetFileWatchEntryElementConditional(fileWatchBuffer, K15_InternalFileWatchComparer, (void*)p_FilePath);

	if (fileWatchEntry)
	{
		K15_LOG_WARNING_MESSAGE("File '%s' is already being watched.", p_FilePath);
		return fileWatchEntry;
	}

	size_t filePathLength = strlen(p_FilePath);
	char* filePathBuffer = (char*)K15_OS_MALLOC(filePathLength + 1); //+1 for 0 terminator
	filePathBuffer[filePathLength] = 0;
	memcpy(filePathBuffer, p_FilePath, filePathLength);

	K15_FileWatchEntry* newFileWatchEntry = (K15_FileWatchEntry*)K15_OS_MALLOC(sizeof(K15_FileWatchEntry));
	newFileWatchEntry->filePath = filePathBuffer;
	newFileWatchEntry->fileName = K15_GetFileNameWithoutPath(filePathBuffer);
	newFileWatchEntry->userParamter = p_UserData;
	newFileWatchEntry->notification = p_NotificationFnc;
	newFileWatchEntry->flags = p_Flags;

	K15_PushFileWatchEntry(fileWatchBuffer, *newFileWatchEntry);

	fileWatchEntry = newFileWatchEntry;

	uint8 result = osContext->system.registerFileWatch(osContext, fileWatchEntry, directoryWatchEntry);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not add file '%s' to the file watcher (%s).", p_FilePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		K15_PopFileWatchEntryElement(fileWatchBuffer, *fileWatchEntry);
		K15_DeleteFileWatch(fileWatchEntry);
		fileWatchEntry = 0;
	}

	return fileWatchEntry;
}
/*********************************************************************************/
K15_FileWatchEntry* K15_AddFileWatchAndCopyUserData(const char* p_FilePath, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_UserDataSize, unsigned int p_Flags)
{
	byte* userDataBuffer = (byte*)K15_OS_MALLOC(p_UserDataSize);
	memcpy(userDataBuffer, p_UserData, p_UserDataSize);

	uint32 flags = p_Flags | K15_USER_DATA_OWNERSHIP;

	K15_FileWatchEntry* watchEntry = K15_AddFileWatch(p_FilePath, p_NotificationFnc, userDataBuffer, p_Flags);

	if (!watchEntry)
	{
		K15_OS_FREE(userDataBuffer);
	}

	return watchEntry;
}
/*********************************************************************************/
void K15_DeleteFileWatch(K15_FileWatchEntry* p_FileWatch)
{
	K15_ASSERT_TEXT(p_FileWatch, "File Watch Entry is NULL.");

	K15_OS_FREE(p_FileWatch->fileName);
	K15_OS_FREE(p_FileWatch->filePath);

	if ((p_FileWatch->flags & K15_USER_DATA_OWNERSHIP) > 0)
	{
		K15_OS_FREE(p_FileWatch->userParamter);
	}

	K15_OS_FREE(p_FileWatch);
}
/*********************************************************************************/