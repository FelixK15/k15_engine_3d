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

#ifndef _K15Engine_Core_StringUtil_h_
#define _K15Engine_Core_StringUtil_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core { 
	class K15_CORE_API StringUtil
	{
	public:
		/*********************************************************************************/
		static const String EMPTY;
		/*********************************************************************************/
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

		/**
		* Create a string representing a float value
		*
		* @param  p_Value - float value
		* @param  p_Precision - decimal precision of the string 
		*
		* @return String - float value as string
		*/
		static String toString(float p_Value, int p_Precision = 2);

		/**
		* Removes all white spaces out of a given string and returns a new string without white spaces
		*
		* @param p_String - String to remove white spaces from
		*
		* @return String - String without white spaces
		*/
		static String removeWhitespaces(const String& p_String);

		/**
		* Transforms a string to a string with only capital letters
		*
		* @param p_String - Input string
		*
		* @return String - String with only capital letters
		*/
		static String toUpperString(const String& p_String);

		/**
		* Transforms a string to a string with only small letters
		*
		* @param p_String - Input string
		*
		* @return String - String with only small letters
		*/
		static String toLowerString(const String& p_String);

		/**
		* Create an integer variable out of a string
		* @note anything in that string except numbers will get ignored.
		*
		* @param  p_String - string containing the int
		*
		* @return int32 - extracted integer from the string
		*/
		static int32 toInt(const String& p_String);

		/**
		* Create an unsigned integer variable out of a string
		* @note anything in that string except numbers will get ignored.
		*
		* @param  p_String - string containing the int
		*
		* @return uint32 - extracted unsigned integer from the string
		*/
		static uint32 toUInt(const String& p_String);

		/**
		* Create a float variable out of a string
		* @note anything in that string except numbers and the delimiter will get ignored.
		*
		* @param  p_String - string containing the float
		*
		* @return float - extracted float from the string
		*/
		static float toFloat(const String& p_String,const unsigned char p_Delimiter = '.');

		/**
		* Create a double variable out of a string
		* @note anything in that string except numbers and the delimiter will get ignored.
		*
		* @param  p_String - string containing the double
		*
		* @return double - extracted double from the string
		*/
		static double toDouble(const String& p_String,const unsigned char p_Delimiter = '.');

		/**
		* Create a bool variable out of a string
		* @note anything except "true", "false", "1" and "0" will get ignored
		*
		* @param  p_String - string containing the bool
		*
		* @return bool - extracted bool from the string
		*/
		static bool toBool(const String& p_String);

		/**
		* Creates a numeric variable out of a string
		* @note anything in that string except numbers and the delimiter will get ignored.
		*
		* @param  p_String - string containing the numeric value
		*
		* @return T - extracted numeric value
		*/
		template<typename T>
		static T stringToNumeric(const String& p_String,const unsigned char p_Delimiter = '.');

		/**
		* Creates a string out of a given object
		*
		* @param  p_Value - value that should be converted to string
		*
		* @return String - value as string
		*/
		template<typename T>
		static String toString(const T& p_Value);

	};//end of StringUtil class
#	include "K15_StringUtil.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_StringUtil_h_