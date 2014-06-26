/**
 * @file K15_EventHandler.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/02
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
template<class T, bool (T::*MemberFunction)(GameEvent*)>
static EventHandler EventHandler::create(const T* p_Object)
{
	EventHandler handler;
	handler.m_Object = (void*)p_Object;
	handler.m_Handler = &getHandlerFunction<T,MemberFunction>;
	return handler;
}
/*********************************************************************************/
template<class T, bool (T::*MemberFunction)(GameEvent*)>
static bool EventHandler::getHandlerFunction(void* p_Object, GameEvent* p_Event)
{
	T* obj = static_cast<T*>(p_Object);
	return (obj->*MemberFunction)(p_Event);
}
/*********************************************************************************/
bool EventHandler::operator()(GameEvent* p_Args) const
{
	return m_Handler(m_Object, p_Args);
}
/*********************************************************************************/
bool EventHandler::operator==(const EventHandler& p_Rhs) const
{
  return m_Object == p_Rhs.m_Object && m_Handler == p_Rhs.m_Handler;
}
/*********************************************************************************/
