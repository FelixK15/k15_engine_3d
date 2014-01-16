/**
 * @file K15_ThreadSafe.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/29
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

#ifndef _K15Engine_Core_ThreadSafe_h_
#define _K15Engine_Core_ThreadSafe_h_

#ifndef K15_USE_PRECOMPILED_HEADER
# include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
  class K15_CORE_API ThreadSafe
  {
  public:
    ThreadSafe();
    virtual ~ThreadSafe();

    void lockMutex(bool p_BlockIfLocked = true);
    void unlockMutex(bool p_BlockIfLocked = true);

    INLINE bool isLocked() const;
  private:
    Mutex m_Mutex;
    bool m_Locked;
  }; //end of ThreadSafe class declaration
#include "K15_ThreadSafe.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ThreadSafe_h_