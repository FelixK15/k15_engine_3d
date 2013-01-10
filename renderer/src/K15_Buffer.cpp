/**
 * @file K15_Buffer.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#include "K15_Buffer.h"
#include "K15_UsefulMacros.h"
#include "K15_Renderer.h"

using namespace K15_Renderer;

IMPLEMENT_RTTI("K15_Renderer",Buffer,Object);

Buffer::Buffer(BufferBinding bbBinding,BufferUsage buUsage,BufferAccess baAccess,FormatType ftType,U32 iBufferSize,U32 iSizePerElement)
	: m_bbBinding(bbBinding),
	  m_buUsage(buUsage),
	  m_baAccess(baAccess),
	  m_iBufferSize(iBufferSize),
	  m_iStride(iSizePerElement),
	  m_ftType(ftType)
{
	m_pData = K15_NEW byte[iBufferSize];
}

Buffer::~Buffer()
{
	K15_DELETE[] m_pData;
}