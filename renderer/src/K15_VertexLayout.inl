/**
 * @file K15_VertexLayout.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/24
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

inline U8 VertexLayout::GetAmountAttributes() const
{
	return m_iAmountAttributes;
}

inline U32 VertexLayout::GetSizeOfVertex() const
{
	return m_iVertexSize;
}

inline VertexLayout::SingleLayout *VertexLayout::GetLayout() const
{
	return m_pLayout;
}