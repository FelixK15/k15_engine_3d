/**
 * @file K15_TrueTypeFont.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/13
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
#include "K15_TrueTypeFont.h"
#include "K15_FontManager.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	TrueTypeFont::TrueTypeFont()
		: m_Face(0)
	{
		
	}
	/*********************************************************************************/
	TrueTypeFont::~TrueTypeFont()
	{
		if(m_Face)
		{
			g_FontManager->destroyFace(m_Face);
		}
	}
	/*********************************************************************************/
	void TrueTypeFont::loadDebug(RawData& p_Data)
	{
		
	}
	/*********************************************************************************/
	bool TrueTypeFont::internalLoad(const RawData& p_Data)
	{
		return (m_Face = g_FontManager->createFontFromMemory(p_Data.data,p_Data.size)) != 0;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace