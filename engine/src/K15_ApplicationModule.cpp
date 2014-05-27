/**
 * @file K15_ApplicationModule.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/29
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

#include "K15_ApplicationModule.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	ApplicationModule::ApplicationModule()
		: m_RendererInitialized(false)
	{

	}
	/*********************************************************************************/
	ApplicationModule::~ApplicationModule()
	{

	}
	/*********************************************************************************/
	void ApplicationModule::onRendererInitialized(const RendererBase* p_Renderer)
	{
		m_RendererInitialized = true;
		_onRendererInitialized(p_Renderer);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace