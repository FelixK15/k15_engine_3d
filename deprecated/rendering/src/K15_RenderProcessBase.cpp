/**
 * @file K15_RenderProcessBase.h
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
#include "K15_RenderProcessBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	RenderProcessBase::RenderProcessBase()
		: m_Renderer(0)
	{

	}
	/*********************************************************************************/
	RenderProcessBase::~RenderProcessBase()
	{

	}
	/*********************************************************************************/
	void RenderProcessBase::setRenderer(Rendering::RendererBase* p_Renderer)
	{
		m_Renderer = p_Renderer;
	}
	/*********************************************************************************/
	Rendering::RendererBase* RenderProcessBase::getRenderer() const
	{
		return m_Renderer;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace