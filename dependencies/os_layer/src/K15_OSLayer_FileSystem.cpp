#include "K15_OSLayer_FileSystem.h"

/*********************************************************************************/
char* K15_GetWholeFileContent(const char* p_FilePath)
{
	return K15_GetWholeFileContentWithFileSize(p_FilePath, 0);
}
/*********************************************************************************/
char* K15_GetWholeFileContentWithFileSize(const char* p_FilePath, uint32* p_FileSize)
{
	assert(p_FilePath);

	uint32 fileSize = K15_GetFileSize(p_FilePath);
	char* fileContent = 0;

	if (fileSize > 0)
	{
		fileContent = (char*)malloc(fileSize + 1); //+1 for 0 terminator
		
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
	uint32 returnBufferSize = 0;

	if (!fileNameOnly)
	{
		returnBufferSize = strlen(convertedFilePath); //+1 for 0 terminator
		returnBuffer = (char*)malloc(returnBufferSize);
		memcpy(returnBuffer, convertedFilePath, returnBufferSize);
	}
	else
	{
		//directory seperator is part of the return of strrchr
		fileNameOnly++;

		uint32 filePathWithFileNameSize = strlen(convertedFilePath);
		uint32 fileNameSize = strlen(fileNameOnly);
		
		returnBufferSize = filePathWithFileNameSize - fileNameSize;
		returnBuffer = (char*)malloc(returnBufferSize + 1); //+1 for 0 terminator
		memcpy(returnBuffer, convertedFilePath, returnBufferSize);
	}

	returnBuffer[returnBufferSize] = 0; //0 terminator

	free((void*)convertedFilePath);

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
		uint32 returnBufferSize = strlen(p_FilePath);
		returnBuffer = (char*)malloc(returnBufferSize + 1); //+1 for 0 terminator

		memcpy(returnBuffer, p_FilePath, returnBufferSize);

		returnBuffer[returnBufferSize] = 0;  //0 terminator
	}
	else
	{
		uint32 sizeFilePath = strlen(p_FilePath);
		uint32 sizePath = strlen(pathWithoutFileName);
		uint32 returnBufferSize = sizeFilePath - sizePath;

		assert(returnBufferSize > 0);

		returnBuffer = (char*)malloc(returnBufferSize + 1); //+1 for 0 terminator

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

	uint32 fileNameLength = strlen(p_FilePath);

	for (uint32 fileNameIndex = fileNameLength;
		 fileNameIndex > 0;
		 --fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			fileNameWithoutExtension = (char*)malloc(fileNameIndex + 1); //+1 for 0 terminator
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

	uint32 fileNameLength = strlen(p_FilePath);

	for (uint32 fileNameIndex = 0;
		fileNameIndex > fileNameLength;
		++fileNameIndex)
	{
		if (fileName[fileNameIndex] == '.')
		{
			uint32 extensionLength = fileNameLength - fileNameIndex;

			assert(extensionLength > 0);

			extension = (char*)malloc(extensionLength + 1); //+1 for 0 terminator
			memcpy(extension, fileName + fileNameIndex, extensionLength);
			extension[extensionLength] = 0;
			break;
		}
	}

	return fileName;
}
/*********************************************************************************/

