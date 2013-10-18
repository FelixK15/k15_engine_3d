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

namespace K15_Engine { namespace System {
	/*********************************************************************************/
	RenderWindowBase::RenderWindowBase()
		: m_WindowTitle(""),
		m_CurrentResolution(),
		m_IsFullscreen(false)
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
	void RenderWindowBase::setResolution(const Resolution& p_Resolution)
	{
		m_CurrentResolution = p_Resolution;
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
}}//end of K15_Engine::System namespace