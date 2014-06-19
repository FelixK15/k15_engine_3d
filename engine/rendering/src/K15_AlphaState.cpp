/**
 * @file K15_AlphaState.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#include "K15_AlphaState.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	AlphaState::AlphaState()
		: m_BlendOperation(0),
		m_SourceBlendFunction(0),
		m_DestinationBlendFunction(0),
		m_ConstantColor(ColorRGBA(0,0,0,0))
	{

	}
	/*********************************************************************************/
	AlphaState::~AlphaState()
	{

	}
	/*********************************************************************************/
	bool AlphaState::operator!=(const AlphaState& p_Other) const
	{
	return !(m_BlendOperation == p_Other.m_BlendOperation && m_SourceBlendFunction == p_Other.m_SourceBlendFunction &&
		m_DestinationBlendFunction == p_Other.m_DestinationBlendFunction && m_ConstantColor == p_Other.m_ConstantColor);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace