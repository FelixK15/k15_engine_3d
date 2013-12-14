/**
 * @file K15_VertexBuffer.cpp
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

#include "K15_VertexBuffer.h"
#include "K15_VertexDeclaration.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	VertexBuffer::VertexBuffer()
		: GpuBuffer(GpuBuffer::BT_VERTEX_BUFFER,GpuBuffer::LO_DISCARD,GpuBuffer::UO_STATIC,GpuBuffer::BA_WRITE_ONLY,false)
	{

	}
	/*********************************************************************************/
	VertexBuffer::~VertexBuffer()
	{

	}
	/*********************************************************************************/
	uint32 VertexBuffer::getVertexSize() const
	{
		return m_Declaration->getVertexSize();
	}
	/*********************************************************************************/
	const VertexDeclaration* VertexBuffer::getVertexDeclaration() const
	{
		return m_Declaration;
	}
	/*********************************************************************************/
	void VertexBuffer::setVertexDeclaration(const String& p_DeclarationString)
	{
		m_Declaration->fromDeclarationString(p_DeclarationString);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace