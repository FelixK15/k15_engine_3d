/**
 * @file K15_DynamicLibraryManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#include "K15_DynamicLibraryManager.h"
#include "K15_LogManager.h"

#if defined K15_OS_WINDOWS
#	include "K15_DynamicLibrary_win32.h"
#endif

namespace K15_Engine { namespace System {
	/*********************************************************************************/
	DynamicLibraryManager::DynamicLibraryManager()
		: StackAllocator(MemoryAllocator,5 * MEGABYTE),
		  m_LoadedLibs()
	{
	
	}
	/*********************************************************************************/
	DynamicLibraryManager::~DynamicLibraryManager()
	{
		for(DynamicLibraryMap::iterator iter = m_LoadedLibs.begin();iter != m_LoadedLibs.end();++iter)
		{
			K15_DELETE iter->second;
		}

		m_LoadedLibs.clear();
	}
	/*********************************************************************************/
	DynamicLibraryBase *DynamicLibraryManager::load(const String& p_FileName)
	{
		DynamicLibraryMap::iterator iter = m_LoadedLibs.find(p_FileName);

		if (iter != m_LoadedLibs.end())
		{
			K15_LogNormalMessage(String("Library already loaded. Returning cached libray. Library:") + p_FileName);
			return iter->second;
		}

		DynamicLibraryBase *library = K15_NEW DynamicLibraryType();

		if(!library->load())
		{
			K15_LogNormalMessage(String("Could not load library :") + p_FileName);
	
			K15_DELETE library;
			return 0;
		}

		m_LoadedLibs.insert(K15_Pair(String,DynamicLibraryBase*)(p_FileName,library));

		return library;
	}
	/*********************************************************************************/
	bool DynamicLibraryManager::unload(DynamicLibraryBase *p_Lib)
	{
		if(p_Lib->unload())
		{
			//m_LoadedLibs.
		}

		return false;
	}
	/*********************************************************************************/
}}// end of K15_Engine::System namespace