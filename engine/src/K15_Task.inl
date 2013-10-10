/**
 * @file K15_Task.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/26
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
inline void Task::update(const GameTime& p_GameTime)
{
	if(m_Functor.isValid())
	{
		m_Functor(p_GameTime);
	}
}
/*********************************************************************************/
inline void Task::setUpdateFunc(Task::FunctorType p_Functor)
{
	m_Functor = p_Functor;
}
/*********************************************************************************/
inline uint32 Task::getPriority() const
{
	return m_Priority;
}
/*********************************************************************************/
inline void Task::setPriority(uint32 p_Priority)
{
	m_Priority = p_Priority;
}
/*********************************************************************************/