/**
 * @file K15_LogManager.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_LOGMANAGER__
#define __K15_LOGMANAGER__

#include "K15_StdInclude.h"
#include "K15_Singleton.h"
#include "K15_String.h"
#include "K15_HashMap.h"
#include "K15_Log.h"

#define g_pLogManager K15_EngineV2::LogManager::GetInstance()
#define WriteDefaultLog(x) g_pLogManager->LogMessage(x)

namespace K15_EngineV2
{
	class Log;

	class K15ENGINE2_API LogManager : public Singleton<LogManager>
	{
	public:
		LogManager();
		virtual ~LogManager();

		void Shutdown();

		Log *CreateLog(const String &sLogName,bool bDefaultLog,bool bDebugLog,bool bFileLog);
		
		Log *GetDefaultLog();
		void SetDefaultLog(Log *pLog);

		void LogMessage(const String &sLogMessage);

	private:
		Log *m_pDefaultLog;
		HashMap<String,Log*> m_hmLogs;
	};
}

#endif //__K15_K15_LOGMANAGER__