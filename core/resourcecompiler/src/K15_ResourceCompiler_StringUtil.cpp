#include "K15_ResourceCompiler_StringUtil.h"
#include "K15_ResourceCompiler_ResourceProcessorEnvironment.h"


namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	String StringUtil::getFileNameWithoutExtension(const String& p_FileName)
	{
		size_t charsUntilSlash = p_FileName.find_last_of('/');
		size_t lengthFileName = p_FileName.find_last_of('.');

		if(charsUntilSlash != String::npos)
		{
			//p_FileName is path - extract file name
			charsUntilSlash += 1;
			lengthFileName = lengthFileName - charsUntilSlash;
		}

		return p_FileName.substr(charsUntilSlash == String::npos ? 0 : charsUntilSlash, lengthFileName);
	}
	/*********************************************************************************/
	String StringUtil::toPath(const String& p_FilePath)
	{
		String path = p_FilePath;

		if(path.size() > 0)
		{
			if(path[path.size() - 1] != '/')
			{
				path.append("/");
			}
		}

		return path;
	}
	/*********************************************************************************/
	String StringUtil::getFileExtension(const String& p_FileName)
	{
		String extension;

		if(!p_FileName.empty())
		{
			extension = p_FileName.substr(p_FileName.find_last_of('.'));
		}

		return extension;
	}
	/*********************************************************************************/
	String StringUtil::generateExpectingOutputFilePath(const String& p_InputFilePath, const ProcessorEnvironment& p_ProcessorSettings)
	{
		String outputFilePath = "";
		size_t beginningExistingFileExtension = p_InputFilePath.find_last_of('.');

		if(beginningExistingFileExtension == String::npos)
		{
			return p_InputFilePath;
		}

		String expectedFileExtension = p_ProcessorSettings.outputExtension;
		
		outputFilePath = p_InputFilePath.substr(0, p_InputFilePath.find_last_of('.'));
		outputFilePath.append(expectedFileExtension);

		return outputFilePath;
	}
	/*********************************************************************************/
	String StringUtil::getFileName(const String& p_Path)
	{
		size_t lastSlash = p_Path.find_last_of('/');

		if(lastSlash == String::npos)
		{
			lastSlash = 0;
		}

		return p_Path.substr(lastSlash, p_Path.size() - lastSlash);
	}	
	/*********************************************************************************/
}