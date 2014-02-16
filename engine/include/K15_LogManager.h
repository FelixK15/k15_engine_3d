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

#ifndef _K15Engine_Core_LogManager_h_
#define _K15Engine_Core_LogManager_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#	include "K15_Singleton.h"
#	include "K15_StringUtil.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#define _LogNormal(msg, ...)  K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_NORMAL)
#define _LogError(msg, ...)   K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_ERROR)
#define _LogDebug(msg, ...)   K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_DEBUG)
#define _LogWarning(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_WARNING)
#define _LogSuccess(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_SUCCESS)
#define _LogCustom0(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_CUSTOM0)
#define _LogCustom1(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_CUSTOM1)
#define _LogCustom2(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_CUSTOM2)
#define _LogCustom3(msg, ...) K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_CUSTOM3)

#define _LogNormal_NoSpam(msg, ...)		{static bool wrote = false; if(!wrote){K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_NORMAL);wrote = true;}}
#define _LogError_NoSpam(msg, ...)		{static bool wrote = false; if(!wrote){K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_ERROR);wrote = true;}}
#define _LogDebug_NoSpam(msg, ...)		{static bool wrote = false; if(!wrote){K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_DEBUG);wrote = true;}}
#define _LogWarning_NoSpam(msg, ...)	{static bool wrote = false; if(!wrote){K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_WARNING);wrote = true;}}
#define _LogSuccess_NoSpam(msg, ...)	{static bool wrote = false; if(!wrote){K15_Engine::Core::LogManager::getInstance()->logMessage(K15_Engine::Core::StringUtil::format(msg, ##__VA_ARGS__),false,K15_Engine::Core::LogManager::LP_SUCCESS);wrote = true;}}

namespace K15_Engine { namespace Core {

	class K15_CORE_API LogManager : public CoreAllocatedObject,
									public Singleton<LogManager>
	{
	public:
		typedef List(LogBase*) LogList;
	public:
		enum eLogPriorityFlags
		{
			LP_INVALID = 0x0000,
			LP_NORMAL = 0x0001,
			LP_DEBUG = 0x0002,
			LP_ERROR = 0x0004,
			LP_WARNING = 0x0008,
			LP_SUCCESS = 0x0010,

			LP_CUSTOM0 = 0x0020,
			LP_CUSTOM1 = 0x0040,
			LP_CUSTOM2 = 0x0080,
			LP_CUSTOM3 = 0x0100,

			LP_ALL_NORMAL = LP_NORMAL | LP_ERROR | LP_WARNING | LP_SUCCESS,
			LP_ALL_NORMAL_DEBUG = LP_ALL_NORMAL | LP_DEBUG,
			LP_ALL = LP_ALL_NORMAL_DEBUG | LP_CUSTOM0 | LP_CUSTOM1 | LP_CUSTOM2 | LP_CUSTOM3
		}; //eLogPriorityFlags

	public:
		
		/**
		 * Default constructor
		 */
		LogManager();

		/**
		 * Default destructor
		 */
		virtual ~LogManager();

		void addLog(LogBase* p_Log, bool p_DefaultLog = false, Enum p_LogMask = LP_ALL_NORMAL);
		
		LogBase* getDefaultLog() const;
		void setDefaultLog(LogBase* p_Log);

		void logMessage(const String& p_LogMessage, bool p_DefaultLogOnly = false, Enum p_LogFlag = LP_INVALID);

	private:
		LogBase *m_DefaultLog;
		LogList m_Logs;
	};// end of LogManager class
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_LogManager_h_