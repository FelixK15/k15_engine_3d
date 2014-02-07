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
Enum Texture::getTextureType() const
{
	return m_Type;
}
/*********************************************************************************/
Enum Texture::getUsage() const
{
	return m_Usage;
}
/*********************************************************************************/
uint32 Texture::getMipmapLevels() const
{
	return m_MipmapLevels;
}
/*********************************************************************************/
uint32 Texture::getWidth() const
{
	return m_Width;
}
/*********************************************************************************/
uint32 Texture::getHeight() const
{
	return m_Height;
}
/*********************************************************************************/
uint32 Texture::getDepth() const
{
	return m_Depth;
}
/*********************************************************************************/
Enum Texture::getPixelFormat() const
{
	return m_PixelFormat;
}
/*********************************************************************************/
const TextureImplBase* Texture::getImpl() const
{
	return m_Impl;
}
/*********************************************************************************/