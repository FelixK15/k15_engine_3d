/**
 * @file K15_FontManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/14
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
#include "K15_FontManager.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	FontManager::FontManager()
		: Singleton(),
		  PoolAllocator(ApplicationAllocator,50,_ON(FontManagerAllocator)),
		  m_FreeType(0)
	{

	}
	/*********************************************************************************/
	FontManager::~FontManager()
	{

	}
	/*********************************************************************************/
	bool FontManager::init()
	{
		K15_ASSERT(!m_FreeType,"FontManager already initialized!.");

		FT_Init_FreeType(m_FreeType);
	}
	/*********************************************************************************/
	void FontManager::shutdown()
	{
		FT_Done_FreeType(*m_FreeType);
	}
	/*********************************************************************************/
	FT_Face* FontManager::createFontFromMemory(const byte* p_Data,uint32 p_Size)
	{
		FT_Face* face = 0;
	
		FT_New_Memory_Face(*m_FreeType,(const FT_Byte*)p_Data,p_Size,0,face);
		return face;
	}
	/*********************************************************************************/
	FT_Face* FontManager::createFontFromFile(const String& p_FileName)
	{
		FT_Face* face = 0;

		FT_New_Face(*m_FreeType,p_FileName.c_str(),0,face);
		return face;
	}
	/*********************************************************************************/
	void FontManager::destroyFace(FT_Face* p_Face)
	{
		FT_Done_Face(*p_Face);
		p_Face = 0;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace