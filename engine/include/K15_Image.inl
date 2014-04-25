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
ColorRGBA Image::getPixel(uint32 p_PosX,uint32 p_PosY)
{
	K15_ASSERT(m_Width >= p_PosX && m_Height >= p_PosY,
		StringUtil::format("Trying to access pixel out of bounds. Image dimension: %ux%u. Trying to read pixel location %ux%u.",
		m_Width,m_Height,p_PosX,p_PosY));

	return m_Pixels[p_PosX + (p_PosY * m_Width)];
}
/*********************************************************************************/
void Image::setPixel(uint32 p_PosX, uint32 p_PosY, const ColorRGBA& p_Color)
{
	K15_ASSERT(m_Width >= p_PosX && m_Height >= p_PosY,
		StringUtil::format("Trying to access pixel out of bounds. Image dimension: %ux%u. Trying to read pixel location %ux%u.",
		m_Width,m_Height,p_PosX,p_PosY));

	m_Pixels[p_PosX + (p_PosY * m_Width)] = p_Color;
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