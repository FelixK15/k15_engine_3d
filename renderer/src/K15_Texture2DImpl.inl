/**
 * @file K15_Texture2DImpl.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/20
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

inline bool Texture2DImpl::HasAlpha() const
{
	return m_bAlpha;
}

inline U32 Texture2DImpl::GetWidth() const
{
	return m_iWidth;
}

inline U32 Texture2DImpl::GetHeight() const
{
	return m_iHeight;
}

inline U32 Texture2DImpl::GetAmountMipMaps() const
{
	return m_iAmountMipMaps;
}