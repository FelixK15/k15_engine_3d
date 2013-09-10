/**
 * @file K15_Memory.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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

#include "K15_Application.h"

namespace K15_Engine { namespace System { 

  Application::Application()
    : Memory::StackAllocator(GIGABYTE),
      m_lCommands()
  {

  }

  Application::Application( int iCommandCount,char** pCommands )
    : Memory::StackAllocator(GIGABYTE),
      m_lCommands()
  {
    _createCommandList(iCommandCount,pCommands);
  }

  Application::~Application()
  {
    m_lCommands.Clear();
  }

  void Application::_createCommandList(int iCommandCount,char** pCommands)
  {
    for(int i = 0;i < iCommandCount;++i){
      String sCommand = pCommands[i];
      m_lCommands.PushBack(sCommand);
    }
  }
}}