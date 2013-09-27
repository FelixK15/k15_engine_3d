/**
 * @file K15_Texture.inl
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

inline bool Texture2D::HasAlpha() const
{
	return m_pTexture2DImpl->HasAlpha();
}

inline U32 Texture2D::GetHeight() const
{
	return m_pTexture2DImpl->GetHeight();
}

inline U32 Texture2D::GetWidth() const
{
	return m_pTexture2DImpl->GetWidth();
}

inline U32 Texture2D::GetAmountMipMaps() const
{
	return m_pTexture2DImpl->GetAmountMipMaps();
}

inline bool Texture2D::load(byte *pBuffer,U32 iSize,const String &sExtension)
{
	return m_pTexture2DImpl->Load(pBuffer,iSize,sExtension);
}