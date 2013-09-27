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

#ifndef __K15Engine_System_DynamicLibrary_h_
#define __K15Engine_System_DynamicLibrary_h_

#include "K15_Prerequisites.h"
#include "K15_DynamicLibraryManager.h"
#include "K15_AllocatedObject.h"
#include "K15_Functor.h"

namespace K15_Engine { namespace System {

	class K15_API_EXPORT DynamicLibraryBase : public DynamicLibraryManagerAllocatedObject
	{

	public:
		DynamicLibraryBase();
		DynamicLibraryBase(const String& p_FileName);

		virtual ~DynamicLibraryBase();

		/**
		* Get the filename of the library.
		*
		* @return String - filename of the library.

		*/
		const inline String &getFileName() const;

		void setFileName(const String& p_FileName);

		/**
		* Check whether the library is loaded or not.
		*
		* @return bool - true library is loaded and false if unloaded.
		*/
		inline bool isLoaded() const;

		/**
		* Returns a pointer to a symbol (function, etc) of a dynamic library.
		*
		* @param  pSymbolName - name of the symbol
		*
		* @return void* - pointer to the symbol or NULL if the symbol could not be found.
		*/
		template<class ReturnType>
		Functor0<ReturnType> getSymbol(const String& p_SymbolName);

	public:
		virtual bool load() = 0;

		virtual bool unload() = 0;
	
	protected:
		virtual void* getSymbolInternal(const String& p_SymbolName) = 0;

	protected:
		String m_FileName;
		bool m_Loaded;
	};
	#include "K15_DynamicLibraryBase.inl"
}} //end of K15_Engine::System namespace

#endif //__K15_DYNAMIC_LIBRARY__