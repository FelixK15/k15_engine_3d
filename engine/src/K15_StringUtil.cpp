/**
 * @file K15_StringUtil.cpp
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

#include "K15_StringUtil.h"

namespace K15_Engine { namespace System {
  /*********************************************************************************/
  String StringUtil::format(char* p_Message,...)
  {
    va_list list;
    uint32 length = strlen(p_Message);
    length += K15_FORMAT_MESSAGE_ADDITIONAL_LENGTH;

    char* buffer = (char*)_malloca(length);
    va_start(list,p_Message);
    vsprintf(buffer,p_Message,list);
    va_end(list);

    return buffer;
  }
  /*********************************************************************************/
}}//end of K15_Engine::System namespace