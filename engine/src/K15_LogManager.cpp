/**
 * @file K15_LogManager.cpp
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

#include "K15_LogManager.h"
#include "K15_LogBase.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	LogManager::LogManager()
		:	StackAllocator(MemoryAllocator,MEGABYTE,_N(LogManagerAllocator)),
			m_DefaultLog(0),
			m_Logs()
	{

	}
	/*********************************************************************************/
	LogManager::~LogManager()
	{
		for(LogList::iterator iter = m_Logs.begin();iter != m_Logs.end();++iter)
		{
			K15_DELETE (*iter);
		}

		m_Logs.clear();
	}
	/*********************************************************************************/
	void LogManager::addLog(LogBase* p_Log, bool p_DefaultLog, uint32 p_LogMask)
	{
		if(p_Log)
		{
			p_Log->setLogBitMask(p_LogMask);
			if(p_DefaultLog)
			{
				setDefaultLog(p_Log);
			}

			m_Logs.push_front(p_Log);
		}
	}
	/*********************************************************************************/
	void LogManager::logMessage(const String& p_LogMessage, bool p_DefaultLogOnly, Enum p_LogFlag)
	{
		if(p_DefaultLogOnly)
		{
			if(m_DefaultLog)
			{
				m_DefaultLog->logMessage(p_LogMessage,(Enum)p_LogFlag);
			}
		}
		else
		{
			LogBase* currentLog = 0;
			for(LogList::iterator iter = m_Logs.begin();iter != m_Logs.end();++iter)
			{
				currentLog = (*iter);
				if(currentLog->getLogBitMask() & p_LogFlag)
				{
					(*iter)->logMessage(p_LogMessage,p_LogFlag);
				}
			}
		}
	}
	/*********************************************************************************/
	void LogManager::setDefaultLog(LogBase* p_Log)
	{
		m_DefaultLog = p_Log;
	}
	/*********************************************************************************/
	LogBase* LogManager::getDefaultLog() const
	{
		return m_DefaultLog;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace