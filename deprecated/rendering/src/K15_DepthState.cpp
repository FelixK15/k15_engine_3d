/**
 * @file K15_DepthState.cpp
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

#include "K15_RendererBase.h"
#include "K15_DepthState.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  DepthState::DepthState()
    : m_Enabled(true),
    m_Bias(0.0f),
    m_Function(RendererBase::FT_LESS_EQUAL)
  {

  }
  /*********************************************************************************/
  DepthState::~DepthState()
  {

  }
  /*********************************************************************************/
  bool DepthState::operator!=(const DepthState& p_Other) const
  {
    return !(m_Enabled == p_Other.m_Enabled && m_Bias == p_Other.m_Bias && m_Function == p_Other.m_Function);
  }
  /*********************************************************************************/
  void DepthState::setFunction(Enum p_Function)
  {
	  K15_ASSERT(p_Function < RendererBase::FT_COUNT,"Invalid depth test function.");
	  m_Function = p_Function;
  }
  /*********************************************************************************/
}}// end of K15_Engine::Rendering namespace
