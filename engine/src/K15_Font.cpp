/**
 * @file K15_Font.cpp
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_Font.h"
#include "K15_ResourceManager.h"
#include "K15_TrueTypeFont.h"

#include "tinyxml2.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	Font::Font()
		: ResourceBase(),
		  m_StartGlyph(33),
		  m_EndGlyph(166),
		  m_GlyphSize(12),
		  m_Resolution(72)
	{

	}
	/*********************************************************************************/
	Font::~Font()
	{

	}
	/*********************************************************************************/
	void Font::loadDebug(RawData& p_Data)
	{
		static const char* debugFont =  "<FontDefintion>\n"
			"<FontFile>comic.ttf</FontFile>\n"
			"<Size>12</Size>\n"
			"<StartGlyph>33</StartGlyph>\n"
			"<EndGlyph>122</EndGlyph>\n"
			"<Resolution>72</Resolution>\n"
			"</FontDefinition>";

		p_Data.data = (byte*)debugFont;
		p_Data.size = strlen(debugFont);
	}
	/*********************************************************************************/
	bool Font::internalLoad(const RawData& p_Data)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError err = tinyxml2::XML_NO_ERROR;
		if((err = doc.Parse((const char*)p_Data.data,p_Data.size)) != tinyxml2::XML_NO_ERROR)
		{
			_LogError("Can't parse Font %s. Error:%s | %s",m_AssetName.c_str(),doc.GetErrorStr1(),doc.GetErrorStr2());
			return false;
		}

		tinyxml2::XMLNode* node = doc.FirstChild();
	
		if(node && strcmp(node->Value(),"FontDefinition"))
		{
			node = node->FirstChild();
			while(node)
			{
				if(strcmp(node->Value(),"FontFile"))
				{
					m_FontName = node->ToElement()->GetText();
					m_Font = ResourceManager::getInstance()->getResource<TrueTypeFont>(m_FontName,RP_HIGH);
				}
				else if(strcmp(node->Value(),"Size"))
				{
					m_GlyphSize = StringUtil::stringToNumeric<uint8>(node->ToElement()->GetText());
				}
				else if(strcmp(node->Value(),"StartGlyph"))
				{
					m_StartGlyph = StringUtil::stringToNumeric<uint8>(node->ToElement()->GetText()); 
				}
				else if(strcmp(node->Value(),"EndGlyph"))
				{
					m_EndGlyph = StringUtil::stringToNumeric<uint8>(node->ToElement()->GetText());
				}
				else if(strcmp(node->Value(),"Resolution"))
				{
					m_Resolution = StringUtil::stringToNumeric<uint8>(node->ToElement()->GetText());
				}

				node = node->NextSibling();
			}

			return m_Font.isValid();
		}
		else if(!node)
		{
			_LogError("Invalid xml file %s",m_AssetName);
			return false;
		}
		else
		{
			_LogError("Invalid xml font file %s. First child is not \"FontDefinition\".",m_AssetName);
			return false;
		}

		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace