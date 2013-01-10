/**
 * @file K15_DynamicLibrary.h
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

#ifndef __K15_DYNAMIC_LIBRARY__
#define __K15_DYNAMIC_LIBRARY__

#include "K15_StdInclude.h"
#include "K15_String.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API DynamicLibrary
	{
		friend class DynamicLibraryManager;

	public:
		~DynamicLibrary();

		/**
		* Get the filename of the library.
		*
		* @return String - filename of the library.

		*/
		const inline String &GetFileName() const;

		/**
		* Check wether the library is loaded or not.
		*
		* @return bool - true library is loaded and false if unloaded.
		*/
		inline bool IsLoaded() const;

		/**
		* Returns the internal handle of the library.
		*
		* @return U32 - Internal handle
		*/
		inline U32 GetHandle() const;

		/**
		* Returns a pointer to a symbol (function, etc) of a dynamic library.
		*
		* @param  pSymbolName - name of the symbol
		*
		* @return void* - pointer to the symbol or NULL if the symbol could not be found.
		*/
		void *GetSymbol(const char* pSymbolName);
	
	private:
		DynamicLibrary(const String &sFileName);

		bool _Load();
		bool _Unload();

	private:
		const String m_pFileName;
		U32 m_iInternalHandle;
		U32 m_iHandle;
		bool m_bLoaded;
	};
	#include "../src/K15_DynamicLibrary.inl"
}

#endif //__K15_DYNAMIC_LIBRARY__