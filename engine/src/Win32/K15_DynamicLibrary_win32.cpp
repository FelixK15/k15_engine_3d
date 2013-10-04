/**
 * @file K15_DynamicLibrary_win32.cpp
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

#include "K15_DynamicLibrary_win32.h"
#include "K15_LogManager.h"

namespace K15_Engine { namespace System { 
  /*********************************************************************************/
  DynamicLibrary_Win32::DynamicLibrary_Win32(const String& p_FileName)
    : DynamicLibraryBase(p_FileName),
    m_Module(0)
  {

  }
  /*********************************************************************************/
  DynamicLibrary_Win32::~DynamicLibrary_Win32()
  {
    if(isLoaded()){
      unload();
    }
  }
  /*********************************************************************************/
  bool DynamicLibrary_Win32::load()
  {
    if(isLoaded())
    {
      m_Module = LoadLibrary(getFileName().c_str());
      if(!m_Module){
        K15_LogNormalMessage(g_pSystem->GetSystemError());
        return false;
      }

      m_Loaded = true;
      return true;
    }

    return false;
  }
  /*********************************************************************************/
  bool DynamicLibrary_Win32::unload()
  {
    if(isLoaded()){
      BOOL bResult = FALSE;
      bResult = FreeLibrary(m_Module);

      if(!bResult){
        K15_LogNormalMessage(g_pSystem->GetSystemError());
        return false;
      }

      m_Loaded = false;
      return true;
    }else{
      K15_LogNormalMessage(String("Trying to load already unloaded library - ") + GetFileName());
      return false;
    }
  }
  /*********************************************************************************/
  void* DynamicLibrary_Win32::getSymbolInternal(const String& p_SymbolName)
  {
    if(isLoaded())
    {
      FARPROC pSymbol = GetProcAddress(m_Module,p_SymbolName.c_str());

      if(!pSymbol)
      {
        K15_LogNormalMessage(g_pSystem->GetSystemError());
        return 0;
      }

      return (void*)pSymbol;
    }
    else
    {
      K15_LogNormalMessage(String("Trying to load symbol from unloaded library - Symbol:") + pSymbolName);
      K15_LogNormalMessage(String("Library:") + GetFileName());	
    }

    return 0;
  }
  /*********************************************************************************/
}}//end of K15_Engine::System namespace