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

#ifndef _K15Engine_System_LogManager_h_
#define _K15Engine_System_LogManager_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_Singleton.h"
#include "K15_Application.h"

#define K15_LogNormalMessage(msg)  K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_NORMAL)
#define K15_LogErrorMessage(msg)   K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_ERROR)
#define K15_LogDebugMessage(msg)   K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_DEBUG)
#define K15_LogWarningMessage(msg) K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_WARNING)
#define K15_LogCustom0Message(msg) K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_CUSTOM0)
#define K15_LogCustom1Message(msg) K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_CUSTOM1)
#define K15_LogCustom2Message(msg) K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_CUSTOM2)
#define K15_LogCustom3Message(msg) K15_Engine::System::LogManager::getInstance()->logMessage(msg,false,K15_Engine::System::LogManager::LP_CUSTOM3)

namespace K15_Engine { namespace System {

	class K15_API_EXPORT LogManager : public ApplicationAllocatedObject, 
									  public Singleton<LogManager>
	{
	public:
		typedef K15_List(LogBase*) LogList;
	public:
		enum eLogPriorityFlags
		{
			LP_INVALID = 0x0000,
			LP_NORMAL = 0x0002,
			LP_DEBUG = 0x0004,
			LP_ERROR = 0x0008,
			LP_WARNING = 0x0020,

			LP_CUSTOM0 = 0x0040,
			LP_CUSTOM1 = 0x0080,
			LP_CUSTOM2 = 0x0200,
			LP_CUSTOM3 = 0x0400
		};

	public:
		LogManager();
		virtual ~LogManager();

		void addLog(LogBase* p_Log, bool p_DefaultLog = false);
		
		LogBase* getDefaultLog();
		void setDefaultLog(LogBase* p_Log);

		void logMessage(const String& p_LogMessage, bool p_DefaultLogOnly = false, eLogPriorityFlags p_PriorityFlag = LP_INVALID);

	private:
		LogBase *m_DefaultLog;
		LogList m_Logs;
	};
#include "K15_LogManager.inl"
}} // end of K15_Engine::System namespace

#endif //_K15Engine_System_LogManager_h_