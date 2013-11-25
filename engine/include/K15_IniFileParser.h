/**
 * @file K15_IniFileParser.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_IniFileParser_h_
#define _K15Engine_Core_IniFileParser_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	struct K15_CORE_API IniFileEntry
	{
		String key;
		String value;
	}; //end of IniFileEntry class declaration
	/*********************************************************************************/
	struct K15_CORE_API IniFileGroup
	{
		/*********************************************************************************/
		typedef List(IniFileEntry) IniEntryList;
		/*********************************************************************************/

		String name;
		IniEntryList entries;
	};
	/*********************************************************************************/
	class K15_CORE_API IniFileParser
	{
	public:
		/*********************************************************************************/
		typedef List(IniFileGroup) IniGroupList;
		/*********************************************************************************/
	public:
		IniFileParser(const String& p_File);
		~IniFileParser();

		bool getGroupEntries(const String& p_GroupName,IniFileGroup* p_Group) const;
		const String& getValue(const String& p_Key) const;

		inline bool isValid() const;
		inline const String& getError() const;

	private:
		bool parseContent(FileStream& p_Stream);

	private:
		IniGroupList m_Values;
		String m_Error;
		bool m_Valid;
	};// end of IniFileParser class declaration
#	include "K15_IniFileParser.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_IniFileParser_h_