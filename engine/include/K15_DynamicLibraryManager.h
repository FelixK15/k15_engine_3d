/**
 * @file K15_DynamicLibraryManager.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_DYNAMIC_LIBRARY_MANAGER__
#define __K15_DYNAMIC_LIBRARY_MANAGER__

#include "K15_StdInclude.h"
#include "K15_Singleton.h"
#include "K15_DynamicArray.h"
#include "K15_DynamicLibrary.h"

#define g_pDynamicLibraryManager K15_EngineV2::DynamicLibraryManager::GetInstance()

namespace K15_EngineV2
{
	class DynamicLibrary;

	class K15ENGINE2_API DynamicLibraryManager : public Singleton<DynamicLibraryManager>
	{
	public:
		DynamicLibraryManager();
		virtual ~DynamicLibraryManager();

		/**
		* Tries to load a dynamic library (*.dll,*.so,*.dynlib).
		*
		* @param  pFileName - dynamic library file
		*
		* @return DynamicLibrary - returns a pointer to a DynamicLibrary file if the library could get loaded without problems. returns NULL if problems occured.
		*/
		DynamicLibrary *Load(const String &pFileName);
		/**
		* Tries to unload a previously loaded library.
		*
		* @param  pDynLib - pointer to the DynamicLibrary that you want to unload.
		*
		* @return bool - returns true if the library could be unloaded successfully, false if not.
		* @note Memory will be cleared internally. You don't need to free the memory of the DynamicLibrary yourself.
		*/
		bool Unload(DynamicLibrary *pDynLib);

	private:
		DynamicArray<DynamicLibrary*> m_arrDynLibs;
	};
}

#endif //__K15_DYNAMIC_LIBRARY_MANAGER__