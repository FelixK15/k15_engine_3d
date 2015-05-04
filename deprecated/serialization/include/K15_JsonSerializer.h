/**
 * @file K15_JsonSerializer.h
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

#ifndef _K15Engine_Core_JsonSerializer_h_
#define _K15Engine_Core_JsonSerializer_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Serialization {
	class K15_CORE_API JsonSerializer
	{
	public:
		typedef HashMap(String,String) KeyValueMap;

	public:
		JsonSerializer(const String& p_FilePath = "");
		~JsonSerializer();

		INLINE bool isValid() const;
		INLINE const String& getError() const;

		void writeInt8(const String& p_Key, int8 p_Value);
		void writeUInt8(const String& p_Key, uint8 p_Value);
		void writeInt16(const String& p_Key, int16 p_Value);
		void writeUInt16(const String& p_Key, uint16 p_Value);
		void writeInt32(const String& p_Key, int32 p_Value);
		void writeUInt32(const String& p_Key, uint32 p_Value);
		void writeInt64(const String& p_Key, int64 p_Value);
		void writeUInt64(const String& p_Key, uint64 p_Value);
		void writeFloat(const String& p_Key, float p_Value);
		/*void writeDouble(const String& p_Key, double p_Value);*/
		void writeString(const String& p_Key, const String& p_Value);

		INLINE int8 readInt8(const String& p_Key) const;
		INLINE uint8 readUInt8(const String& p_Key) const;
		INLINE int16 readInt16(const String& p_Key) const;
		INLINE uint16 readUInt16(const String& p_Key) const;
		INLINE int32 readInt32(const String& p_Key) const;
		INLINE uint32 readUInt32(const String& p_Key) const;
		INLINE int64 getInt64(const String& p_Key) const;
		INLINE uint64 readUInt64(const String& p_Key) const;
		INLINE float readFloat(const String& p_Key) const;
		/*INLINE double readDouble(const String& p_Key) const;*/
		INLINE const String& readString(const String& p_Key) const;

  private:
    void _parse();
    void _traverseDocument_R();

  private:
    String m_FilePath;
    String m_Error;
    KeyValueMap m_KeyValueMap;
    bool m_Valid;
    
	};// end of JsonSerializer class declaration
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_JsonSerializer_h_