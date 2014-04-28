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

#include "K15_RenderWindowBase.h"
#include "K15_GameEvent.h"

namespace K15_Engine { namespace Core {
// 	/*********************************************************************************/
// 	RenderWindowBase::Listener::Listener()
// 		: EventListener()
// 	{
// 		g_EventManager->addListener(_EN(ResolutionChanged),this);
// 	}
// 	/*********************************************************************************/
// 	RenderWindowBase::Listener::~Listener()
// 	{
// 		g_EventManager->removeListener(_EN(ResolutionChanged),this);
// 	}
// 	/*********************************************************************************/
// 	void RenderWindowBase::Listener::handleEvent(GameEvent* p_Event)
// 	{
// 		if(p_Event->getName() == _EN(ResolutionChanged) && p_Event->getArgumentSize() == sizeof(Resolution))
// 		{
// 			Resolution* resolution = (Resolution*)p_Event->getArgument();
// 			onResolutionChanged(*resolution);
// 			return;
// 		}
// 	}
// 	/*********************************************************************************/

  /*********************************************************************************/
  float Resolution::getAspectRatio() const
  {
    if(height == 0) return 0.0f;

    return width / height;
  }
  /*********************************************************************************/

	/*********************************************************************************/
	RenderWindowBase::RenderWindowBase()
		: m_WindowTitle(""),
		m_CurrentResolution(),
		m_IsFullscreen(false),
		m_HasFocus(true)
	{

	}
	/*********************************************************************************/
	RenderWindowBase::~RenderWindowBase()
	{

	}
	/*********************************************************************************/
	void RenderWindowBase::setWindowTitle(const String& p_WindowTitle)
	{
		m_WindowTitle = p_WindowTitle;
	}
	/*********************************************************************************/
	const String& RenderWindowBase::getWindowTitle() const
	{
		return m_WindowTitle;
	}
	/*********************************************************************************/
	void RenderWindowBase::setResolution(const Resolution& p_Resolution, bool p_ForceChange)
	{
		m_CurrentResolution = p_Resolution;

		if(p_ForceChange || (m_CurrentResolution.width == p_Resolution.width || m_CurrentResolution.height == p_Resolution.height))
		{
			g_EventManager->triggerEvent(K15_NEW GameEvent(_EN(ResolutionChanged),(void*)&p_Resolution,K15_PTR_SIZE));
		}
		
	}
	/*********************************************************************************/
	const Resolution& RenderWindowBase::getResolution() const
	{
		return m_CurrentResolution;
	}
	/*********************************************************************************/
	void RenderWindowBase::setIsFullscreen(bool p_Fullscreen)
	{
		m_IsFullscreen = p_Fullscreen;
	}
	/*********************************************************************************/
	bool RenderWindowBase::isFullscreen() const
	{
		return m_IsFullscreen;
	}
	/*********************************************************************************/
	void RenderWindowBase::setHasFocus(bool p_HasFocus)
	{
  		if(m_HasFocus == p_HasFocus)
  		{
  				return;
  		}

  		GameEvent *focusEvent = 0;

  		if(p_HasFocus)
  		{
      		focusEvent = K15_NEW GameEvent(_EN(FocusGained));
  		}
  		else
    	{
      		focusEvent = K15_NEW GameEvent(_EN(FocusLost));
    	}

    	m_HasFocus = p_HasFocus;

    	g_EventManager->triggerEvent(focusEvent);
	}
	/*********************************************************************************/
	bool RenderWindowBase::getHasFocus() const
	{
		return m_HasFocus;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace