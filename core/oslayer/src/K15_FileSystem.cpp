#include "K15_FileSystem.h"

#include "K15_DefaultCLibraries.h"

/*********************************************************************************/
char* K15_GetWholeFileContent(const char* p_FilePath)
{
	return K15_GetWholeFileContentWithFileSize(p_FilePath, 0);
}
/*********************************************************************************/
char* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize)
{
	K15_ASSERT_TEXT(p_FilePath, "Filepath is NULL.");

	uint32 fileSize = K15_GetFileSize(p_FilePath);
	char* fileContent = 0;

	if (fileSize > 0)
	{
		fileContent = (char*)K15_OS_MALLOC(fileSize + 1); //+1 for 0 terminator
		
		FILE* file = fopen(p_FilePath, "rb");
		size_t amountRead = fread(fileContent, sizeof(char), fileSize, file);
		fclose(file);

		fileContent[amountRead] = 0; //0 terminator

		if (p_FileSize)
		{
			*p_FileSize = (uint32)amountRead;
		}
	}

	return fileContent;
}
/*********************************************************************************/
char* K15_GetPathWithoutFileName(const char* p_FilePath)
{
	assert(p_FilePath);

	const char* convertedFilePath = K15_ConvertToSystemPath(p_FilePath);
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

	K15_OS_FREE((void*)convertedFilePath);

	return returnBuffer;
}
/*********************************************************************************/
char* K15_GetFileName(const char* p_FilePath)
{
	assert(p_FilePath);

	const char* pathWithoutFileName = strchr(p_FilePath, K15_DIR_SEPARATOR);
	
	char* returnBuffer = 0;

	if (!pathWithoutFileName)
	{
		size_t returnBufferSize = strlen(p_FilePath);
		returnBuffer = (char*)K15_OS_MALLOC(returnBufferSize + 1); //+1 for 0 terminator

		memcpy(returnBuffer, p_FilePath, returnBufferSize);

		returnBuffer[returnBufferSize] = 0;  //0 terminator
	}
	else
	{
		size_t sizeFilePath = strlen(p_FilePath);
		size_t sizePath = strlen(pathWithoutFileName);
		size_t returnBufferSize = sizeFilePath - sizePath;

		assert(returnBufferSize > 0);

		returnBuffer = (char*)K15_OS_MALLOC(returnBufferSize + 1); //+1 for 0 terminator

		memcpy(returnBuffer, p_FilePath + sizePath, returnBufferSize);

		returnBuffer[returnBufferSize] = 0; //0 terminator
	}


	return returnBuffer;
}
/*********************************************************************************/
char* K15_GetFileNameWithoutExtension(const char* p_FilePath)
{
	char* fileName = K15_GetFileName(p_FilePath);
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
	char* fileName = K15_GetFileName(p_FilePath);
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

