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

#include "K15_LogManager.h"

using namespace K15_EngineV2;

LogManager::LogManager()
	: m_pDefaultLog(NULL)
{
	
}

LogManager::~LogManager()
{

}

void LogManager::Shutdown()
{
	HashItem<String,Log*> *pItem = NULL;
	for(U32 i = 0;i < m_hmLogs.BucketSize();++i){
		for(pItem = m_hmLogs.GetBucketItem(i);pItem;pItem = pItem->GetNext()){
			K15_DELETE pItem->GetValue();
		}
	}
	m_pDefaultLog = NULL;
	m_hmLogs.Clear();
}

Log *LogManager::CreateLog( const String &sLogName,bool bDefaultLog,bool bDebugLog,bool bFileLog )
{
	Log *pLog = K15_NEW Log(sLogName,bDebugLog,bFileLog);
	
	if(bDefaultLog){
		SetDefaultLog(pLog);
	}

	m_hmLogs.Insert(sLogName,pLog);

	return pLog;
}

Log *LogManager::GetDefaultLog()
{
	return m_pDefaultLog;
}

void LogManager::SetDefaultLog( Log *pLog )
{
	m_pDefaultLog = pLog;
}

void LogManager::LogMessage( const String &sLogMessage )
{
	if(m_pDefaultLog){
		m_pDefaultLog->LogMessage(sLogMessage);
	}
}
