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
void RenderQueue::addRenderOperation(RenderOperation* p_ROP)
{
  m_RenderOperations[m_RenderOperationCount++] = p_ROP;
}
/*********************************************************************************/
uint32 RenderQueue::getRenderOperationCount() const
{
  return m_RenderOperationCount;
}
/*********************************************************************************/
void RenderQueue::clearRenderOperations()
{
//   for(int i = 0;i < m_RenderOperationCount;++i)
//   {
//     m_RenderOperations[i] = 0;
//   }
//   
  m_RenderOperationCount = 0;
}
/*********************************************************************************/