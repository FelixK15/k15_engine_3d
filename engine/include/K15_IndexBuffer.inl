/**
 * @file K15_IndexBuffer.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/09
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
Enum IndexBuffer::getIndexType() const
{
	return m_IndexType;
}
/*********************************************************************************/
void IndexBuffer::setIndexType(Enum p_IndexType) 
{
	K15_ASSERT(p_IndexType > 0 && p_IndexType < IT_COUNT,"Invalid index type.");

	m_IndexType = p_IndexType;
}
/*********************************************************************************/