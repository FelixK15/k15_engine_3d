/**
 * @file K15_Image.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/14
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
ColorRGBA* Image::getPixels() const
{
	return m_Pixels;
}
/*********************************************************************************/
uint32 Image::getWidth() const
{
	return m_Width;
}
/*********************************************************************************/
uint32 Image::getHeight() const
{
	return m_Height;
}
/*********************************************************************************/
uint32 Image::getAmountPixels() const
{
	return m_Height * m_Width;
}
/*********************************************************************************/