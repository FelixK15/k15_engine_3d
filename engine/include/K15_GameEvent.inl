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