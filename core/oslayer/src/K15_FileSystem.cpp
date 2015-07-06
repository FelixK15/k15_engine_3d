#include "K15_FileSystem.h"

#include "K15_String.h"
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
	byte* fileContent = 0;

	if (fileSize > 0)
	{
		fileContent = (byte*)K15_OS_MALLOC(fileSize + 1);
		fileContent = K15_CopyWholeFileContentIntoBuffer(p_FilePath, fileContent);

		if (p_FileSize)
		{
			*p_FileSize = fileSize;
		}
	}

	return fileContent;
}
/*********************************************************************************/
bool8 K15_IsAbsolutePath(const char* p_FilePath)
{
	return !K15_IsRelativePath(p_FilePath);
}
/*********************************************************************************/
char* K15_GetPathWithoutFileName(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	char* convertedFilePath = K15_ConvertToSystemPath(p_FilePath);
	const char* fileNameOnly = strrchr(convertedFilePath, K15_DIR_SEPARATOR);

	char* returnBuffer = 0;
	uint32 returnBufferSize = 0;

	if (!fileNameOnly)
	{
		returnBufferSize= (uint32)strlen(convertedFilePath); //+1 for 0 terminator
		returnBuffer = (char*)K15_OS_MALLOC(returnBufferSize);
		memcpy(returnBuffer, convertedFilePath, returnBufferSize);
	}
	else
	{
		//directory seperator is part of the return of strrchr
		fileNameOnly++;

		uint32 filePathWithFileNameSize= (uint32)strlen(convertedFilePath);
		uint32 fileNameSize= (uint32)strlen(fileNameOnly);

		if (fileNameSize == 0)
		{
			K15_OS_FREE(convertedFilePath);
			return 0;
		}
		
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

	uint32 fileNameLength= (uint32)strlen(fileNameOnly);
	uint32 offset = 0;

	if (fileNameOnly[0] == K15_DIR_SEPARATOR)
	{
		fileNameLength -= 1;
		offset = 1;
	}

	if (fileNameLength == 0)
	{
		return 0;
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

	size_t fileNameLength = strlen(fileName);

	for (size_t fileNameIndex = fileNameLength;
		 fileNameIndex > 0;
		 --fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			fileNameWithoutExtension = (char*)K15_OS_MALLOC(fileNameIndex + 1); //+1 for 0 terminator
			memcpy(fileNameWithoutExtension, fileName, fileNameIndex);
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

	uint32 fileNameLength= (uint32)strlen(p_FilePath);

	for (uint32 fileNameIndex = 0;
		fileNameIndex > fileNameLength;
		++fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			uint32 extensionLength = fileNameLength - fileNameIndex;

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
char* K15_ConvertToDirectoryPath(const char* p_String)
{
	K15_ASSERT_TEXT(p_String, "String is NULL.");

	uint32 stringLength= (uint32)strlen(p_String);

	//last char equals dir separator?
	if (p_String[stringLength-1] != '/' && 
		p_String[stringLength-1] != '\\')
	{
		stringLength += 1;
	}

	char* copyString = K15_CopyStringIntoBuffer(p_String, (char*)alloca(stringLength + 1), stringLength);

	copyString[stringLength-1] = K15_DIR_SEPARATOR;

	return K15_ConvertToSystemPath(copyString);
}
/*********************************************************************************/
char* K15_ConvertToAbsolutePath(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	const char* workingDir = K15_GetWorkingDirectory();
	uint32 workingDirLength= (uint32)strlen(workingDir);
	uint32 filePathLength= (uint32)strlen(p_FilePath);
	uint32 absolutPathLength = workingDirLength + filePathLength;

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

	uint32 filePathLength= (uint32)strlen(p_FilePath);
	char* convertedFilePath = (char*)K15_OS_MALLOC(filePathLength + 1); //+1 for 0 terminator
	memcpy(convertedFilePath, p_FilePath, filePathLength);
	convertedFilePath[filePathLength] = 0;

	return K15_ConvertToSystemPathInplace(convertedFilePath);
}
/*********************************************************************************/
char* K15_ConvertToSystemPathInplace(char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	char* convertedFilePath = p_FilePath;

	while(*p_FilePath)
	{
		if (*p_FilePath == '\\')
		{
			*p_FilePath = '/';
		}

		++p_FilePath;
	}

	return convertedFilePath;
}
/*********************************************************************************/
char* K15_GetPathWithoutExtension(const char* p_FilePath)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	uint32 filePathLength= (uint32)strlen(p_FilePath);
	uint32 extensionIndex = filePathLength;

	for(uint32 filePathIndex = filePathLength;
		filePathLength > 0;
		--filePathLength)
	{
		if (p_FilePath[filePathIndex] == '.')
		{
			extensionIndex = filePathIndex;
		}
	}

	uint32 filePathWithoutExtensionLength = extensionIndex;

	char* pathBuffer = (char*)K15_OS_MALLOC(filePathWithoutExtensionLength + 1);
	pathBuffer[filePathWithoutExtensionLength] = 0;
	memcpy(pathBuffer, p_FilePath, filePathWithoutExtensionLength);

	return pathBuffer;
}
/*********************************************************************************/