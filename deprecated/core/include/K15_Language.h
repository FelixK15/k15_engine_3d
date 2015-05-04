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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_Language_h_
#define _K15Engine_Core_Language_h_

#include "K15_Prerequisites.h"
#include "K15_HashedString.h"

#define LANG_STRING(ID) K15_Engine::Core::Language::getStringForCurrentLanguage(_ON(ID))
#define C_LANG_STRING(ID) K15_Engine::Core::Language::getStringForCurrentLanguage(_ON(ID)).c_str()

namespace K15_Engine { namespace Core {
	class K15_CORE_API Language
	{
	public:
		/*********************************************************************************/
		enum LanguageID
		{
			LID_ENGLISH = 0,
			LID_GERMAN,
			LID_SPANISH,
			LID_ITALIAN,
			LID_FRENCH,

			LID_COUNT
		}; // LanguageID
		/*********************************************************************************/
		static const String LanguageString[LID_COUNT];
		/*********************************************************************************/
		typedef HashMap(ObjectName,String) StringMap;
		typedef HashMap(Enum,StringMap) LanguageMap;
		/*********************************************************************************/

	public:
		Language();
		~Language();

		INLINE static Enum getCurrentLanguageID();
		INLINE static const String& getCurrentLanguageString();
		INLINE static const String& getLanguageString(Enum p_LanguageID);

		INLINE static const String& getStringForCurrentLanguage(const ObjectName& p_StringID);
		INLINE static const String& getStringForLanguage(const ObjectName& p_StringID, Enum p_LanguageID);

		INLINE static void setCurrentLanguageString(const String& p_String);
		static void setCurrentLanguageID(Enum p_LanguageID);
		
		static void serialize();
		static void deserialize();

		static LanguageMap createTestLangMap();

	private:
		static Enum m_CurrentLanguage;
		static LanguageMap m_LanguageMap;
	};// end of Language class declaration
#	include "K15_Language.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Language_h_