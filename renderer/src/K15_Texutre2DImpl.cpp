/**
 * @file K15_Texture2DImpl.h
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

#include "K15_Texture2DImpl.h"
using namespace K15_Renderer;

Texture2DImpl::Texture2DImpl()
	:	m_iAmountMipMaps(0),
		m_iHeight(0),
		m_iWidth(0),
		m_bAlpha(false)
{

}

Texture2DImpl::~Texture2DImpl()
{

}
