/**
 * @file K15_ResourceManager.inl
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

template<class ResourceType>
bool ResourceManager::cacheResource(const ResourceName& p_ResourceName,Enum p_Priority)
{
	bool cachedResource = false;
	ResourceData resourceData = {0};
	ResourceFileBase* resourceFile = 0;
	//we need to load the resource from one of the resource files.
	for(ResourceFileList::iterator iter = m_ResoureFiles.begin();iter != m_ResourceFiles.end();++iter)
	{
		resourceFile = (*iter);
		//try to open the resource file
		if(!resourceFile->isOpen())
		{
			//log error and load debug resoure
			K15_LogErrorMessage("Could not open resource file " + resourceFile->getResourceFileName());
			ResourceType::loadDebugResource(resourceData);
		}
		else
		{
			if(!resourceFile->hasResource(p_ResourceName) || !resourceFile->getResource(p_ResourceName,resourceData))
			{
				//oh oh...that didn't go so well. Log Error and try to load the debug resource from that resource type.
				String logMessage = "Could not load asset:";
				logMessage += p_ResourceName.getString();

				K15_LogErrorMessage(logMessage);

				ResourceType::loadDebugResource(resourceData);
			}
		}

		//check if actual data has been written
		if(resourceData.Data)
		{
			//load Resource using resource data from the resource file
			ResourceType* resource = K15_NEW ResourceType(p_ResourceName);
			if((cachedResource = resource->load(resourceData)) == true)
			{
				m_ResourceDataChache.insert(K15_Pair(ResourceName,ResourceBase*)(p_ResourceName,resource));
			}
		}

		//close the file after we're finished
		resourceFile->close();

		if(cachedResource)
		{
			break;
		}
	}

	return cachedResource;
}

template<class ResourceType>
ResourceHandle<ResourceType> ResourceManager::getResource(const ResourceName& p_ResourceName,Enum p_Priority)
{
	ResourceCache::iterator iter = m_ResourceDataCache.find(p_ResourceName);
	ResourceHandle<ResourceType> handle(K15_INVALID_RESOURCE_ID);

	if(iter == m_ResourceDataCache.end())
	{
		if(!cacheResource<ResourceType>(p_FileName,p_Priority))
		{
			//resource could not get loaded.
			K15_LogErrorMessage(String("Could not load resource. name:") + p_ResourceName.getString());
			K15_LogErrorMessage(String("Last Error:") + Application::getInstance()->getLastError());
		}
		else
		{
			iter = m_ResourceDataCache.find(p_ResourceName);

			m_Resources.push_back(resource);
			handle.setResourceID(m_Resource.size() - 1);
		}
	}

	return handle;
}

const ResourceManager::ResourceFileList& ResourceManager::getResourceFileList()
{
	return m_ResoureFiles;
}

const ResourceManager::ResourceList& ResourceManager::getLoadedResources()
{
	return m_Resources;
}

ResourceBase* ResourceManager::getResourceByID(ResourceID p_ResourceID)
{
	if(p_ResourceID > m_Resources.size())
	{
		return 0;
	}

	return m_Resources[p_ResourceID];
}
