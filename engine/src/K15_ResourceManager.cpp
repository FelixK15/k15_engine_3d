/**
 * @file K15_ResourceManager.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_ResourceManager.h"
#include "K15_ResourceFile.h"

#include "K15_LogManager.h"

using namespace K15_EngineV2;

ResourceManager::ResourceManager()
	: m_pResoureFile(NULL)
{

}

ResourceManager::~ResourceManager()
{
	if(m_pResoureFile){
		if(m_pResoureFile->IsOpen()){
			m_pResoureFile->CloseResourceFile();
		}

		K15_DELETE m_pResoureFile;
	}
}

void ResourceManager::Initialize()
{

}

void ResourceManager::Update(const GameTime &gtTime)
{
	double dDifference = 0;

	//Go through all Resources and check if they need to be deleted
	for(U32 i = 0;i < m_arrResource.Size();++i){
		if(m_arrResource[i] != NULL){
			if(m_arrResource[i]->GetResourceReferences() == 0){
				if(m_arrResource[i]->GetPriority() == Resource::RP_LOW){
					_DeleteResource(m_arrResource[i]);
					m_arrResource[i] = NULL;
				}else if(m_arrResource[i]->GetPriority() == Resource::RP_NORMAL){
					dDifference = g_pSystem->TimeSinceStart() - m_arrResource[i]->LastUsed();
					if(dDifference > 200.0){ // @todo read time from file
						_DeleteResource(m_arrResource[i]);
						m_arrResource[i] = NULL;
					}
				}else{
					dDifference = g_pSystem->TimeSinceStart() - m_arrResource[i]->LastUsed();
					if(dDifference > 500.0){ // @todo read time from file
						_DeleteResource(m_arrResource[i]);
						m_arrResource[i] = NULL;
					}
				}
			}

			//Check each Resource if its needs to be reloaded.

		}
	}
}

void ResourceManager::Shutdown()
{
	for(U32 i = 0;i < m_arrResource.Size();++i){
		if(m_arrResource[i] != NULL){
			K15_DELETE m_arrResource[i];
			m_arrResource[i] = NULL;
		}
	}

	m_arrResource.Clear();
	m_hmResources.Clear();
}

void ResourceManager::SetResourceFile( ResourceFile *pResourceFile )
{
	if(m_pResoureFile){
		if(m_pResoureFile->IsOpen()){
			m_pResoureFile->CloseResourceFile();
		}
		// @todo clear resource cache?
		K15_DELETE m_pResoureFile;
	}

	m_pResoureFile = pResourceFile;
}

void ResourceManager::_DeleteResource( Resource *pResource )
{
	m_hmResources.Erase(pResource->GetName().C_Str());
	K15_DELETE pResource;	
}
