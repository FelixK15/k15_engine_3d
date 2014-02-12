/**
 * @file K15_StringUtil.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_PrecompiledHeader.h"

#include "K15_StringUtil.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const String StringUtil::EMPTY = "";
	/*********************************************************************************/

	/*********************************************************************************/
	String StringUtil::format(char* p_Message,...)
	{
		static const uint32 BufferSize = 2048;
		static char MessageBuffer[BufferSize] = {0};

		va_list list;

		va_start(list,p_Message);
		vsprintf(MessageBuffer,p_Message,list);
		va_end(list);

		return MessageBuffer;
	}
	/*********************************************************************************/
	String StringUtil::timeAsString(const String& p_Format /*= "hh:mm:ss"*/)
	{
		String timeString;
		String::size_type pos;

		pos = 0;
		timeString = p_Format;

		time_t t(time(0));
		tm tm(*localtime(&t));

		//hours
		pos = p_Format.find_first_of("hh");
		if(pos != String::npos)
		{
			static String hour = "";
			hour = toString(tm.tm_hour);
			if(tm.tm_hour < 10)
			{
				hour = "0" + hour;
			}
			timeString.replace(pos,2,hour);
		}
		
		//minutes
		pos = p_Format.find_first_of("mm");
		if(pos != String::npos)
		{
			static String minute = "";
			minute = toString(tm.tm_min);
			if(tm.tm_min < 10)
			{
				minute = "0" + minute;
			}
			timeString.replace(pos,2,minute);
		}
		
		//seconds
		pos = p_Format.find_first_of("ss");
		if(pos != String::npos)
		{
			static String second = "";
			second = toString(tm.tm_sec);
			if(tm.tm_sec < 10)
			{
				second = "0" + second;
			}
			timeString.replace(pos,2,second);
		}

		return timeString;
	}
	/*********************************************************************************/
	String StringUtil::dateAsString(const String& p_Format)
	{
		String dateString;
		String::size_type pos;

		pos = 0;
		dateString = p_Format;

		time_t t(time(0));
		tm tm(*localtime(&t));

		//days
		pos = p_Format.find_first_of("dd");
		if(pos != String::npos)
		{
			dateString.replace(pos,2,toString(tm.tm_mday));
		}

		//months
		pos = p_Format.find_first_of("mm");
		if(pos != String::npos)
		{
			dateString.replace(pos,2,toString(tm.tm_mon));
		}

		//year
		pos = p_Format.find_first_of("yyyy");
		if(pos != String::npos)
		{
			dateString.replace(pos,4,toString(tm.tm_year));
		}

		//year - short
		pos = p_Format.find_first_of("yy");
		if(pos != String::npos)
		{
			dateString.replace(pos,2,toString(tm.tm_year).substr(2,2));
		}

		return dateString;
	}
	/*********************************************************************************/
	String StringUtil::toString(float p_Value, int p_Precision)
	{
		StringStream stream;

		stream.precision(p_Precision);

		stream << p_Value;

		return stream.str();
	}
	/*********************************************************************************/
	int32 StringUtil::toInt(const String& p_String)
	{
		return stringToNumeric<int32>(p_String);
	}
	/*********************************************************************************/
	uint32 StringUtil::toUInt(const String& p_String)
	{
		return stringToNumeric<uint32>(p_String);
	}
	/*********************************************************************************/
	float StringUtil::toFloat(const String& p_String,const unsigned char p_Delimiter)
	{
		return stringToNumeric<float>(p_String,p_Delimiter);
	}
	/*********************************************************************************/
	double StringUtil::toDouble(const String& p_String,const unsigned char p_Delimiter)
	{
		return stringToNumeric<double>(p_String,p_Delimiter);
	}
	/*********************************************************************************/
	bool StringUtil::toBool(const String& p_String)
	{
		return p_String.find_first_of("true") != String::npos || p_String.find_first_of("1") != String::npos;
	}
	/*********************************************************************************/
	String StringUtil::removeWhitespaces(const String& p_String)
	{
		String string;
		string = p_String;
		string.erase(std::remove_if(string.begin(),string.end(),isspace),string.end());
		return string;
	}
	/*********************************************************************************/
	String StringUtil::toUpperString(const String& p_String)
	{
		String string;
		string = p_String;
		std::transform(string.begin(),string.end(),string.begin(),::toupper);
		return string;
	}
	/*********************************************************************************/
	String StringUtil::toLowerString(const String& p_String)
	{
		String string;
		string = p_String;
		std::transform(string.begin(),string.end(),string.begin(),::tolower);
		return string;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace