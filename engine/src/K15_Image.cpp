/**
 * @file K15_Image.cpp
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

#include "K15_PrecompiledHeader.h"

#include "K15_Image.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,Image,ResourceBase);
	/*********************************************************************************/

	/*********************************************************************************/
	Image::Image()
		: m_Pixels(0),
		m_Width(0),
		m_Height(0)
	{

	}
	/*********************************************************************************/
	Image::Image(const Image& p_Rs)
		: m_Pixels(0),
		m_Width(p_Rs.m_Width),
		m_Height(p_Rs.m_Height)
	{
		if(m_Width > 0 && m_Height > 0)
		{
			m_Pixels = K15_NEW_SIZE(Allocators[AC_RESOURCE],m_Width * m_Height * sizeof(ColorRGBA)) ColorRGBA;
			K15_MEMCPY(m_Pixels,p_Rs.m_Pixels,getAmountPixels() * sizeof(ColorRGBA));
		}
	}
	/*********************************************************************************/
	Image::Image(uint32 p_Width, uint32 p_Height)
		: m_Pixels(0),
		m_Width(p_Width),
		m_Height(p_Height)
	{
		if(m_Width > 0 && m_Height > 0)
		{
			m_Pixels = K15_NEW_SIZE(Allocators[AC_RESOURCE],m_Width * m_Height * sizeof(ColorRGBA)) ColorRGBA;
		}
	}
	/*********************************************************************************/
	Image::Image(uint32 p_Width, uint32 p_Height, ColorRGBA* p_Pixels)
		: m_Pixels(0),
		m_Width(p_Width),
		m_Height(p_Height)
	{
		if(m_Width > 0 && m_Height > 0)
		{
			m_Pixels = K15_NEW_SIZE(Allocators[AC_RESOURCE],m_Width * m_Height * sizeof(ColorRGBA)) ColorRGBA;
			K15_MEMCPY(m_Pixels,p_Pixels,getAmountPixels() * sizeof(ColorRGBA));
		}
	}
	/*********************************************************************************/
	Image::~Image()
	{
		if(m_Pixels)
		{
			K15_DELETE_SIZE(Allocators[AC_RESOURCE],m_Pixels,m_Width * m_Height * sizeof(ColorRGBA));
			m_Pixels = 0;
		}
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
	const Image& Image::operator=(const Image& p_Rs)
	{
		if(m_Pixels)
		{
			if(m_Width == p_Rs.m_Width && m_Height == p_Rs.m_Height)
			{
				if(m_Pixels)
				{
					K15_MEMCPY(m_Pixels,p_Rs.m_Pixels,m_Width * m_Height * sizeof(ColorRGBA));
					return *this;
				}
			}

			if(m_Pixels)
			{
				K15_DELETE_SIZE(Allocators[AC_RESOURCE],m_Pixels,m_Width * m_Height * sizeof(ColorRGBA));
			}

			m_Pixels = K15_NEW_SIZE(Allocators[AC_RESOURCE],p_Rs.m_Height * p_Rs.m_Width * sizeof(ColorRGBA)) ColorRGBA;
			K15_MEMCPY(m_Pixels,p_Rs.m_Pixels,p_Rs.m_Width * p_Rs.m_Height * sizeof(ColorRGBA));
		}

		m_Width = p_Rs.m_Width;
		m_Height = p_Rs.m_Height;

		return *this;
	}
	/*********************************************************************************/
	bool Image::operator==(const Image& p_Rs)
	{
		if(m_Width == p_Rs.m_Width && m_Height == p_Rs.m_Height && 
			(m_Pixels && p_Rs.m_Pixels))
		{
			for(uint32 i = 0;i < getAmountPixels();++i)
			{
				if(m_Pixels[i] != p_Rs.m_Pixels[i])
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}
	/*********************************************************************************/
	bool Image::operator!=(const Image& p_Rs)
	{
		return !this->operator==(p_Rs);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace