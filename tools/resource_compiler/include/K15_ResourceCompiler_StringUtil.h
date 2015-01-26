#ifndef _K15_ResourceCompiler_StringUtil_h_
#define _K15_ResourceCompiler_StringUtil_h_

#include "K15_ResourceCompiler_Prerequisites.h"

namespace K15_ResourceCompiler
{
	struct ProcessorEnvironment;

	class StringUtil
	{
		K15_NO_DEFAULT_CONSTRUCTOR(StringUtil);
		K15_NON_COPYABLE(StringUtil);
		K15_NON_MOVEABLE(StringUtil);

	public:
		static String getFileNameWithoutExtension(const String& p_FileName);

		static String toPath(const String& p_FilePath);

		static String getFileExtension(const String& p_FileName);

		static String generateExpectingOutputFilePath(const String& p_InputFilePath, const ProcessorEnvironment& p_ProcessorSettings);

		static String getFileName(const String& p_Path);

	}; // StringUtil class declaration
} // end of K15_ResourceCompiler namespace

#endif //_K15_ResourceCompiler_StringUtil_h_