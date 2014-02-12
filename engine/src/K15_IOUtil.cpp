/**
 * @file K15_IOUtil.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#include "K15_IOUtil.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	String IOUtil::readWholeFile(const String& p_FileName)
	{
		FileStream stream(p_FileName);
		
		K15_ASSERT(stream.is_open(),StringUtil::format("Could not open file \"%s\".",p_FileName.c_str()));

		return (String((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>())));
	}
	/*********************************************************************************/
	uint32 IOUtil::getFileSize(const String& p_FileName)
	{
		struct stat filestatus;
		::stat(p_FileName.c_str(),&filestatus);
		//http://linux.die.net/man/2/stat
		
		return filestatus.st_size;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace