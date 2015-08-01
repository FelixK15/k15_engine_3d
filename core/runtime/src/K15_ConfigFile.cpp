#include "K15_ConfigFile.h"
#include "K15_DefaultCLibraries.h"
#include "K15_FileSystem.h"
#include "K15_Logging.h"
#include "K15_String.h"

#include "K15_CustomMemoryAllocator.h"

#include "generated/K15_ConfigValueBuffer.cpp"

/*********************************************************************************/
intern inline uint32 K15_InternalGetEqualPosition(char* p_ConfigLine, uint32 p_ConfigLineLength)
{
	uint32 equalPosition = 0;

	for (uint32 configLineCharIndex = 0;
		configLineCharIndex < p_ConfigLineLength;
		++configLineCharIndex)
	{
		if (p_ConfigLine[configLineCharIndex] == '=')
		{
			equalPosition = configLineCharIndex;
			break;
		}
	}

	return equalPosition;
}
/*********************************************************************************/
intern inline uint8 K15_InternalIsCategoryConfigLine(char* p_ConfigLine)
{
	uint8 result = K15_SUCCESS;
	uint8 foundEqual = K15_FALSE;
	uint8 foundBracketLeft = K15_FALSE;
	uint8 foundBracketRight = K15_FALSE;

	uint32 lineSize= (uint32)strlen(p_ConfigLine);

	for (uint32 lineCharIndex = 0 ;
		lineCharIndex < lineSize;
		++lineCharIndex)
	{
		char currentLineChar = p_ConfigLine[lineCharIndex];

		if (currentLineChar == '=')
		{
			foundEqual = K15_TRUE;
			break;
		}

		if (currentLineChar == '[')
		{
			foundBracketLeft = K15_TRUE;
		}

		if (currentLineChar == ']')
		{
			foundBracketRight = K15_TRUE;
		}
	}

	return foundEqual == K15_FALSE &&
			(foundBracketLeft == K15_TRUE &&
			 foundBracketRight == K15_TRUE);
}
/*********************************************************************************/
intern inline char* K15_InternalGetCategoryName(char* p_ConfigLine)
{
	uint32 nameLength= (uint32)strlen(p_ConfigLine) - 1; //-2 (brackets) + 1 (0 terminator)
	uint32 nameIndex = 0;
	char* name = (char*)malloc(nameLength);
	name[nameLength] = 0;

	for (size_t nameCharIndex = 0;
		nameCharIndex < nameLength;
		++nameCharIndex)
	{
		char nameChar = p_ConfigLine[nameCharIndex];

		if (nameChar != '[' && nameChar != ']')
		{
			name[nameIndex++] = nameChar;
		}
	}

	return name;
}
/*********************************************************************************/
intern inline uint8 K15_InternalIsValidConfigLine(char* p_ConfigLine)
{
	uint8 foundRightValue = K15_FALSE;
	uint8 foundLeftValue = K15_FALSE;
	uint8 foundEqual = K15_FALSE;
	uint32 lineLength= (uint32)strlen(p_ConfigLine);


	for (size_t lineCharIndex = 0;
		 lineCharIndex < lineLength;
		 ++lineCharIndex)
	{
		char currentLineChar = p_ConfigLine[lineCharIndex];

		if (currentLineChar == '='
			&& foundEqual == K15_FALSE)
		{
			foundEqual = K15_TRUE;
		}
		else if(isspace(currentLineChar) == 0)
		{
			if (foundEqual == K15_TRUE)
			{
				foundRightValue = K15_TRUE;
			}
			else
			{
				foundLeftValue = K15_TRUE;
			}
		}
	}

	return foundEqual == K15_TRUE &&
		   foundLeftValue == K15_TRUE &&
		   foundRightValue == K15_TRUE;
}
/*********************************************************************************/
intern inline char* K15_InternalGetConfigLineValue(char* p_ConfigLine)
{
	uint32 equalIndex = 0;
	uint32 lineLength= (uint32)strlen(p_ConfigLine);

	uint32 equalPosition = K15_InternalGetEqualPosition(p_ConfigLine, lineLength);

	char* value = 0;

	if(equalPosition != 0)
	{
		equalPosition += 1; //we don't want exactly the equal sign

		uint32 valueSize = lineLength - equalPosition;
		value = (char*)malloc(valueSize + 1);
		value[valueSize] = 0;

		memcpy(value, p_ConfigLine + equalPosition, valueSize);
	}

	return value;
}
/*********************************************************************************/
intern inline char* K15_InternalGetConfigLineName(char* p_ConfigLine)
{
	uint32 equalIndex = 0;
	uint32 lineLength= (uint32)strlen(p_ConfigLine);

	uint32 equalPosition = K15_InternalGetEqualPosition(p_ConfigLine, lineLength);

	char* name = 0;

	if(equalPosition != 0)
	{
		uint32 nameSize = equalPosition;
		name = (char*)malloc(nameSize + 1);
		name[nameSize] = 0;

		memcpy(name, p_ConfigLine, nameSize);
	}

	return name;
}
/*********************************************************************************/
intern inline char* K15_InternalGetValueFromConfigStretchBuffer(K15_ConfigValueStretchBuffer* p_ConfigValues, const char* p_Name)
{
	char* value = 0;
	uint32 numElements = p_ConfigValues->numElements;

	for (uint32 configValueIndex = 0;
		configValueIndex < numElements;
		++configValueIndex)
	{
		K15_ConfigValue* currentConfigValue = K15_GetConfigValueStretchBufferElement(p_ConfigValues, configValueIndex);

		if (strcmp(currentConfigValue->name, p_Name) == 0)
		{
			value = currentConfigValue->value;
			break;
		}
	}

	return value;
}
/*********************************************************************************/
uint8 K15_LoadConfigFile(const char* p_ConfigFile, K15_ConfigFileContext* p_ConfigFileContext)
{
	K15_ASSERT_TEXT(p_ConfigFile, "Config File is NULL.");
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");

	K15_ConfigValueStretchBuffer* configStretchBuffer = &p_ConfigFileContext->configValues;

	//create stretch buffer for config values
	if (configStretchBuffer->elements)
	{
		//delete old values
		K15_DeleteConfigValueStretchBuffer(configStretchBuffer);
	}
	
	//create new stretch buffer
	K15_CreateConfigValueStretchBuffer(configStretchBuffer);
	char* configFilePath = 0;

	if (K15_IsRelativePath(p_ConfigFile))
	{
		const char* workingDirectory = K15_GetWorkingDirectory();

		uint32 configFileNameLength= (uint32)strlen(p_ConfigFile);
		uint32 workingDirectoryNameLength= (uint32)strlen(workingDirectory);
		uint32 lengthPathName = configFileNameLength + workingDirectoryNameLength;

		configFilePath = (char*)malloc(lengthPathName);

		sprintf(configFilePath, "%s%s", workingDirectory, p_ConfigFile);
	}
	else
	{
		configFilePath = K15_CopyString(p_ConfigFile);
	}

	uint32 configFileSize = 0;
	char* configFileContent = (char*)K15_GetWholeFileContentWithFileSize(configFilePath, &configFileSize);

	if (!configFileContent)
	{
		K15_LOG_ERROR_MESSAGE("File '%s' is empty.", configFilePath);
		return K15_OS_ERROR_EMPTY_FILE;
	}

	configFileContent[configFileSize] = 0;

	p_ConfigFileContext->path = (char*)configFilePath;

	//chop config file into lines
	char* currentValue = 0;
	char* currentName = 0;
	char* currentCategory = 0;

	uint32 lineCount = K15_GetLinesInplace(configFileContent, 0);
	char** configLines = (char**)alloca(sizeof(char*) * lineCount);

	K15_GetLinesInplace(configFileContent, configLines);

	for (uint32 lineIndex = 0;
		lineIndex < lineCount;
		++lineIndex)
	{
		char* currentLine = configLines[lineIndex];

		if (K15_InternalIsCategoryConfigLine(currentLine))
		{
			currentCategory = K15_InternalGetCategoryName(currentLine);
		}
		else if (K15_InternalIsValidConfigLine(currentLine))
		{
			currentValue = K15_InternalGetConfigLineValue(currentLine);
			currentName = K15_InternalGetConfigLineName(currentLine);
		}
		else
		{
			K15_LOG_WARNING_MESSAGE("Could not parse line '%s' from config file '%s'.", currentLine, p_ConfigFile);
		}

		if (currentValue 
			&& currentName)
		{
			K15_ConfigValue currentConfigValue;
			currentConfigValue.name = currentName;
			currentConfigValue.value = currentValue;
			currentConfigValue.category = currentCategory;

			//add config value to stretch buffer
			K15_PushConfigValueStretchBufferElement(configStretchBuffer, currentConfigValue);

			currentName = 0;
			currentValue = 0;
		}
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint32 K15_GetNumConfigValuesForCategory(K15_ConfigFileContext* p_ConfigFileContext, const char* p_Category)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_Category, "Category Name is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	uint32 categoryElements = 0;

	for (uint32 elementIndex = 0;
		elementIndex < configValueStretchBuffer->numElements;
		++elementIndex)
	{
		K15_ConfigValue* configValue = K15_GetConfigValueStretchBufferElement(configValueStretchBuffer, elementIndex);

		if (configValue->category)
		{
			if (strcmp(configValue->category, p_Category) == 0)
			{
				++categoryElements;
			}
		}
	}

	return categoryElements;
}
/*********************************************************************************/
void K15_CopyCategoryConfigValuesIntoBuffer(K15_ConfigFileContext* p_ConfigFileContext, const char* p_Category, K15_ConfigValue** p_ConfigValueBuffer)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_Category, "Category Name is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueBuffer, "Config value buffer is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	uint32 configValueBufferIndex = 0;

	for (uint32 elementIndex = 0;
		elementIndex < configValueStretchBuffer->numElements;
		++elementIndex)
	{
		K15_ConfigValue* configValue = K15_GetConfigValueStretchBufferElement(configValueStretchBuffer, elementIndex);

		if (strcmp(configValue->category, p_Category) == 0)
		{
			p_ConfigValueBuffer[configValueBufferIndex++] = configValue;
		}
	}
}
/*********************************************************************************/
float K15_GetConfigValueAsFloat(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, float p_DefaultValue)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueName, "Config Vale Name is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	char* elementValue = K15_InternalGetValueFromConfigStretchBuffer(configValueStretchBuffer, p_ConfigValueName);
	float returnValue = p_DefaultValue;

	if (elementValue)
	{
		returnValue = (float)atof(elementValue);
	}

	return returnValue;
}
/*********************************************************************************/
int K15_GetConfigValueAsInt(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, int p_DefaultValue)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueName, "Config Vale Name is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	char* elementValue = K15_InternalGetValueFromConfigStretchBuffer(configValueStretchBuffer, p_ConfigValueName);
	int returnValue = p_DefaultValue;

	if (elementValue)
	{
		returnValue = atoi(elementValue);
	}

	return returnValue;
}
/*********************************************************************************/
bool8 K15_GetConfigValueAsBool(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, bool8 p_DefaultValue)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueName, "Config Vale Name is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	char* elementValue = K15_InternalGetValueFromConfigStretchBuffer(configValueStretchBuffer, p_ConfigValueName);
	bool8 returnValue = p_DefaultValue;

	if (elementValue)
	{
		if (strcmp(elementValue, "true") == 0)
		{
			returnValue = K15_TRUE;
		}
		else
		{
			returnValue = K15_FALSE;
		}
	}

	return returnValue;
}
/*********************************************************************************/
char* K15_GetConfigValueAsString(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, char* p_DefaultValue)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueName, "Config Vale Name is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	char* elementValue = K15_InternalGetValueFromConfigStretchBuffer(configValueStretchBuffer, p_ConfigValueName);
	char* elementValueBuffer = p_DefaultValue;

	if (elementValue)
	{
		uint32 valueLength= (uint32)strlen(elementValue);
		elementValueBuffer = (char*)malloc(valueLength + 1);
		elementValueBuffer[valueLength] = 0;

		memcpy(elementValueBuffer, elementValue, valueLength);
	}	

	return elementValueBuffer;
}
/*********************************************************************************/
void K15_GetConfigValueAsResolution(K15_ConfigFileContext* p_ConfigFileContext, const char* p_ConfigValueName, int* p_Width, int* p_Height, int p_DefaultWidth, int p_DefaultHeight)
{
	K15_ASSERT_TEXT(p_ConfigFileContext, "Config File Context is NULL.");
	K15_ASSERT_TEXT(p_ConfigValueName, "Config Vale Name is NULL.");
	K15_ASSERT_TEXT(p_Width, "width pointer is NULL.");
	K15_ASSERT_TEXT(p_Height, "height pointer is NULL.");

	K15_ConfigValueStretchBuffer* configValueStretchBuffer = &p_ConfigFileContext->configValues;
	char* elementValue = K15_InternalGetValueFromConfigStretchBuffer(configValueStretchBuffer, p_ConfigValueName);

	*p_Width = p_DefaultWidth;
	*p_Height = p_DefaultHeight;

	if (elementValue)
	{
		char* width = 0;
		char* height = 0;

		width = strtok(elementValue, "x");
		
		if (width)
		{
			height = strtok(0, "x");
		}

		if (width && height)
		{
			*p_Width = atoi(width);
			*p_Height = atoi(height);
		}
	}	
}
/*********************************************************************************/