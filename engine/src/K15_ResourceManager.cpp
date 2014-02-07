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

#include "K15_PrecompiledHeader.h"

#include "K15_ResourceManager.h"
#include "K15_ResourceFileBase.h"

#include "K15_LogManager.h"

namespace K15_Engine { namespace Core { 
  /*********************************************************************************/
	ResourceManager::ResourceManager()
		: m_Resources(),
		  m_ResourceDataCache(),
		  m_ResoureFiles()
	{

	}
  /*********************************************************************************/
	ResourceManager::~ResourceManager()
	{
		closeOpenResourceFiles();
		clearResourceCache();
		clearResources();
	}
  /*********************************************************************************/
	void ResourceManager::update(const GameTime &gtTime)
	{
		double difference = 0;
		bool alreadyOrphan = false;

		for(ResourceCache::iterator iter = m_ResourceDataCache.begin();
			iter != m_ResourceDataCache.end();++iter)
		{
			if(iter->second->getResourceReferences() == 0 && !iter->second->isMarkedAsUnreferenced())
			{
				_LogNormal("Asset \"%s\" is no longer referenced.",iter->second->getName().c_str());
				iter->second->setMarkedAsUnreferenced(true);
			}

			if(iter->second->isMarkedAsUnreferenced())
			{
				difference = g_Application->getTime() - iter->second->getLastUsed();

				if(iter->second->getPriority() == ResourceBase::RP_LOW ||
					(iter->second->getPriority() == ResourceBase::RP_NORMAL && difference > 200.0) ||
					(iter->second->getPriority() == ResourceBase::RP_HIGH	&& difference > 500.0))
				{
					_LogNormal("Deleting Asset \"%s\"",iter->second->getName().c_str());
					deleteResource(iter->second);

					m_ResourceDataCache.erase(iter);
				}
			}
		}
	}
  /*********************************************************************************/
	void ResourceManager::addResourceFile( ResourceFileBase* p_ResourceFile )
	{
		for(uint32 i = 0;i < m_ResoureFiles.size();++i)
		{
			if(m_ResoureFiles[i] == 0)
			{
				m_ResoureFiles[i] = p_ResourceFile;
				return;
			}
		}
	}
  /*********************************************************************************/
	void ResourceManager::deleteResource( ResourceBase *pResource )
	{
		for(ResourceList::iterator iter = m_Resources.begin();iter != m_Resources.end();++iter)
		{
			if((*iter) == pResource)
			{
				m_Resources.erase(iter);
			}
		}

		K15_DELETE pResource;	
	}
  /*********************************************************************************/
	void ResourceManager::closeOpenResourceFiles()
	{
		for(ResourceFileList::iterator iter = m_ResoureFiles.begin();iter != m_ResoureFiles.end();++iter)
		{
			if((*iter)->isOpen())
			{
				_LogNormal("Closing resource file \"%s\"",(*iter)->getResourceFileName().c_str());
				(*iter)->close();
			}
		}
	}
  /*********************************************************************************/
	void ResourceManager::clearResourceCache()
	{
		for(ResourceCache::iterator iter = m_ResourceDataCache.begin();iter != m_ResourceDataCache.end();++iter)
		{
			deleteResource(iter->second);
		}

		m_ResourceDataCache.clear();
	}
  /*********************************************************************************/
	void ResourceManager::clearResources()
	{
		ResourceList toDelete;
		for(ResourceList::iterator iter = m_Resources.begin();iter != m_Resources.end();++iter)
		{
			if((*iter)->getResourceReferences() == 0)
			{
				toDelete.push_back((*iter));
			}
		}

		for(ResourceList::iterator iter = toDelete.begin();iter != toDelete.end();++iter)
		{
			deleteResource((*iter));
		}

		m_Resources.clear();
	}
  /*********************************************************************************/
 }}//end of K15_Engine::Core namespace