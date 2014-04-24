/**
 * @file K15_ResourceFileBase.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/15
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
bool ResourceFileBase::isOpen() const
{
	return m_isOpen;
}
/*********************************************************************************/
const String& ResourceFileBase::getFileName() const
{
	return m_FileName;
}
/*********************************************************************************/
const String& ResourceFileBase::getError() const
{
	return m_Error;
}
/*********************************************************************************/
void ResourceFileBase::setError(const String& p_Error)
{
	m_Error = p_Error;
}
/*********************************************************************************/