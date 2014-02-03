/**
 * @file K15_SerializerBase.inl
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

/*********************************************************************************/
uint32 SerializerBase::getCurrentPosition() const
{
	return m_CurrentPosition;
}
/*********************************************************************************/
SerializerBase::eSerializerType SerializerBase::getType() const
{
	return m_Type;
}
/*********************************************************************************/
bool SerializerBase::isFailed() const
{
  return m_Failed;
}
/*********************************************************************************/
template<class T>
const SerializerBase& SerializerBase::operator&(T& p_POD)
{
	if(m_Type == ST_SIZER || m_Type == ST_WRITER)
	{
		byte* data = static_cast<byte*>(&p_POD);
		_io(data,sizeof(T));
	}
	else
	{
		byte* data = static_cast<byte*>(alloca(sizeof(T)));
		_io(data,sizeof(T));

		memcpy(&p_POD,data,sizeof(T));
	}
}
/*********************************************************************************/