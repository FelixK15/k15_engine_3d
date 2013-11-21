/**
 * @file K15_HashedString.inl
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
unsigned long HashedString::getIdentifier() const
{
	return m_Hash;
}
/*********************************************************************************/
#ifndef K15_NO_STRINGS
const char* HashedString::c_str() const
{
	return m_String;
}
#endif //K15_NO_STRINGS
/*********************************************************************************/