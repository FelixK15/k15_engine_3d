#include "K15_FileSystem.h"

#include "K15_DefaultCLibraries.h"
#include "K15_OSContext.h"

/*********************************************************************************/
const char* K15_GetWorkingDirectory()
{
	K15_OSContext* osContext = K15_GetOSLayerContext();

	return osContext->system.homeDir;
}
/*********************************************************************************/
byte* K15_GetWholeFileContent(const char* p_FilePath)
{
	return K15_GetWholeFileContentWithFileSize(p_FilePath, 0);
}
/*********************************************************************************/
byte* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	uint32 fileSize = K15_GetFileSize(p_FilePath);
	byte* fileContent = (byte*)K15_OS_MALLOC(fileSize + 1);

	if (fileSize > 0)
	{
		fileContent = K15_CopyWholeFileContentIntoBuffer(p_FilePath, fileContent);

		if (p_FileSize)
		{
			*p_FileSize = fileSize;
		}
	}

	return fileContent;
}
/*********************************************************************************/
char* K15_GetPathWithoutFileName(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	char* convertedFilePath = K15_ConvertToSystemPath(p_FilePath);
	const char* fileNameOnly = strrchr(convertedFilePath, K15_DIR_SEPARATOR);

	char* returnBuffer = 0;
	size_t returnBufferSize = 0;

	if (!fileNameOnly)
	{
		returnBufferSize = strlen(convertedFilePath); //+1 for 0 terminator
		returnBuffer = (char*)K15_OS_MALLOC(returnBufferSize);
		memcpy(returnBuffer, convertedFilePath, returnBufferSize);
	}
	else
	{
		//directory seperator is part of the return of strrchr
		fileNameOnly++;

		size_t filePathWithFileNameSize = strlen(convertedFilePath);
		size_t fileNameSize = strlen(fileNameOnly);
		
		returnBufferSize = filePathWithFileNameSize - fileNameSize;
		returnBuffer = (char*)K15_OS_MALLOC(returnBufferSize + 1); //+1 for 0 terminator
		memcpy(returnBuffer, convertedFilePath, returnBufferSize);
	}

	returnBuffer[returnBufferSize] = 0; //0 terminator

	K15_OS_FREE(convertedFilePath);

	return returnBuffer;
}
/*********************************************************************************/
char* K15_GetFileNameWithoutPath(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	char* convertedFilePath = K15_ConvertToSystemPath(p_FilePath);
	const char* fileNameOnly = strrchr(convertedFilePath, K15_DIR_SEPARATOR);

	size_t fileNameLength = strlen(fileNameOnly);
	uint32 offset = 0;

	if (fileNameOnly[0] == K15_DIR_SEPARATOR)
	{
		fileNameLength -= 1;
		offset = 1;
	}

	char* fileNameBuffer = (char*)K15_OS_MALLOC(fileNameLength + 1);
	fileNameBuffer[fileNameLength] = 0;

	memcpy(fileNameBuffer, fileNameOnly + offset, fileNameLength);

	K15_OS_FREE(convertedFilePath);

	return fileNameBuffer;
}
/*********************************************************************************/
char* K15_GetFileNameWithoutExtension(const char* p_FilePath)
{
	char* fileName = K15_GetFileNameWithoutPath(p_FilePath);
	char* fileNameWithoutExtension = 0;

	size_t fileNameLength = strlen(p_FilePath);

	for (size_t fileNameIndex = fileNameLength;
		 fileNameIndex > 0;
		 --fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			fileNameWithoutExtension = (char*)K15_OS_MALLOC(fileNameIndex + 1); //+1 for 0 terminator
			memcpy(fileNameWithoutExtension, p_FilePath, fileNameIndex);
			fileNameWithoutExtension[fileNameIndex] = 0;
			break;
		}
	}

	return fileNameWithoutExtension;
}
/*********************************************************************************/
char* K15_GetFileExtension(const char* p_FilePath)
{
	char* fileName = K15_GetFileNameWithoutPath(p_FilePath);
	char* extension = 0;

	size_t fileNameLength = strlen(p_FilePath);

	for (size_t fileNameIndex = 0;
		fileNameIndex > fileNameLength;
		++fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			size_t extensionLength = fileNameLength - fileNameIndex;

			assert(extensionLength > 0);

			extension = (char*)K15_OS_MALLOC(extensionLength + 1); //+1 for 0 terminator
			memcpy(extension, fileName + fileNameIndex, extensionLength);
			extension[extensionLength] = 0;
			break;
		}
	}

	return fileName;
}
/*********************************************************************************/
char* K15_ConvertToAbsolutePath(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	const char* workingDir = K15_GetWorkingDirectory();
	size_t workingDirLength = strlen(workingDir);
	size_t filePathLength = strlen(p_FilePath);
	size_t absolutPathLength = workingDirLength + filePathLength;

	char* absolutPathBuffer = (char*)K15_OS_MALLOC(absolutPathLength + 1);
	absolutPathBuffer[absolutPathLength] = 0;

	memcpy(absolutPathBuffer, workingDir, workingDirLength);
	memcpy(absolutPathBuffer + workingDirLength, p_FilePath, filePathLength);

	return K15_ConvertToSystemPathInplace(absolutPathBuffer);
}
/*********************************************************************************/
char* K15_ConvertToSystemPath(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	size_t filePathLength = strlen(p_FilePath);
	char* convertedFilePath = (char*)K15_OS_MALLOC(filePathLength + 1); //+1 for 0 terminator
	memcpy(convertedFilePath, p_FilePath, filePathLength);
	convertedFilePath[filePathLength] = 0;

	return K15_ConvertToSystemPathInplace(convertedFilePath);

	/*const char* filePath = p_FilePath;

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

	return convertedFilePath;*/
}
/*********************************************************************************/
char* K15_ConvertToSystemPathInplace(char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	size_t filePathLength = strlen(p_FilePath);
	char* convertedFilePath = p_FilePath;

	for (size_t fileNameIndex = 0;
		fileNameIndex < filePathLength;
		++fileNameIndex)
	{
		if (convertedFilePath[fileNameIndex] == '\\')
		{
			convertedFilePath[fileNameIndex] = '/';
		}
	}

	return convertedFilePath;
}
/*********************************************************************************/
char* K15_GetPathWithoutExtension(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	size_t filePathLength = strlen(p_FilePath);
	size_t extensionIndex = filePathLength;

	for(size_t filePathIndex = filePathLength;
		filePathLength > 0;
		--filePathLength)
	{
		if (p_FilePath[filePathIndex] == '.')
		{
			extensionIndex = filePathIndex;
		}
	}

	size_t filePathWithoutExtensionLength = extensionIndex;

	char* pathBuffer = (char*)K15_OS_MALLOC(filePathWithoutExtensionLength + 1);
	pathBuffer[filePathWithoutExtensionLength] = 0;
	memcpy(pathBuffer, p_FilePath, filePathWithoutExtensionLength);

	return pathBuffer;
}
/*********************************************************************************/