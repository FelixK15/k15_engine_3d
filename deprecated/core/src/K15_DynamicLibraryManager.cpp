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

#include "K15_PrecompiledHeader.h"

#include "K15_DynamicLibraryManager.h"

#include "K15_Functor.h"

#if defined K15_OS_WINDOWS
#	include "win32/K15_DynamicLibrary_win32.h"
#elif defined K15_OS_ANDROID || defined K15_OS_LINUX || defined K15_OS_APPLE
#	include "linux/K15_DynamicLibrary_Linux.h"
#endif //K15_OS_WINDOWS

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	DynamicLibraryManager::DynamicLibraryManager()
		: m_LoadedLibs()
	{
		K15_LOG_NORMAL_MESSAGE("Initializing DynamicLibraryManager...");
	}
	/*********************************************************************************/
	DynamicLibraryManager::~DynamicLibraryManager()
	{
		DynamicLibraryBase* lib = 0;
		while(m_LoadedLibs.size() > 0)
		{
			if((lib = m_LoadedLibs.begin()->second) != 0)
			{
				if(lib->isLoaded())
				{
					unload(lib); //will get erase from the map in here
				}
				else
				{
					m_LoadedLibs.erase(lib->getFileName());
				}
				K15_DELETE lib;
			}
		}
		
		m_LoadedLibs.clear();
	}
	/*********************************************************************************/
	DynamicLibraryBase *DynamicLibraryManager::load(const String& p_FileName)
	{
		DynamicLibraryMap::iterator iter = m_LoadedLibs.find(p_FileName);
	
		if (iter != m_LoadedLibs.end())
		{
			K15_LOG_NORMAL_MESSAGE("Library \"%s\" already loaded. Returning cached libray.",p_FileName.c_str());
			return iter->second;
		}

		
		DynamicLibraryBase *library = K15_NEW DynamicLibraryType(p_FileName);

		if(!library->load())
		{
            K15_LOG_ERROR_MESSAGE("Could not load library (\"%s\") Error:%s",library->getFileName().c_str(),OSLayer::getError().c_str());
	
			K15_DELETE library;
			return 0;
		}

        K15_LOG_SUCCESS_MESSAGE("Successfully load library \"%s\".",library->getFileName().c_str());

        m_LoadedLibs.insert(Pair(String,DynamicLibraryBase*)(library->getFileName(),library));

		return library;
	}
	/*********************************************************************************/
	bool DynamicLibraryManager::unload(DynamicLibraryBase *p_Lib)
	{
		//check if the lib has a pluginUnload function...this is true for plugins
		Functor0<void> unloadFunc = p_Lib->getSymbol<void>("pluginUnload");
		if(unloadFunc.isValid())
		{
			unloadFunc();
		}

		K15_LOG_NORMAL_MESSAGE("Unloading dynamic library \"%s\".",p_Lib->getFileName().c_str());
		if(p_Lib->unload())
		{
			K15_LOG_SUCCESS_MESSAGE("Successfully unloaded dynamic library \"%s\".",p_Lib->getFileName().c_str());
			m_LoadedLibs.erase(p_Lib->getFileName());
			return true;
		}

		return false;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
