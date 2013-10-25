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
#ifndef _K15Engine_Core_DynamicLibraryManager_h_
#define _K15Engine_Core_DynamicLibraryManager_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#	include "K15_Singleton.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_DynamicLibraryBase.h"

namespace K15_Engine { namespace Core {

	class DynamicLibraryManager : public ApplicationAllocatedObject,
								  public Singleton<DynamicLibraryManager>,
								  public StackAllocator
	{
	public:
		typedef HashMap(String,DynamicLibraryBase*) DynamicLibraryMap;

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
		DynamicLibraryBase* load(const String& p_FileName);
		/**
		* Tries to unload a previously loaded library.
		*
		* @param  pDynLib - pointer to the DynamicLibrary that you want to unload.
		*
		* @return bool - returns true if the library could be unloaded successfully, false if not.
		* @note Memory will be cleared internally. You don't need to free the memory of the DynamicLibrary yourself.
		*/
		bool unload(DynamicLibraryBase* p_Lib);

	private:
		DynamicLibraryMap m_LoadedLibs; //caching dynamic libs doesnt make much sense - reloading would be more sufficient
	};
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_DynamicLibraryManager_h_