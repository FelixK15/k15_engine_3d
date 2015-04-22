#include "posix/K15_FileSystemPosix.h"

/*********************************************************************************/
uint32 K15_PosixGetFileSize(const char* p_FilePath)
{
	assert(p_FilePath);

	struct stat filestat = {};

	int result = stat(p_FilePath, &filestat);

	if (result != 0)
	{
		return 0;
	}

	return (uint32)filestat.st_size;
}
/*********************************************************************************/
uint8 K15_PosixFileExists(const char* p_FilePath)
{
	assert(p_FilePath);

	struct stat filestat = {};

	int result = stat(p_FilePath, &filestat);

	return result == 0;
}
/*********************************************************************************/
char* K15_PosixConvertToSystemPath(const char* p_FilePath)
{
	assert(p_FilePath);

	size_t filePathSize = strlen(p_FilePath);
	char* convertedFilePath = (char*)K15_OS_MALLOC(filePathSize + 1); //+1 for 0 terminator
	const char* filePath = p_FilePath;

	for (size_t fileNameIndex = 0;
		fileNameIndex < filePathSize;
		++fileNameIndex)
	{
		if (filePath[fileNameIndex] == '\\')
		{
			convertedFilePath[fileNameIndex] = '/';
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