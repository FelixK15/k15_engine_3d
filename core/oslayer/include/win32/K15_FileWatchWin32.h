#ifndef _K15_OSLayer_File_Watch_Win32_h_
#define _K15_OSLayer_File_Watch_Win32_h_

#include "K15_Prerequisites.h"

#define K15_WIN32_READDIRECTORYCHANGES_NOTIFICATION_COUNT 32

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

result8 K15_Win32RegisterFileWatch(K15_OSContext* p_OSContext, K15_FileWatchEntry* p_FileWatchEntry, K15_DirectoryWatchEntry* p_DirectoryWatchEntry);
result8 K15_Win32RegisterChangeNotification(HANDLE p_DirectoryHandle, K15_DirectoryWatchEntry* p_DirectoryWatchEntry, K15_DirectoryWatchEntryWin32* p_DirectoryWatchEntryWin32);
#endif //_K15_OSLayer_File_Watch_Win32_h_