/**
 * @file K15_TextureSampler.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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
#include "K15_TextureSampler.h"
#include "K15_RendererBase.h"
#include "K15_RenderTask.h"
#include "K15_StringUtil.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  TextureSampler::TextureSampler()
    : m_MagFilterMode(TFM_LINEAR),
      m_MinFilterMode(TFM_LINEAR),
      m_UWrappingMode(TWM_CLAMP),
      m_VWrappingMode(TWM_CLAMP),
      m_WWrappingMode(TWM_CLAMP),
	  m_Impl(0)
  {
	  m_Impl = g_Application->getRenderTask()->getRenderer()->createTextureSamplerImpl();
	  m_Impl->setTextureSampler(this);
  }
  /*********************************************************************************/
  TextureSampler::~TextureSampler()
  {
	  K15_DELETE m_Impl;
  }
  /*********************************************************************************/
  Enum TextureSampler::getWrappingMode(uint8 p_Axis) const
  {
    K15_ASSERT(p_Axis < 2,StringUtil::format("Texture axis %u is unsufficient. Pass 0 for U - 1 for V - 2 for W.",p_Axis));

    if(p_Axis == 0)
    {
      return m_UWrappingMode;
    }
    else if(p_Axis == 1)
    {
      return m_VWrappingMode;
    }

    return m_WWrappingMode;
  }
  /*********************************************************************************/
  void TextureSampler::setWrappingMode(uint8 p_Axis,Enum p_WrappingMode)
  {
    K15_ASSERT(p_Axis < 2,StringUtil::format("Texture axis %u is unsufficient. Pass 0 for U - 1 for V - 2 for W.",p_Axis));

    if(p_Axis == 0)
    {
      m_UWrappingMode = p_WrappingMode;
      m_Impl->setUWrappingMode(p_WrappingMode);
    }
    else if(p_Axis == 1)
    {
      m_VWrappingMode = p_WrappingMode;
      m_Impl->setVWrappingMode(p_WrappingMode);
    }
    else if(p_Axis == 2)
    {
      m_WWrappingMode = p_WrappingMode;
      m_Impl->setWWrappingMode(p_WrappingMode);
    }
  }
  /*********************************************************************************/
}} // end of K15_Engine::Rendering namespace