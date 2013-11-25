/**
 * @file K15_IniFileParser.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/23
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
#include "K15_IniFileParser.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	IniFileParser::IniFileParser(const String& p_File)
		: m_Error(),
		  m_Values(),
		  m_Valid(true)
	{
		FileStream stream(p_File);

		if(!stream.is_open())
		{
			m_Error += StringUtil::format("Could not open *.ini file %s",p_File.c_str());
			m_Valid = false;
		}
		else
		{
			parseContent(stream);
			stream.close();
		}
	}
	/*********************************************************************************/
	IniFileParser::~IniFileParser()
	{

	}
	/*********************************************************************************/
	bool IniFileParser::getGroupEntries(const String& p_GroupName,IniFileGroup* p_Group) const
	{
		for(IniGroupList::const_iterator iter = m_Values.begin();iter != m_Values.end();++iter)
		{
			if(iter->name == p_GroupName)
			{
				*p_Group = (*iter);
				return true;
			}
		}

		return false;
	}
	/*********************************************************************************/
	bool IniFileParser::parseContent(FileStream& p_Stream)
	{
		static String line;
		static String groupName;
		static String value;
		static String key;
		static IniFileGroup group;
		static IniFileEntry entry;
		static char bufferline[256];
		bufferline[0] = '\0';

		String::size_type groupstart = 0;
		String::size_type groupend = 0;
		String::size_type equalPos = 0;
		
		int lineCount = 0;
		int lineEnd = 0;

		groupName = "";

		group.entries.clear();
		group.name = "";

		entry.key = "";
		entry.value = "";

		while(!p_Stream.eof())
		{
			++lineCount;
			p_Stream.getline(bufferline,256);
			line = bufferline;

			if(line.find_first_of("//") != String::npos) //comment?
			{
				continue;
			}
				
			groupstart = line.find_first_of('[');
			groupend = line.find_first_of(']');
			equalPos = line.find_first_of('=');
			if(groupstart < groupend)
			{
				if(group.entries.size() > 0)
				{
						m_Values.push_back(group);
				}

				groupstart += 1;
				groupName = line.substr(groupstart,groupend - groupstart);
				group.name = groupName;
			}
			else if(equalPos != String::npos)
			{
				line = StringUtil::removeWhitespaces(line);
				key = line.substr(0,equalPos);
				value = line.substr(equalPos+1);

				entry.key = key;
				entry.value = value;

				group.entries.push_back(entry);
			}
			else
			{
				m_Valid = false;
				m_Error += StringUtil::format("Error in line %i, (content:\"%s\")",lineCount,line.c_str());
			}
		}

		if(group.entries.size() > 0)
		{
			m_Values.push_back(group);
		}

		return m_Valid;
	}
	/*********************************************************************************/
	const String& IniFileParser::getValue(const String& p_Key) const
	{
		for(IniGroupList::const_iterator iter = m_Values.begin();iter != m_Values.end();++iter)
		{
			for(IniFileGroup::IniEntryList::const_iterator entry_iter = (*iter).entries.begin();entry_iter != (*iter).entries.end();++entry_iter)
			{
				if(entry_iter->key == p_Key)
				{
					return entry_iter->value;
				}
			}
		}

		return StringUtil::EMPTY;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace