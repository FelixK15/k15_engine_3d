/**
 * @file K15_LogBase.cpp
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

#include "K15_PrecompiledHeader.h"

#include "K15_LogBase.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	LogBase::LogBase()
	{

	}
	/*********************************************************************************/
	LogBase::LogBase(const String &p_Name)
		: m_Name(p_Name)
	{

	}
	/*********************************************************************************/
	LogBase::~LogBase()
	{

	}
	/*********************************************************************************/
	void LogBase::setLogBitMask(uint32 p_Mask)
	{
		m_LogBitMask = p_Mask;
	}
	/*********************************************************************************/
	void LogBase::toggleLogFlag(uint32 p_Flag)
	{

	}
	/*********************************************************************************/
	const uint32 LogBase::getLogBitMask() const
	{
		return m_LogBitMask;
	}
	/*********************************************************************************/
	const String& LogBase::getName() const
	{
		return m_Name;
	}
	/*********************************************************************************/
	void LogBase::setName(const String& p_Name) 
	{
		m_Name = p_Name;
	}
/*********************************************************************************/

}} //end of K15_Engine::System
