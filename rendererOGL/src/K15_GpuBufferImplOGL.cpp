/**
 * @file K15_GpuBufferImplOGL.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_GpuBufferImplOGL.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
	/*********************************************************************************/
	GpuBufferImplOGL::GpuBufferImplOGL()
		: m_BufferIndex(0)
	{

	}
	/*********************************************************************************/
	GpuBufferImplOGL::~GpuBufferImplOGL()
	{

	}
	/*********************************************************************************/
	void GpuBufferImplOGL::init(Enum p_BufferType,Enum p_UsageOptions,Enum p_LockOptions)
	{
		glGenBuffers(1,&m_BufferIndex);
		
	}
	/*********************************************************************************/
	void GpuBufferImplOGL::shutdown()
	{

	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::lock()
	{

	}
	/*********************************************************************************/
	bool GpuBufferImplOGL::unlock()
	{

	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset)
	{
		
	}
	/*********************************************************************************/
	uint32 GpuBufferImplOGL::writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset)
	{
		glBufferData(m_BufferIndex,p_Size,(GLvoid*)(p_Destination + p_Offset)
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace