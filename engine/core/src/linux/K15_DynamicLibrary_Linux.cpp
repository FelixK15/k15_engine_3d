/**
 * @file K15_DynamicLibrary_Linux.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/03/02
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

#ifdef K15_OS_LINUX

#ifdef K15_OS_ANDROID
#include "Android\K15_OSLayer_Android.h"
#endif //K15_OS_ANDROID

#include "linux/K15_DynamicLibrary_Linux.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	DynamicLibrary_Linux::DynamicLibrary_Linux()
	{

	}
	/*********************************************************************************/
	DynamicLibrary_Linux::DynamicLibrary_Linux(const String& p_FileName)
		: DynamicLibraryBase(p_FileName),
		m_Module(0)
	{

	}
	/*********************************************************************************/
	DynamicLibrary_Linux::~DynamicLibrary_Linux()
	{

	}
	/*********************************************************************************/
	bool DynamicLibrary_Linux::load()
	{
	#if defined K15_OS_ANDROID
		String path = "lib" + m_FileName;
	#else
		String path = g_Application->getGameRootDir() + m_FileName;
	#endif //K15_OS_ANDROID
		K15_LOG_NORMAL("Trying to load \"%s\".",path.c_str());
		m_Module = dlopen(path.c_str(),RTLD_NOW);

		if(!m_Module)
		{
			return false;
		}

		m_Loaded = true;

		return true;
	}
	/*********************************************************************************/
	bool DynamicLibrary_Linux::unload()
	{
		dlclose(m_Module);
	}
	/*********************************************************************************/
	void* DynamicLibrary_Linux::getSymbolInternal( const String& p_SymbolName )
	{
		return dlsym(m_Module,p_SymbolName.c_str());
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace

#endif //K15_OS_LINUX
