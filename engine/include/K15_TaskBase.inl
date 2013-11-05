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
inline uint32 TaskBase::getPriority() const
{
	return m_Priority;
}
/*********************************************************************************/
inline void TaskBase::setPriority(uint32 p_Priority)
{
	m_Priority = p_Priority;
}
/*********************************************************************************/
inline uint32 TaskBase::getFrameModFilter() const
{
  return m_FrameModFilter;
}
/*********************************************************************************/
inline void TaskBase::setFrameModFilter(uint32 p_FrameModFilter)
{
  if(p_FrameModFilter == 0) p_FrameModFilter = 1;

  m_FrameModFilter = p_FrameModFilter;
}
/*********************************************************************************/