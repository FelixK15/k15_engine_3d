/**
 * @file K15_VertexData.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/29
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
VertexDeclaration* VertexData::getVertexDeclaration() const
{
  return m_VertexDeclaration;
}
/*********************************************************************************/
VertexBuffer* VertexData::getVertexBuffer() const
{
  return m_VertexBuffer;
}
/*********************************************************************************/
uint32 getOffsetInBytes() const
{
  return m_Offset;
}
/*********************************************************************************/
uint32 getVertexCount() const
{
  return m_VertexCount;
}
/*********************************************************************************/