/**
 * @file K15_Font.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/12
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

#include "K15_Font.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	static const int32 NOT_FOUND = -1;
	K15_IMPLEMENT_RTTI_BASE(Core,Font,ResourceBase);
	/*********************************************************************************/

	/*********************************************************************************/
	Font::Font()
		: ResourceBase(),
		m_FontTexture(0),
		m_Letters()
	{

	}
	/*********************************************************************************/
	Font::~Font()
	{

	}
	/*********************************************************************************/
	const Font::Letter& Font::getLetter(char p_Character) const
	{
		int32 index = _getLetterIndex(p_Character);
		K15_ASSERT(index != NOT_FOUND,
			StringUtil::format("Could not find unicode letter \"%c\" for font \"%s\".",p_Character,getName().c_str()));

		return m_Letters[index];
	}
	/*********************************************************************************/
	int32 Font::_getLetterIndex(char p_Character) const
	{
		for(uint32 i = 0;i < m_Letters.size();++i)
		{
			if(m_Letters[i].character == p_Character)
			{
				return i;
			}
		}

		return NOT_FOUND;
	}
	/*********************************************************************************/
	void Font::addLetter(const Letter& p_Letter)
	{
		if(_getLetterIndex(p_Letter.character) == NOT_FOUND)
		{
			m_Letters.push_back(p_Letter);
		}
		else
		{
			_LogWarning(TEXT("Letter data for letter \"%c\" has already been added to font \"%s\". Skipping letter."),
				p_Letter.character,getName().c_str());
		}
	}
	/*********************************************************************************/
	void Font::setTexture(Texture* p_Texture)
	{
		K15_ASSERT(p_Texture,"Texture is NULL!");

		m_FontTexture = p_Texture;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace