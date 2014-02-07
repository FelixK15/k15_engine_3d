/**
 * @file K15_JsonSerializer.cpp
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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_JsonSerializer.h"

#include "json\reader.h"

namespace K15_Engine { namespace Serialization {
  /*********************************************************************************/
  void traverseDocument_R(Json::Value& p_Root, JsonSerializer::KeyValueMap& p_KeyValueMap)
  {
    // we could do this as a member function but then we'd have to include the json file into the header of 
    // the json serializer...And I don't want that.

    for(Json::Value::iterator iter = p_Root.begin();iter != p_Root.end();++iter)
    {
      //p_KeyValueMap[]
    }
  }
	/*********************************************************************************/
	JsonSerializer::JsonSerializer(const String& p_FilePath)
    : m_FilePath(p_FilePath),
    m_KeyValueMap()
	{

	}
	/*********************************************************************************/
	JsonSerializer::~JsonSerializer()
	{

	}
  /*********************************************************************************/
  void JsonSerializer::writeInt8(const String& p_Key, int8 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<int8>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeUInt8(const String& p_Key, uint8 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<uint8>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeInt16(const String& p_Key, int16 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<int16>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeUInt16(const String& p_Key, uint16 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<uint16>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeInt32(const String& p_Key, int32 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<int32>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeUInt32(const String& p_Key, uint32 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<uint32>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeInt64(const String& p_Key, int64 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<int64>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeUInt64(const String& p_Key, uint64 p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString<uint64>(p_Value);
  }
  /*********************************************************************************/
  void JsonSerializer::writeFloat(const String& p_Key, float p_Value)
  {
    m_KeyValueMap[p_Key] = StringUtil::toString(p_Value,2);
  }
  /*********************************************************************************/
  void JsonSerializer::writeString(const String& p_Key, const String& p_Value)
  {
    m_KeyValueMap[p_Key] = p_Value;
  }
  /*********************************************************************************/
  void JsonSerializer::_parse()
  {
    Json::Reader reader;
    Json::Value root;

    if(reader.parse(m_FilePath,root,false))
    {
      
    }
    else
    {
      m_Valid = false;
      m_Error = reader.getFormattedErrorMessages();
    }
  }
  /*********************************************************************************/

}}// end of K15_Engine::Core namespace