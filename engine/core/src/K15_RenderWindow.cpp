/**
 * @file K15_RenderWindowBase.h
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

#include "K15_RenderWindow.h"
#include "K15_EventManager.h"
#include "K15_GameEvent.h"

#ifdef K15_OS_WINDOWS
#include "win32\K15_RenderWindow_Win32.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  EventName   RenderWindow::EventFocusLost			= _EN(EventFocusLost);
  EventName   RenderWindow::EventFocusReceived		= _EN(EventFocusReceived);
  EventName   RenderWindow::EventResolutionChanged	= _EN(EventResolutionChanged);
  EventName   RenderWindow::EventInitialized			= _EN(EventRenderWindowInitialized);
  bool	    RenderWindow::ms_HasFocus			    = false;
  bool        RenderWindow::ms_IsFullscreen		    = false;
  bool        RenderWindow::ms_IsInitialized			= false;
  String	    RenderWindow::ms_WindowTitle;
  Resolution  RenderWindow::ms_CurrentResolution;
  /*********************************************************************************/

  /*********************************************************************************/
  bool RenderWindow::initialize()
  {
     K15_ASSERT(!ms_IsInitialized, "Render window is already initialized!");

     if(ms_IsInitialized = RenderWindowImpl::initialize()) {
        g_EventManager->triggerEvent(K15_NEW GameEvent(EventInitialized));

        ms_CurrentResolution.width  = RenderWindowImpl::getWidth();
        ms_CurrentResolution.height = RenderWindowImpl::getHeight();
     }

     return ms_IsInitialized;
  }
  /*********************************************************************************/
  void RenderWindow::shutdown()
  {
     RenderWindowImpl::shutdown();
     ms_IsInitialized = false;
  }
  /*********************************************************************************/
  void RenderWindow::setWindowTitle(const String& p_WindowTitle)
  {
      ms_WindowTitle = p_WindowTitle;

     if(ms_IsInitialized) {
        RenderWindowImpl::setWindowTitle(p_WindowTitle);
     }
  }
  /*********************************************************************************/
  void RenderWindow::setResolution(const Resolution& p_Resolution)
  {
      ms_CurrentResolution = p_Resolution;

     if(ms_IsInitialized && (ms_CurrentResolution.width == p_Resolution.width || ms_CurrentResolution.height == p_Resolution.height)) {
        EventManager::getInstance()->triggerEvent(K15_NEW GameEvent(EventResolutionChanged,(void*)&p_Resolution,K15_PTR_SIZE));
        RenderWindowImpl::setResolution(p_Resolution);
     }
  }
  /*********************************************************************************/
  void RenderWindow::setIsFullscreen(bool p_Fullscreen)
  {
    ms_IsFullscreen = p_Fullscreen;

    if(ms_IsInitialized) {
        RenderWindowImpl::setIsFullscreen(p_Fullscreen);
     }
  }
  /*********************************************************************************/
  void RenderWindow::setHasFocus(bool p_HasFocus)
  {
    ms_HasFocus = p_HasFocus;

    if(!ms_IsInitialized) {
      return;
    }
    if(ms_HasFocus == p_HasFocus) {
      return;
    }

    GameEvent* focusEvent = 0;

    if(p_HasFocus) {
      focusEvent = K15_NEW GameEvent(EventFocusReceived);
    } else {
      focusEvent = K15_NEW GameEvent(EventFocusLost);
    }

    g_EventManager->triggerEvent(focusEvent);
  }
  /*********************************************************************************/
}}//end of K15_Engine::Core namespace
