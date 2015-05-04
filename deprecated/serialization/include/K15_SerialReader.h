/**
 * @file K15_SerialReader.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/20
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Serialization_SerialReader_h_
#define _K15Engine_Serialization_SerialReader_h_

#include "K15_Prerequisites.h"
#include "K15_SerializerBase.h"

namespace K15_Engine { namespace Serialization {
  class K15_CORE_API SerialReader : public SerializerBase
  {
	public:
		SerialReader();
		~SerialReader();

		virtual byte* getBufferPointer() const OVERRIDE;

		INLINE uint32 getInputBufferSize() const;
		INLINE void setInputBuffer(byte* p_InputBuffer, uint32 p_InputBufferSize);

	protected:
		virtual void _io(byte* p_Data, uint32 p_Size) OVERRIDE;
		virtual void _io(uint32& p_Integer) OVERRIDE;
		virtual void _io(int32& p_Integer) OVERRIDE;
		virtual void _io(float& p_Float) OVERRIDE;

	private:
		byte* m_InputBuffer;
		uint32 m_InputBufferSize;
		bool m_BufferOwner;
  }; //end of SerialReader class declaration
#include "K15_SerialReader.inl"
}} //end of K15_Engine::Serialization

#endif //_K15Engine_Serialization_SerialReader_h_