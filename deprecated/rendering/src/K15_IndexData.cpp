/**
 * @file K15_IndexData.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/29
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

#include "K15_IndexData.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  IndexData::IndexData(IndexBuffer* p_IndexBuffer, uint32 p_IndexCount, uint32 p_OffsetInBytes)
    : m_IndexBuffer(p_IndexBuffer),
    m_Offset(p_OffsetInBytes),
    m_IndexCount(p_IndexCount)
  {

  }
  /*********************************************************************************/
  IndexData::~IndexData()
  {

  }
  /*********************************************************************************/
}} // end of K15_Engine::Rendering namespace