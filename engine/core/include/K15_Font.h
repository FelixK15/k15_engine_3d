/**
 * @file K15_Font.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/12
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Rendering_Font_h_
#define _K15Engine_Rendering_Font_h_

#include "K15_Prerequisites.h"
#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API Font : public ResourceBase, public GeneralAllocatedObject
	{
		K15_DECLARE_RTTI;

	public:
		struct Letter
		{
			float left;
			float right;
			float top;
			float bottom;

			char character;
		}; // end of Letter struct declaration
		typedef DynamicArray(Letter) LetterArray;
	public:
		Font();
		~Font();

		const Letter& getLetter(char p_Character) const;
		void addLetter(const Letter& p_Letter);
		void setTexture(Texture* p_Texture);

		INLINE Texture* getTexture() const;
	
	private:
		int32 _getLetterIndex(char p_Character) const;

	private:
		Texture* m_FontTexture;
		LetterArray m_Letters;
	};// end of Font class declaration
#	include "K15_Font.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_Font_h_