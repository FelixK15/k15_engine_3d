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

#include "K15_DynamicLibraryManager.h"
#include "K15_LogManager.h"

using namespace K15_EngineV2;

DynamicLibraryManager::DynamicLibraryManager()
{

}

DynamicLibraryManager::~DynamicLibraryManager()
{
	for(U32 i = 0;i < m_arrDynLibs.Size();++i){
		K15_DELETE m_arrDynLibs[i];
		m_arrDynLibs[i] = NULL;
	}

	m_arrDynLibs.Clear();
}

DynamicLibrary *DynamicLibraryManager::Load(const String &pFileName)
{
	for(U32 i = 0;i < m_arrDynLibs.Size();++i){
		if(m_arrDynLibs[i]->GetFileName() == pFileName){
			WriteDefaultLog(String("Library already loaded. Returning cached libray. Library:") + pFileName);
			return m_arrDynLibs[i];
		}
	}

	DynamicLibrary *pDynLib = K15_NEW DynamicLibrary(pFileName);
	if(!pDynLib->_Load()){
		WriteDefaultLog(String("Could not load library :") + pFileName);
		WriteDefaultLog(g_pSystem->GetSystemError());
	
		K15_DELETE pDynLib;
		return NULL;
	}

	m_arrDynLibs.PushBack(pDynLib);
	pDynLib->m_iHandle = m_arrDynLibs.Size() - 1;

	return pDynLib;
}

bool DynamicLibraryManager::Unload(DynamicLibrary *pDynLib)
{
	bool bUnload = pDynLib->_Unload();
	if(bUnload){
		m_arrDynLibs[pDynLib->GetHandle()] = NULL;
		K15_DELETE pDynLib;
	}

	return bUnload;
}

