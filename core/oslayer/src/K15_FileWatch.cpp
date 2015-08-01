#include "K15_FileWatch.h"
#include "K15_Prerequisites.h"
#include "K15_DefaultCLibraries.h"

#ifdef K15_OS_WINDOWS
	#include "win32/K15_FileWatchWin32.h"
#endif //K15_OS_WINDOWS

#include "K15_CustomMemoryAllocator.h"

#include "generated/K15_FileWatchEntryStretchBuffer.cpp"
#include "generated/K15_DirectoryWatchEntryStretchBuffer.cpp"

#include "K15_FileSystem.h"
#include "K15_Logging.h"
#include "K15_System.h"
#include "K15_String.h"
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
intern K15_DirectoryWatchEntry* K15_InternalGetDirectoryWatch(K15_OSContext* p_OSContext, char* p_DirectoryPath)
{
	K15_DirectoryWatchEntryStretchBuffer* directoryWatchBuffer = &p_OSContext->system.directoryWatchEntries;
	K15_DirectoryWatchEntry* directoryWatchEntry = K15_GetDirectoryWatchEntryStretchBufferElementConditional(directoryWatchBuffer, K15_InternalDirectoryWatchComparer, p_DirectoryPath);

	return directoryWatchEntry;
}
/*********************************************************************************/
intern K15_DirectoryWatchEntry* K15_InternalAddDirectoryWatch(K15_OSContext* p_OSContext, char* p_DirectoryPath)
{
	K15_DirectoryWatchEntryStretchBuffer* directoryWatchBuffer = &p_OSContext->system.directoryWatchEntries;
	K15_DirectoryWatchEntry* newDirectoryWatchEntry = (K15_DirectoryWatchEntry*)K15_OS_MALLOC(sizeof(K15_DirectoryWatchEntry));
	memset(newDirectoryWatchEntry, 0, sizeof(K15_DirectoryWatchEntry));
	newDirectoryWatchEntry->dirPath = p_DirectoryPath;

	K15_CreateFileWatchEntryStretchBuffer(&newDirectoryWatchEntry->fileWatchEntries);
	return K15_PushDirectoryWatchEntryStretchBufferElement(directoryWatchBuffer, *newDirectoryWatchEntry);
}
/*********************************************************************************/
K15_FileWatchEntry* K15_AddDirectoryWatch(const char* p_DirectoryWatch, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_Flags /*= 0*/)
{
	char* directoryWatchPath = 0;

	if (K15_IsRelativePath(p_DirectoryWatch))
	{
		directoryWatchPath = K15_ConvertToAbsolutePath(p_DirectoryWatch);
	}
	else
	{
		directoryWatchPath = K15_ConvertToSystemPath(p_DirectoryWatch);
	}

	if (!K15_DirectoryExists(directoryWatchPath))
	{
		K15_LOG_ERROR_MESSAGE("Could not add directory '%s' to the file watcher (%s).", p_DirectoryWatch, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		free(directoryWatchPath);
		return 0;
	}

	K15_FileWatchEntry* watchEntry = K15_AddFileWatch(directoryWatchPath, p_NotificationFnc, p_UserData, p_Flags);
	free(directoryWatchPath);
	
	return watchEntry;
}
/*********************************************************************************/
K15_FileWatchEntry* K15_AddFileWatch(const char* p_FilePath, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_Flags)
{
	K15_ASSERT_TEXT(p_FilePath, "File Path is NULL.");
	K15_ASSERT_TEXT(p_NotificationFnc, "Notification Function is NULL.");

	bool8 isDirectory = K15_DirectoryExists(p_FilePath);

	if (K15_FileExists(p_FilePath) == K15_FALSE &&
		isDirectory == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Could not add file '%s' to the file watcher (%s).", p_FilePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		return 0;
	}

	K15_OSContext* osContext = K15_GetOSLayerContext();

	char* dirPath = K15_GetPathWithoutFileName(p_FilePath);

	K15_DirectoryWatchEntry* directoryWatchEntry = K15_InternalGetDirectoryWatch(osContext, dirPath);

	if (!directoryWatchEntry)
	{
		directoryWatchEntry = K15_InternalAddDirectoryWatch(osContext, dirPath);
	}
	else
	{
		K15_OS_FREE(dirPath);
	}

	K15_FileWatchEntryStretchBuffer* fileWatchBuffer = &directoryWatchEntry->fileWatchEntries;

	//check if file is already being watched
	K15_FileWatchEntry* fileWatchEntry = K15_GetFileWatchEntryStretchBufferElementConditional(fileWatchBuffer, K15_InternalFileWatchComparer, (void*)p_FilePath);

	if (fileWatchEntry)
	{
		K15_LOG_WARNING_MESSAGE("File '%s' is already being watched.", p_FilePath);
		return fileWatchEntry;
	}

	char* filePathBuffer = K15_CopyString(p_FilePath);
	char* fileName = K15_GetFileNameWithoutPath(filePathBuffer);

	fileName = fileName == 0 ? K15_CopyString(".") : fileName;

	K15_FileWatchEntry* newFileWatchEntry = (K15_FileWatchEntry*)K15_OS_MALLOC(sizeof(K15_FileWatchEntry));
	newFileWatchEntry->filePath = filePathBuffer;
	newFileWatchEntry->fileName = fileName;
	newFileWatchEntry->userParamter = p_UserData;
	newFileWatchEntry->notification = p_NotificationFnc;
	newFileWatchEntry->flags = p_Flags;
	newFileWatchEntry->lastWriteTime = 0;

	K15_PushFileWatchEntryStretchBufferElement(fileWatchBuffer, *newFileWatchEntry);

	fileWatchEntry = newFileWatchEntry;

	uint8 result = osContext->system.registerFileWatch(osContext, fileWatchEntry, directoryWatchEntry);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not add file '%s' to the file watcher (%s).", p_FilePath, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(K15_ERROR_MESSAGE_LENGTH)));
		K15_PopFileWatchEntryStretchBufferElement(fileWatchBuffer, *fileWatchEntry);
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
unsigned char K15_FileIsBeingWatched(const char* p_FilePath)
{
	assert(p_FilePath);

	K15_OSContext* osContext = K15_GetOSLayerContext();
	char* dirPath = K15_GetPathWithoutFileName(p_FilePath);

	K15_DirectoryWatchEntry* directoryWatchEntry = K15_InternalGetDirectoryWatch(osContext, dirPath);
	bool8 isBeingWatched = K15_FALSE;

	if (directoryWatchEntry)
	{
		K15_FileWatchEntry* fileWatchEntry = K15_GetFileWatchEntryStretchBufferElementConditional(&directoryWatchEntry->fileWatchEntries, K15_InternalFileWatchComparer, (void*)p_FilePath);
		isBeingWatched = fileWatchEntry != 0;
	}

	K15_OS_FREE(dirPath);

	return isBeingWatched;
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