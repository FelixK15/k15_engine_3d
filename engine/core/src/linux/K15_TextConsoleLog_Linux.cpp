/**
 * @file K15_TextConsoleLog_Linux.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
 */

#include "K15_PrecompiledHeader.h"

#ifdef K15_OS_LINUX

#include "linux/K15_TextConsoleLog_Linux.h"

namespace K15_Engine { namespace Core {
    /*********************************************************************************/
    TextConsoleLog_Linux::TextConsoleLog_Linux()
    {

    }
    /*********************************************************************************/
    TextConsoleLog_Linux::~TextConsoleLog_Linux()
    {

    }
    /*********************************************************************************/
    void TextConsoleLog_Linux::logMessage(const String &p_Message, Enum p_PriorityFlag)
    {
        static const String NORMAL_CONSOLE_COLOR = "\033[0m";
        static const String DEBUG_CONSOLE_COLOR = "\033[0;34m";
        static const String ERROR_CONSOLE_COLOR = "\033[0;31m";
        static const String WARNING_CONSOLE_COLOR = "\033[0;37m";
        static const String SUCCESS_CONSOLE_COLOR = "\033[0;32m";

        String logMessage;

        if(p_PriorityFlag == LogManager::LP_DEBUG)
        {
            logMessage = DEBUG_CONSOLE_COLOR;
        }
        else if(p_PriorityFlag == LogManager::LP_ERROR)
        {
            logMessage = ERROR_CONSOLE_COLOR;
        }
        else if(p_PriorityFlag == LogManager::LP_WARNING)
        {
            logMessage = WARNING_CONSOLE_COLOR;
        }
        else if(p_PriorityFlag == LogManager::LP_SUCCESS)
        {
            logMessage = SUCCESS_CONSOLE_COLOR;
        }

        logMessage += p_Message;
        logMessage += NORMAL_CONSOLE_COLOR;

        printf("%s\n",logMessage.c_str());
    }
    /*********************************************************************************/
}} // end of K15_Engine::Core namespace

#endif //K15_OS_LINUX
