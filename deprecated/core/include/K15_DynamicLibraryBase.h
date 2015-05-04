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

#ifndef _K15Engine_Core_DynamicLibrary_h_
#define _K15Engine_Core_DynamicLibrary_h_

#include "K15_Prerequisites.h"
#include "K15_DynamicLibraryManager.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Core {

	class K15_CORE_API DynamicLibraryBase : public DynamicLibraryAllocatedObject
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
		const String &getFileName() const;

		void setFileName(const String& p_FileName);

		/**
		* Check whether the library is loaded or not.
		*
		* @return bool - true library is loaded and false if unloaded.
		*/
		bool isLoaded() const;

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
		virtual bool load(){return false;}
		virtual bool unload() = 0;
	
	protected:
		virtual void* getSymbolInternal(const String& p_SymbolName) = 0;

	protected:
		String m_FileName;
		bool m_Loaded;
	};// end of DynamicLibraryBase class
	#include "K15_DynamicLibraryBase.inl"
}} //end of K15_Engine::Core namespace

#endif //__K15_DYNAMIC_LIBRARY__