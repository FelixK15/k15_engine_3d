/**
 * @file K15_Language.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#include "K15_Language.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const String Language::LanguageString[LID_COUNT] = {
		"English",	//LID_ENGLISH
		"German",	//LID_GERMAN
		"Spanish",	//LID_SPANISH
		"Italian",	//LID_ITALIAN
		"French"	//LID_FRENCH
	}; //LanguageString
	/*********************************************************************************/
	Enum Language::m_CurrentLanguage = Language::LID_ENGLISH;
	Language::LanguageMap Language::m_LanguageMap = Language::LanguageMap();
	/*********************************************************************************/

	/*********************************************************************************/
	Language::Language()
	{

	}
	/*********************************************************************************/
	Language::~Language()
	{

	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace