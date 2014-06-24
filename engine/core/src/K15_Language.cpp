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


#include "json\json.h"

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
	Language::LanguageMap Language::m_LanguageMap = createTestLangMap();
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
	void Language::setCurrentLanguageID(Enum p_LanguageID)
	{
		K15_ASSERT(p_LanguageID < Language::LID_COUNT,StringUtil::format("Invalid language ID \"%u\".",p_LanguageID));
		m_CurrentLanguage = p_LanguageID;

		K15_LOG_NORMAL("Language changed to \"%s\".",LanguageString[p_LanguageID].c_str());
	}
	/*********************************************************************************/
	void Language::serialize()
	{
		Json::Reader reader;
		Json::Value root;
		FileStream jsonFile("language.json", FileStream::in);
		String content;

		if(jsonFile.is_open())
		{
			if(!reader.parse(jsonFile,root,false))
			{
				printf(reader.getFormattedErrorMessages().c_str());
				return;
			}
		}
		else
		{
			return;
		}


		const Json::Value languages = root.get("languages",0);


		for(Json::ValueIterator iter = languages.begin();iter != languages.end();++iter)
		{
			String lang = iter.memberName();
			Enum langID = 0;
			for(uint32 i = LID_ENGLISH;i < LID_COUNT;++i)
			{
				if(LanguageString[i] == lang)
				{
					langID = i;
					break;
				}
			}

			Language::StringMap& currentStringMap = m_LanguageMap[langID];
			const Json::Value langStrings = languages[lang];
			for(Json::ValueIterator sIter = langStrings.begin();sIter != langStrings.end();++sIter)
			{
				ObjectName key = sIter.memberName();
				String value = (*sIter).asString();
				currentStringMap[key] = value;
			}
		}
	}
	/*********************************************************************************/
	void Language::deserialize()
	{
		Json::StyledWriter writer;
		Json::Value root;
		Json::Value& languages = root["languages"];
		for(Language::LanguageMap::iterator iter = m_LanguageMap.begin();iter != m_LanguageMap.end();++iter)
		{
			Language::StringMap& stringMap = iter->second;
			Json::Value& language = languages[Language::getLanguageString(iter->first)];

			for(Language::StringMap::iterator sIter = stringMap.begin();sIter != stringMap.end();++sIter)
			{
				language[sIter->first.c_str()] = sIter->second;
			}
		}
		
		String content = writer.write(root);

		FileStream fStream("language.json",FileStream::trunc | FileStream::out);

		if(fStream.is_open())
		{
			fStream << content;
		}

		fStream.flush();
		fStream.close();

		m_LanguageMap.clear();
	}
	/*********************************************************************************/
	Language::LanguageMap Language::createTestLangMap()
	{
		LanguageMap map;
		StringMap english;
		StringMap german;

		english["ID_HELLO"] = "hello";
		english["ID_BYE"] = "bye";
		english["ID_THANKS"] = "thanks";

		german["ID_HELLO"] = "hallo";
		german["ID_BYE"] = "tschüss";
		german["ID_THANKS"] = "danke";

		map[Language::LID_ENGLISH] = english;
		map[Language::LID_GERMAN] = german;

		return map;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace