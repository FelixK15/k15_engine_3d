#include "K15_ResourceCompiler_Logging.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	Logging& Logging::getInstance()
	{
		static Logging logging;

		return logging;
	}
	/*********************************************************************************/
	Logging::Logging()
		: m_FileStream(),
		m_LogToFile(false)
	{

	}
	/*********************************************************************************/
	Logging::~Logging()
	{
		if(m_LogToFile)
		{
			m_FileStream.flush();
			m_FileStream.close();
		}
	}
	/*********************************************************************************/
	void Logging::enableFileLogging(const String& p_FileName)
	{
		m_FileStream.open(p_FileName);

		if((m_LogToFile = m_FileStream.is_open()) == false)
		{
			LOG_ERROR("Could not create file log \"%s\".", p_FileName.c_str());
		}
	}
	/*********************************************************************************/
	void Logging::writeMessage(LogPriority p_Priority, const char* p_Message, ...)
	{
		static const uint32 BufferSize = 2048;
		char MessageBuffer[BufferSize] = {0};

		va_list list;

		va_start(list, p_Message);
		vsprintf(MessageBuffer, p_Message, list);
		va_end(list);

		String messageAsString = _timeAsString() + " : " + MessageBuffer;
		messageAsString += "\n";

		if(m_LogToFile)
		{
			m_FileStream.write(messageAsString.data(), messageAsString.size());
		}
		else
		{
			printf(messageAsString.c_str());
		}
	}
	/*********************************************************************************/
	String Logging::_timeAsString()
	{
		String timeString;
		String::size_type pos;
		char numberBuffer[3];
		pos = 0;

		time_t t(time(0));
		tm tm(*localtime(&t));

		itoa(tm.tm_hour, numberBuffer, 10);

		//hours
		String hour = "";
		hour = numberBuffer;
		if(tm.tm_hour < 10)
		{
			hour = "0" + hour;
		}
		timeString += hour + ":";
		
		//minutes
		String minute = "";
		itoa(tm.tm_min, numberBuffer, 10);
		minute = numberBuffer;
		if(tm.tm_min < 10)
		{
			minute = "0" + minute;
		}
		timeString += minute + ":";
		
		//seconds
		String second = "";
		itoa(tm.tm_sec, numberBuffer, 10);
		second = numberBuffer;
		if(tm.tm_sec < 10)
		{
			second = "0" + second;
		}
		timeString += second;

		return timeString;
	}
	/*********************************************************************************/
}