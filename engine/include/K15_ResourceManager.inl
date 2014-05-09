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

/*********************************************************************************/
template<class ResourceType>
ResourceType* ResourceManager::getResource(const String& p_ResourceName,Enum p_Priority)
{
	//K15_PROFILE(StringUtil::format("ResourceManager::getResource (%s)",p_ResourceName.c_str()));

	ResourceCache::iterator iter = m_ResourceDataCache.find(p_ResourceName);
	ResourceType* resource = 0;
	if(iter == m_ResourceDataCache.end())
	{
		if(cacheResource(p_ResourceName, ResourceType::TYPE, p_Priority))
		{
			iter = m_ResourceDataCache.find(p_ResourceName);

			m_Resources.push_back(iter->second);
		}
		else
		{
			_LogError("Could not load resource \"%s\".",p_ResourceName.c_str());
		}
	}
	resource = (ResourceType*)iter->second;
	return resource;
}
/*********************************************************************************/
const ResourceManager::ResourceArchiveList& ResourceManager::getResourceFileList() const
{
	return m_ResoureFiles;
}
/*********************************************************************************/
// ResourceBase* ResourceManager::getResourceByID(ResourceID p_ResourceID)
// {
// 	if(p_ResourceID == K15_INVALID_RESOURCE_ID || (unsigned int)p_ResourceID > m_Resources.size())
// 	{
// 		return 0;
// 	}
// 
// 	return m_Resources[p_ResourceID];
// }
/*********************************************************************************/
void ResourceManager::addResourceArchive(ResourceArchiveBase *p_ResourceFile)
{
	m_ResoureFiles.push_back(p_ResourceFile);
}
/*********************************************************************************/
void ResourceManager::addResourceImporter(ResourceImporterBase* p_ResourceImporter)
{
	m_ResourceImporters.push_back(p_ResourceImporter);
}
/*********************************************************************************/