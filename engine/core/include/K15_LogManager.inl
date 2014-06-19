/**
 * @file K15_LogManager.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/17
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
LogManager* LogManager::getInstance()
{
	return Instance;
}
/*********************************************************************************/
LogBase* LogManager::getDefaultLog()
{
	return m_DefaultLog;
}
/*********************************************************************************/
void LogManager::setDefaultLog(LogBase* p_Log)
{
	m_DefaultLog = p_Log;
}
/*********************************************************************************/
