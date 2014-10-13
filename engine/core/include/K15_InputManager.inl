/**
 * @file K15_InputManager.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/05
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
float InputEvent::getValue() const
{
	return m_Value;
}
/*********************************************************************************/
Enum InputEvent::getInputSource() const
{
	return m_InputSource;
}
/*********************************************************************************/


/*********************************************************************************/
template<class T, bool (T::*MemberFunction)(InputEvent*)>
static InputHandler InputHandler::create(const T* p_Object)
{
	InputHandler handler;
	handler.m_Object = (void*)p_Object;
	handler.m_Handler = &getHandlerFunction<T, MemberFunction>;
	return handler;
}
/*********************************************************************************/
bool InputHandler::operator()(InputEvent* p_Arg) const
{
	return m_Handler(m_Object, p_Arg);
}
/*********************************************************************************/
bool InputHandler::operator==(const InputHandler& p_Rhs) const
{
	return m_Object == p_Rhs.m_Object && m_Handler == p_Rhs.m_Handler;
}
/*********************************************************************************/
bool InputHandler::isValid() const
{
	return m_Object != 0 && m_Handler != 0;
}
/*********************************************************************************/
template<class T, bool (T::*MemberFunction)(InputEvent*)>
static bool InputHandler::getHandlerFunction(void* p_Object, InputEvent* p_Args)
{
	T* obj = static_cast<T*>(p_Object);
	return (obj->*MemberFunction)(p_Args);
}
/*********************************************************************************/