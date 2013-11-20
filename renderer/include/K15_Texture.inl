/**
 * @file K15_Texture.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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
inline Enum Texture::getType() const
{
	return m_Type;
}
/*********************************************************************************/
inline Enum Texture::getUsage() const
{
	return m_Usage;
}
/*********************************************************************************/
inline uint8 Texture::getMipMapCount() const
{
	return m_MipMapCount;
}
/*********************************************************************************/
inline uint32 Texture::getWidth() const
{
	return m_Width;
}
/*********************************************************************************/
inline uint32 Texture::getHeight() const
{
	return m_Height;
}
/*********************************************************************************/
inline Resolution Texture::getResolution() const
{
	Resolution resolution;
	resolution.width = m_Width;
	resolution.height = m_Height;
	return resolution;
}
/*********************************************************************************/
inline uint32 Texture::getDepth() const
{
	return m_Depth;
}
/*********************************************************************************/
inline Enum Texture::getPixelFormat() const
{
	return m_PixelFormat;
}
/*********************************************************************************/