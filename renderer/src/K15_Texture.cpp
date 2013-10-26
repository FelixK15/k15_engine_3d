/**
 * @file K15_Texture.cpp
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

#include "K15_RendererPrecompiledHeader.h"
#include "K15_Texture.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	Texture::Texture()
		: m_Impl(0),
		  m_MipMapCount(0),
		  m_PixelFormat(0),
		  m_TextureType(0),
		  m_TextureUsage(0),
		  m_Width(0),
		  m_Height(0),
		  m_Depth(0)
	{

	}
	/*********************************************************************************/
	Texture::~Texture()
	{

	}
	/*********************************************************************************/
	void Texture::setTextureType(Enum p_TextureType)
	{
		m_TextureType = p_TextureType;
		m_Impl->setTextureType(p_TextureType);
	}
	/*********************************************************************************/
	void Texture::setTextureUsage(Enum p_TextureUsage)
	{
		m_TextureUsage = p_TextureUsage;
		m_Impl->setTextureUsage(m_TextureUsage);
	}
	/*********************************************************************************/
	void Texture::setMipMapCount(uint8 p_MipMapCount)
	{
		m_MipMapCount = p_MipMapCount;
		m_Impl->setMipMapCount(m_MipMapCount);
	}
	/*********************************************************************************/
	void Texture::setHeight(uint32 p_Height)
	{
		m_Height = p_Height;
		m_Impl->setHeight(m_Height);
	}
	/*********************************************************************************/
	void Texture::setWidth(uint32 p_Width)
	{
		m_Width = p_Width;
		m_Impl->setWidth(m_Width);
	}
	/*********************************************************************************/
	void Texture::setResolution(const Resolution& p_Resolution)
	{
		m_Width = p_Resolution.width;
		m_Height = p_Resolution.height;

		m_Impl->setWidth(m_Width);
		m_Impl->setHeight(m_Height);
	}
	/*********************************************************************************/
	void Texture::setDepth(uint32 p_Depth)
	{
		m_Depth = p_Depth;
		m_Impl->setDepth(p_Depth);
	}
	/*********************************************************************************/
	void Texture::setPixelFormat(Enum p_PixelFormat)
	{
		m_PixelFormat = p_PixelFormat;
		m_Impl->setPixelFormat(p_PixelFormat);
	}
	/*********************************************************************************/
	bool Texture::hasAlpha() const
	{
		return m_Impl->hasAlpha();
	}
}}//end of K15_Engine::Rendering namespace