#ifndef _K15_OSLayer_File_Watch_h_
#define _K15_OSLayer_File_Watch_h_

#include "generated/K15_FileWatchEntryStretchBuffer.h"

typedef void(*K15_FileChangeNotificationFnc)(const char*, void*);

/*********************************************************************************/
enum K15_FileWatchFlags
{
	K15_USER_DATA_OWNERSHIP = 0x02 //Delete user data when deleting the file watch entry
};
/*********************************************************************************/
struct K15_DirectoryWatchEntry
{
	char* dirPath;
	void* userData;
	void* userParamter; //userData from the user
	K15_FileChangeNotificationFnc notification;
	K15_FileWatchEntryStretchBuffer fileWatchEntries;
};
/*********************************************************************************/
struct K15_FileWatchEntry
{
	K15_FileChangeNotificationFnc notification;
	char* filePath;
	char* fileName;
	void* userParamter; //userData from the user
	unsigned int flags;
};
/*********************************************************************************/

K15_FileWatchEntry* K15_AddDirectoryWatch(const char* p_DirectoryWatch, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_Flags = 0);
K15_FileWatchEntry* K15_AddFileWatch(const char* p_FilePath, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_Flags = 0);
K15_FileWatchEntry* K15_AddFileWatchAndCopyUserData(const char* p_FilePath, K15_FileChangeNotificationFnc p_NotificationFnc, void* p_UserData, unsigned int p_UserDataSize, unsigned int p_Flags = 0);

void K15_DeleteFileWatch(K15_FileWatchEntry* p_FileWatch);

#endif //_K15_OSLayer_File_Watch_h_