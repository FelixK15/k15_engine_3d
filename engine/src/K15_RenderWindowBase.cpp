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
	/*********************************************************************************/
	float Resolution::getAspectRatio() const
	{
		if(height == 0) return 0.0f;

		return (float)((float)width / (float)height);
	}
	/*********************************************************************************/

	/*********************************************************************************/
	EventName RenderWindowBase::EventFocusLost = _EN(EventFocusLost);
	EventName RenderWindowBase::EventFocusReceived = _EN(EventFocusReceived);
	EventName RenderWindowBase::EventResolutionChanged = _EN(EventResolutionChanged);
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
			g_EventManager->triggerEvent(K15_NEW GameEvent(EventResolutionChanged,(void*)&p_Resolution,K15_PTR_SIZE));
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
      		focusEvent = K15_NEW GameEvent(EventFocusReceived);
  		}
  		else
    	{
      		focusEvent = K15_NEW GameEvent(EventFocusLost);
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