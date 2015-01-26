#include "win32/K15_ResourceCompiler_Win32.h"

#include "K15_ResourceCompiler_ResourceProcessorEnvironment.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	StringSet Win32_FileFinder::getFiles(const String& p_Directory, const String& p_Filter, FileSearchPattern p_SearchPattern)
	{
		StringSet files;
		String currentDirectory = p_Directory;

		std::replace(currentDirectory.begin(), currentDirectory.end(), '\\', '/');

		if(currentDirectory.back() != '/')
		{
			currentDirectory.append("/");
		}

		String fileName = currentDirectory + p_Filter;

		WIN32_FIND_DATA findData = {};

		HANDLE searchHandle = FindFirstFile(fileName.c_str(), &findData);

		if(searchHandle == INVALID_HANDLE_VALUE)
		{
			return files;
		}

		do 
		{
			if(strcmp(findData.cFileName, ".") != 0
			   && strcmp(findData.cFileName, "..") != 0)
			{
				fileName = currentDirectory + findData.cFileName;

				if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
					&& p_SearchPattern == FileSearchPattern::FSP_RECURSIVE)
				{
					StringSet dirFiles = getFiles(fileName, p_Filter, p_SearchPattern);
					files.insert(dirFiles.begin(), dirFiles.end());
				}
				else
				{
					files.insert(fileName);
				}
			}
		} while (FindNextFile(searchHandle, &findData));

		FindClose(searchHandle);

		return files;
	}
	/*********************************************************************************/
	bool Win32_FileFinder::fileExisting(const String& p_FilePath)
	{
		WIN32_FIND_DATA findData = {0};

		HANDLE searchHandle = FindFirstFile(p_FilePath.c_str(), &findData);

		if(searchHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		FindClose(searchHandle);

		return true;
	}
	/*********************************************************************************/
}