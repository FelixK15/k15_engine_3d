/**
 * @file K15_TextConsoleLog.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_TextConsoleLog_h_
#define _K15Engine_Core_TextConsoleLog_h_

#include "K15_Prerequisites.h"
#include "K15_LogBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API TextConsoleLog_Win32 : public LogBase
	{
	public:
		/**
		 * Default constructor
		 */
		TextConsoleLog_Win32();

		/**
		 * Add a new log message to the console via the stdout stream
		 *
		 * @param const String& p_Message - message to log
		 * @param Enum p_PriorityFlag - flag to determine the kind of log message
		 */
		virtual void logMessage(const String &p_Message,Enum p_PriorityFlag);
	};// end of TextConsoleLog class definition
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_TextConsoleLog_h_