/**
 * @file K15_DynamicLibrary.cpp
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

#include "K15_DynamicLibrary.h"
#include "K15_DynamicArray.h"
#include "K15_LogManager.h"
#include "Windows.h"

using namespace K15_EngineV2;

namespace {
	DynamicArray<HMODULE> _arrModules;
}

DynamicLibrary::DynamicLibrary(const String& sFileName)
	: m_pFileName(sFileName)
{

}

DynamicLibrary::~DynamicLibrary()
{
	if(IsLoaded()){
		_Unload();
	}
}

bool DynamicLibrary::_Load()
{
	HMODULE pModule = LoadLibrary(m_pFileName.C_Str());
	if(!pModule){
		WriteDefaultLog(g_pSystem->GetSystemError());
		return false;
	}

	_arrModules.PushBack(pModule);
	m_iHandle = _arrModules.Size() - 1;
	m_bLoaded = true;
	return true;
}

bool DynamicLibrary::_Unload()
{
	assert(m_bLoaded);

	if(m_bLoaded){
		BOOL bResult = FALSE;
		bResult = FreeLibrary(_arrModules[m_iHandle]);

		if(!bResult){
			WriteDefaultLog(g_pSystem->GetSystemError());
			return false;
		}

		_arrModules[m_iHandle] = NULL;
		m_bLoaded = false;
		return true;
	}else{
		WriteDefaultLog(String("Trying to load already unloaded library - ") + GetFileName());
		return false;
	}
}

void *DynamicLibrary::GetSymbol(const char* pSymbolName)
{
	assert(m_bLoaded);

	if(m_bLoaded){
		FARPROC pSymbol = GetProcAddress(_arrModules[m_iHandle],pSymbolName);

		if(!pSymbol){
			WriteDefaultLog(g_pSystem->GetSystemError());
			return NULL;
		}

		return pSymbol;

	}else{
		WriteDefaultLog(String("Trying to load symbol from already unloaded library - Symbol:") + pSymbolName);
		WriteDefaultLog(String("Library:") + GetFileName());
		return false;
	}
}