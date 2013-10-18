/**
 * @file K15_StringUtil.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_System_StringUtil_h_
#define _K15Engine_System_StringUtil_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif

namespace K15_Engine { namespace System { 
	class K15_API_EXPORT StringUtil
	{
	public:
		
		/**
		 * Formats a string using printf syntax
		 *
		 * @param char * p_Message - message to format with printf placeholder
		 *
		 * @return String - Formatted String
		 */
		static String format(char* p_Message,...);
		
		/**
		 * Return the current time as string
		 *
		 * @param char * p_Format - format of the returned string
		 *							hh = hours
		 *							mm = minutes
		 *							ss = seconds
		 *							ms = milliseconds
		 * @note example: StringUtil::timeAsString("hh:mm:ss:ms") = "15:21:54:54"
		 *
		 * @return String - - Formatted string
		 */
		static String timeAsString(const String& p_Format = "hh:mm:ss");

		/**
		 * Returns the current date as string
		 *
		 * @param char * p_Format - format of the returned string
		 *							dd = day
		 *							yy = last 2 digits of the year
		 *							yyyy = year
		 *							mm = month
		 * @note example: StringUtil::dateAsString("dd.mm.yyyy") = "05.12.2010"
		 *
		 * @return String - Formatted string
		 */
		static String dateAsString(const String& p_Format = "dd/yyyy/mm");

		static String toString(int p_Value);

		static String toString(float p_Value, int p_Precision = 2);

	};//end of StringUtil class
}}//end of K15_Engine::System namespace

#endif //_K15Engine_System_StringUtil_h_