/**
 * @file K15_DynamicLibrary_win32.h
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

#ifndef _K15Engine_Core_DynamicLibrary_win32_h_
#define _K15Engine_Core_DynamicLibrary_win32_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_DynamicLibraryBase.h"

namespace K15_Engine { namespace Core { 

	class K15_API_EXPORT DynamicLibrary_Win32 : public DynamicLibraryBase
	{
	public:
		DynamicLibrary_Win32();
		DynamicLibrary_Win32(const String& p_FileName);
		virtual ~DynamicLibrary_Win32();

	protected:
		virtual bool load();
		virtual bool unload();

		virtual void* getSymbolInternal( const String& p_SymbolName );

	protected:
		HMODULE m_Module;
	};// end of DynamicLibrary_Win32 class
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_DynamicLibrary_win32_h_