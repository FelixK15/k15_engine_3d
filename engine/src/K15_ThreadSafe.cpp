/**
 * @file K15_ThreadSafe.cpp
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

#include "K15_ThreadSafe.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  ThreadSafe::ThreadSafe()
    : m_Mutex(),
    m_Locked(false)
  {

  }
  /*********************************************************************************/
  ThreadSafe::~ThreadSafe()
  {
    if(isLocked())
    {
      unlockMutex();
    }
  }
  /*********************************************************************************/
  void ThreadSafe::lockMutex(bool p_BlockIfLocked)
  {
    K15_ASSERT(!isLocked(),"Mutex already locked.");

    if(p_BlockIfLocked)
    {
      m_Locked = true;
      m_Mutex.lock();
    }
    else
    {
     m_Locked = m_Mutex.try_lock();
    }
  }
  /*********************************************************************************/
  void ThreadSafe::unlockMutex(bool p_BlockIfLocked)
  {
    K15_ASSERT(isLocked(),"Mutex is not locked.");

    if(p_BlockIfLocked)
    {
      m_Mutex.unlock();
      m_Locked = false;
    }
    else
    {
      if(!isLocked())
      {
        m_Mutex.unlock();
        m_Locked = false;
      }
    }
  }
  /*********************************************************************************/

}}// end of K15_Engine::Core namespace