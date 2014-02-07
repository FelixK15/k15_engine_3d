/**
 * @file K15_AllocatorSettings.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/23
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

#include "K15_AllocatorSettings.h"
#include "K15_IniFileParser.h"

#include "json\json.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const uint32 AllocatorSettings::DefaultAllocatorSize = 10 * MEGABYTE;
	bool AllocatorSettings::m_FileRead = false;
	AllocatorSettings::AllocatorSizeMap AllocatorSettings::m_AllocatorSizeMap;
	/*********************************************************************************/

	/*********************************************************************************/
	uint32 AllocatorSettings::getAllocatorSize(const String& p_Name)
	{
		if(_fileRead())
		{
			AllocatorSizeMap::iterator iter = m_AllocatorSizeMap.find(p_Name);

			if(iter != m_AllocatorSizeMap.end())
			{
				return iter->second;
			}
		}

		return DefaultAllocatorSize;
	}
	/*********************************************************************************/
	bool AllocatorSettings::_fileRead()
	{
		if(!m_FileRead)
		{
			String allocatorSize;
			IniFileParser parser(Application::SettingsFileName);

			K15_ASSERT(parser.isValid(),StringUtil::format("Error parsing ini file \"%s\". %s",Application::SettingsFileName.c_str(),
															parser.getError().c_str()));

			IniFileGroup group;
			K15_ASSERT(parser.getGroupEntries("AllocatorSettings",&group),StringUtil::format("Category \"AllocatorSettings\" not found in ini file \"%s\".",
												Application::SettingsFileName.c_str()));

			for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
			{
				allocatorSize = iter->value;
				
				m_AllocatorSizeMap.insert(Pair(String,uint32)(iter->key,_parseSize(allocatorSize)));
			}

			m_FileRead = true;
		}

		return m_FileRead;
	}
	/*********************************************************************************/
	uint32 AllocatorSettings::_parseSize(const String& p_SizeString)
	{
		static uint32 multiplicator = 1;
		uint32 size = 0;
		String formattedSize = StringUtil::toUpperString(p_SizeString);

		if(formattedSize.find("BYTE") != String::npos)
		{
			multiplicator = 1;
		}
		else if(formattedSize.find("KB") != String::npos)
		{
			multiplicator = 1024;
		}
		else if(formattedSize.find("MB") != String::npos)
		{
			multiplicator = 1024 * 1024;
		}
		else if(formattedSize.find("GB") != String::npos)
		{
			multiplicator = 1024 * 1024 * 1024;
		}
		else
		{
			_LogError("Unrecognized token in group \"AllocatorSettings\":\"%s\"",formattedSize.c_str());
		}

		size = multiplicator * StringUtil::toUInt(formattedSize);

		return size;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace