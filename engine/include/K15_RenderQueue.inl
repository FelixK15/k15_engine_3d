/**
 * @file K15_RenderQueue.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
void RenderQueue::setSortMode(Enum p_SortMode)
{
	m_SortMode = p_SortMode;
}
/*********************************************************************************/
Enum RenderQueue::getSortMode() const
{
	return m_SortMode;
}
/*********************************************************************************/
RenderOperation* RenderQueue::getRenderOperation(uint32 p_Index) const
{
	return m_RenderOperations.at(p_Index);
}
/*********************************************************************************/
void RenderQueue::addRenderOperation(RenderOperation* p_ROP)
{
	m_Dirty = true;
	m_RenderOperations.push_back(p_ROP);
}
/*********************************************************************************/
uint32 RenderQueue::size() const
{
	return m_RenderOperations.size();
}
/*********************************************************************************/
void RenderQueue::clear()
{
	m_RenderOperations.clear();
}
/*********************************************************************************/