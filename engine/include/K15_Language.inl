/**
 * @file K15_Language.inl
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

/*********************************************************************************/
Enum Language::getCurrentLanguageID()
{
	return m_CurrentLanguage;
}
/*********************************************************************************/
const String& Language::getCurrentLanguageString()
{
	return LanguageString[m_CurrentLanguage];
}
/*********************************************************************************/
const String& Language::getLanguageString(Enum p_LanguageID)
{
	K15_ASSERT(p_LanguageID < Language::LID_COUNT,StringUtil::format("Invalid language ID \"%u\".",p_LanguageID));
	return LanguageString[p_LanguageID];
}
/*********************************************************************************/
const String& Language::getStringForCurrentLanguage(const ObjectName& p_StringID)
{
	return getStringForLanguage(p_StringID,m_CurrentLanguage);
}
/*********************************************************************************/
const String& Language::getStringForLanguage(const ObjectName& p_StringID, Enum p_LanguageID)
{
	K15_ASSERT(p_LanguageID < Language::LID_COUNT,StringUtil::format("Invalid language ID \"%u\".",p_LanguageID));
	StringMap& languageMap = m_LanguageMap[p_LanguageID];
	StringMap::iterator iter = languageMap.find(p_StringID);

	if(iter == languageMap.end())
	{
		return p_StringID.toString();
	}

	return iter->second;
}
/*********************************************************************************/
void Language::setCurrentLanguageString(const String& p_String)
{
	for(int i = 0;i < Language::LID_COUNT;++i)
	{
		const String& lang = LanguageString[i];

		if(lang == p_String)
		{
			setCurrentLanguageID(i);
			return;
		}
	}
}
/*********************************************************************************/