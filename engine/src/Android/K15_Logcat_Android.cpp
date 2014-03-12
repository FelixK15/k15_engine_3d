/**
 * @file K15_Logcat_Android.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#include "Android\K15_Logcat_Android.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	Logcat_Android::Logcat_Android()
	{

	}
	/*********************************************************************************/
	Logcat_Android::~Logcat_Android()
	{

	}
	/*********************************************************************************/
	void Logcat_Android::logMessage(const String &p_Message,Enum p_PriorityFlag)
	{
		android_LogPriority prio;

		if(p_PriorityFlag == LogManager::LP_NORMAL)
		{
			prio = ANDROID_LOG_DEFAULT;
		}
		else if(p_PriorityFlag == LogManager::LP_SUCCESS)
		{
			prio = ANDROID_LOG_INFO;
		}
		else if(p_PriorityFlag == LogManager::LP_WARNING)
		{
			prio = ANDROID_LOG_WARN;
		}
		else if(p_PriorityFlag == LogManager::LP_ERROR)
		{
			prio = ANDROID_LOG_ERROR;
		}
		else if(p_PriorityFlag == LogManager::LP_DEBUG)
		{
			prio = ANDROID_LOG_DEBUG;
		}

		__android_log_write(prio,"K15_Engine",p_Message.c_str());
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace