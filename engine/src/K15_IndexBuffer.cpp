/**
 * @file K15_IndexBuffer.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/09
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

#include "K15_IndexBuffer.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	const uint32 IndexBuffer::IndexSize[IndexBuffer::IT_COUNT] = {
		1, //IT_UINT8
		2, //IT_UINT16
		4  //IT_UINT32
	};//IndexSize
	/*********************************************************************************/

	/*********************************************************************************/
	IndexBuffer::IndexBuffer()
		: GpuBuffer(GpuBuffer::BT_INDEX_BUFFER,GpuBuffer::LO_DISCARD,GpuBuffer::UO_STATIC,GpuBuffer::BA_WRITE_ONLY,false),
		m_IndexType(IT_UINT8)
	{

	}
	/*********************************************************************************/
	IndexBuffer::~IndexBuffer()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace