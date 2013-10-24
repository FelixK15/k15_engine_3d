/**
 * @file K15_GameTime.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#include "K15_GameTime.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	GameTime::GameTime(double p_DeltaTime,double p_Multiplier)
		: m_DeltaTime(p_DeltaTime * p_Multiplier),
		  m_Mulitplicator(p_Multiplier),
		  m_RawDeltaTime(p_DeltaTime)
	{

	}
	/*********************************************************************************/
	GameTime::~GameTime()
	{

	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace