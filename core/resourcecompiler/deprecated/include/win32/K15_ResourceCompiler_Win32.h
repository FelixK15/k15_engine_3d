#ifndef _K15_ResourceCompiler_Win32_h_
#define _K15_ResourceCompiler_Win32_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "enums/K15_ResourceCompiler_FileSearchPattern.h"

namespace K15_ResourceCompiler
{
	struct ProcessorEnvironment;

	class Win32_FileFinder
	{
	public:

		static StringSet getFiles(const String& p_Directory, const String& p_Filter = "*.*", FileSearchPattern p_SearchPattern = FileSearchPattern::FSP_RECURSIVE);
		static bool fileExisting(const String& p_FilePath);
	};

}

#endif //_K15_ResourceCompiler_Win32_h_