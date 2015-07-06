#ifndef _K15_ResourceCompiler_Logging_h_
#define _K15_ResourceCompiler_Logging_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "enums/K15_ResourceCompiler_LogPriority.h"

#define LOG_NORMAL(msg, ...) 	K15_ResourceCompiler::Logging::getInstance().writeMessage(LogPriority::LP_Normal, msg, __VA_ARGS__);
#define LOG_DEBUG(msg, ...) 	K15_ResourceCompiler::Logging::getInstance().writeMessage(LogPriority::LP_Debug, msg, __VA_ARGS__);
#define LOG_WARNING(msg, ...) 	K15_ResourceCompiler::Logging::getInstance().writeMessage(LogPriority::LP_Warning, msg, __VA_ARGS__);
#define LOG_ERROR(msg, ...) 	K15_ResourceCompiler::Logging::getInstance().writeMessage(LogPriority::LP_Error, msg, __VA_ARGS__);

namespace K15_ResourceCompiler
{
	class Logging
	{	
		K15_NON_COPYABLE(Logging);
		K15_NON_MOVEABLE(Logging);

	public:
		static Logging& getInstance();

	public:
		~Logging();

		void enableFileLogging(const String& p_FileName);
		void writeMessage(LogPriority p_Priority, const char* p_Message, ...);

	private:
		Logging();
		String _timeAsString();

	private:
		OutputFileStream m_FileStream;
		bool m_LogToFile;
	};
}

#endif //_K15_ResourceCompiler_Logging_h_
