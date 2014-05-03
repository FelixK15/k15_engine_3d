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
#include "K15_ResourceArchiveBase.h"

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
	void ResourceManager::deleteResource(ResourceBase *pResource)
	{
		for(ResourceList::iterator iter = m_Resources.begin();iter != m_Resources.end();++iter)
		{
			if((*iter) == pResource)
			{
				m_Resources.erase(iter);
				break;
			}
		}

		K15_DELETE pResource;	
	}
	/*********************************************************************************/
	void ResourceManager::closeOpenResourceFiles()
	{
		for(ResourceArchiveList::iterator iter = m_ResoureFiles.begin();iter != m_ResoureFiles.end();++iter)
		{
			if((*iter)->isOpen())
			{
				_LogNormal("Closing resource file \"%s\"",(*iter)->getFileName().c_str());
				(*iter)->close();
			}
		}
	}
	/*********************************************************************************/
	void ResourceManager::clearResourceCache()
	{
		for(ResourceCache::iterator iter = m_ResourceDataCache.begin();iter != m_ResourceDataCache.end();++iter)
		{
			K15_DELETE iter->second;
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

		clearResourceCache();

		m_Resources.clear();
	}
	/*********************************************************************************/
	bool ResourceManager::isResourceInCache(const String& p_ResourceName)
	{
		for(ResourceList::iterator iter = m_Resources.begin();iter != m_Resources.end();++iter)
		{
			if((*iter)->getName() == p_ResourceName)
			{
				return true;
			}
		}

		return false;
	}
	/*********************************************************************************/
	bool ResourceManager::cacheResource(const String& p_ResourceName, const TypeName& p_ResourceTypeName, Enum p_Priority)
	{
		K15_PROFILE(StringUtil::format("ResourceManager::cacheResource (%s)",p_ResourceName.c_str()));

		_LogDebug("Trying to cache resource \"%s\".",p_ResourceName.c_str());

		bool cachedResource = false;
		RawData resourceData(0,0);
		ResourceArchiveBase* resourceArchive = 0;
		ResourceBase* resource = 0;
		ResourceImporterBase* importer = 0;
		//we need to load the resource from one of the resource files.
		for(ResourceArchiveList::iterator iter = m_ResoureFiles.begin();iter != m_ResoureFiles.end();++iter)
		{
			resourceArchive = (*iter);
			//try to open the resource file

			if(!resourceArchive->open())
			{
				//log error and load debug resoure
				_LogError("Could not open resource archive %s. Error:\"%s\".",
					resourceArchive->getFileName().c_str(),resourceArchive->getError().c_str());
			}
			else
			{
				if(resourceArchive->hasResource(p_ResourceName.c_str()))
				{
					if(!resourceArchive->getResource(p_ResourceName.c_str(),&resourceData))
					{
						_LogError("Could not load resource \"%s\" from resource archive \"%s\", "
							"eventhough resource confirms to hold the resource. Error:\"%s\".",
							p_ResourceName.c_str(),resourceArchive->getFileName().c_str(),resourceArchive->getError().c_str());

						//try next resource archive
						continue;
					}
				}
			}

			//check if actual data has been written
			if(resourceData.data)
			{
				//check what importer we can use for the resource
				for(ResourceImporterList::iterator r_iter = m_ResourceImporters.begin();r_iter != m_ResourceImporters.end();++r_iter)
				{
					importer = (*r_iter);

					//check if file can be loaded by checking the file extension
					if(!importer->canLoad(p_ResourceName))
					{
						//check deeper...Check file signature.
						if(!importer->canLoadBaseOnSignature(resourceData))
						{
							//nope, file can not be loaded. try next importer
							continue;
						}
					}

					_LogDebug("Loading resource \"%s\" using resource loader \"%s\".",
						p_ResourceName.c_str(),importer->getTypeName().c_str());

					//load resource using importer
					if((resource = importer->load(resourceData,p_ResourceTypeName)) == 0)
					{
						_LogError("Could not load Resource \"%s\" using Resource Importer \"%s\". Error\"%s\".",
							p_ResourceName.c_str(),importer->getName().c_str(),importer->getError().c_str());
					}
					else
					{
						_LogSuccess("Successfully loaded resource \"%s\" using resource loader \"%s\".",
							p_ResourceName.c_str(),importer->getTypeName().c_str());

						m_ResourceDataCache.insert(Pair(ResourceName,ResourceBase*)(p_ResourceName,resource));
						cachedResource = true;
						break;
					}
				}
			}

			//close the file after we're finished
			resourceArchive->close();

			//if the resource has been cached, exit loop.
			if(cachedResource)
			{
				break;
			}
		}

		return cachedResource;
	}
	/*********************************************************************************/
 }}//end of K15_Engine::Core namespace