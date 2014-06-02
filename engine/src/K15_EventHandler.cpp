/**
 * @file K15_EventHandler.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/02
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

#include "K15_EventHandler.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	EventHandler::EventHandler()
		: m_Object(0),
		m_Handler(0)
	{

	}
	/*********************************************************************************/
	EventHandler::EventHandler(const EventHandler& p_Rhs)
		: m_Object(p_Rhs.m_Object),
		m_Handler(p_Rhs.m_Handler)
	{

	}
	/*********************************************************************************/
	EventHandler::~EventHandler()
	{
		
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace