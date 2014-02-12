/**
 * @file K15_StringUtil.inl
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

/*********************************************************************************/
template<typename T>
T StringUtil::stringToNumeric(const String& p_String,const unsigned char p_Delimiter /*= '.'*/)
{
	static String tempString;
	static T returnValue;
	StringStream stream;

	tempString.clear();

	for(String::size_type i = 0;i < p_String.size();++i)
	{
		if(isdigit(p_String.at(i)) || p_String.at(i) == p_Delimiter)
		{
			tempString += p_String.at(i);
		}
	}

	//insert string
	stream << tempString;
	//extract value
	stream >> returnValue;

	K15_ASSERT(!stream.fail(),format("Could not convert \"%s\" to numerical value.",p_String.c_str()));

	return returnValue;
}
/*********************************************************************************/
template<typename T>
String StringUtil::toString(const T& p_Value)
{
	StringStream stream;

	stream << p_Value;

	K15_ASSERT(!stream.fail(),"Could not convert to string.");

	return stream.str();
}
/*********************************************************************************/