/**
 * @file K15_TextConsoleLog.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
#include "K15_TextConsoleLog.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	TextConsoleLog::TextConsoleLog()
		: LogBase()
	{
		setLogBitMask(LogManager::LP_ALL);
	}
	/*********************************************************************************/
	void TextConsoleLog::logMessage(const String &p_Message,Enum p_PriorityFlag)
	{
		static String msg;
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		static WORD colorCode = 0;

		msg = Core::StringUtil::timeAsString() + " ";

		if(p_PriorityFlag & Core::LogManager::LP_NORMAL)
		{
			msg += "[NORMAL]: ";
			colorCode = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		}
		else if(p_PriorityFlag & Core::LogManager::LP_ERROR)
		{
			msg += "[ERROR]: ";
			colorCode = FOREGROUND_RED;
		}
		else if(p_PriorityFlag & Core::LogManager::LP_WARNING)
		{
			msg += "[WARNING]: ";
			colorCode = FOREGROUND_GREEN | FOREGROUND_RED;
		}
		else if(p_PriorityFlag & Core::LogManager::LP_SUCCESS)
		{
			msg += "[SUCCESS]: ";
			colorCode = FOREGROUND_GREEN;
		}
		else if(p_PriorityFlag & Core::LogManager::LP_DEBUG)
		{
			msg += "[DEBUG]: ";
			colorCode = FOREGROUND_BLUE;
		}

		msg += p_Message;
		SetConsoleTextAttribute(consoleHandle,colorCode);
		printf("%s\n",msg.c_str());
	}
	/*********************************************************************************/
}}//end of K15_Engine::System namespace