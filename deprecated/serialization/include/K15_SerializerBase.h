/**
 * @file K15_SerializerBase.h
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

#ifndef _K15Engine_Core_SerializerBase_h_
#define _K15Engine_Core_SerializerBase_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Serialization {
	class K15_CORE_API SerializerBase
	{
		K15_NON_COPYABLE(SerializerBase)

	public:
		enum eSerializerType
		{
			ST_READER = 0,
			ST_WRITER,
			ST_SIZER,

			ST_COUNT
		}; //SerializerType
	public:
		virtual ~SerializerBase();
    
		template<class T> const SerializerBase& operator&(T& p_POD);
		const SerializerBase& operator&(String& p_String);
		const SerializerBase& operator&(int32& p_Integer);
		const SerializerBase& operator&(uint32& p_Integer);
		const SerializerBase& operator&(float& p_Integer);
    
		INLINE uint32 getCurrentPosition() const;
		INLINE eSerializerType getType() const;
		INLINE bool isFailed() const;

		virtual byte* getBufferPointer() const = 0;

	protected:
		SerializerBase(eSerializerType p_Type);

		virtual void _io(byte* p_Data, uint32 p_Size) = 0;
		virtual void _io(int32& p_Integer) = 0;
		virtual void _io(uint32& p_Integer) = 0;
		virtual void _io(float& p_Float) = 0;

	protected:
		uint32 m_CurrentPosition;
		uint32 m_Alignment;
		eSerializerType m_Type;
		byte m_AlignmentByte;
		bool m_Failed;

	};// end of SerializerBase class declaration
#include "K15_SerializerBase.inl"
}}// end of K15_Engine::Serialization namespace

#endif //_K15Engine_Core_SerializerBase_h_