/**
 * @file K15_VisualStudioLog_Win32.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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

#if defined(_MSC_VER)

#include "Win32\K15_VisualStudioLog_Win32.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	void VisualStudioLog::logMessage(const String &p_Message,Enum p_PriorityFlag)
	{
		static String msg;

		msg = Core::StringUtil::timeAsString() + " ";

		if(p_PriorityFlag & Core::LogManager::LP_NORMAL)
		{
			msg += "[NORMAL]: ";
		}
		else if(p_PriorityFlag & Core::LogManager::LP_ERROR)
		{
			msg += "[ERROR]: ";
		}
		else if(p_PriorityFlag & Core::LogManager::LP_WARNING)
		{
			msg += "[WARNING]: ";
		}
		else if(p_PriorityFlag & Core::LogManager::LP_SUCCESS)
		{
			msg += "[SUCCESS]: ";
		}
		else if(p_PriorityFlag & Core::LogManager::LP_DEBUG)
		{
			msg += "[DEBUG]: ";
		}
		msg += p_Message + "\n";

		OutputDebugString(msg.c_str());
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace

#endif //_MSC_VER