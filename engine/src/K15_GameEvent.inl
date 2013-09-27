/**
 * @file K15_Event.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/09
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
template<class T>
GameEvent::GameEvent(const EventName& p_Name,T p_Argument)
	: m_Name(p_Name),
	  m_ArgumentSize(sizeof(T)),
	  m_Argument(0)
{
	if(m_ArgumentSize > 0)
	{
#if defined (K15_DEBUG)
		m_Argument = ArgAllocator.allocateDebug(m_ArgumentSize,__FILE__,__LINE__,false,__FUNCTION__);
#else
		m_Argument = ArgAllocator.allocate(m_ArgumentSize);
#endif // K15_DEBUG

		memcpy(m_Argument,T,sizeof(T));
	}
}
/*********************************************************************************/
inline const EventName& GameEvent::getName() const
{
	return m_Name;
}
/*********************************************************************************/
inline const uint32 GameEvent::getArgumentSize() const
{
	return m_ArgumentSize;
}
/*********************************************************************************/
inline void* GameEvent::getArgument() const
{
	return m_Argument;
}
/*********************************************************************************/
// template<class T>
// T GameEvent::getArgument<T>(uint32 p_Offset) const
// {
// 	K15_ASSERT(p_Offset + sizeof(T) > m_ArgumentSize,"Argumentsize + offset would access undefined memory.");
// 
// 	return (T)((byte*)m_Argument + p_Offset);
// }
/*********************************************************************************/