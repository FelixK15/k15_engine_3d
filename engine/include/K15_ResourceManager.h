/**
 * @file K15_ResourceManager.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#ifndef _K15Engine_Core_Resourcemanager_h_
#define _K15Engine_Core_Resourcemanager_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceHandle.h"
#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Core { 

	class K15_CORE_API ResourceManager : public ApplicationAllocatedObject, public Singleton<ResourceManager>  //Singleton?
	{
	public:
		typedef DynamicArray(ResourceBase*)			ResourceList;
		typedef FixedArray(ResourceFileBase*,20)	ResourceFileList;
		typedef HashMap(ResourceName,ResourceBase*)	ResourceCache;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		void update(const GameTime &gtTime);

		template<class ResourceType> ResourceHandle<ResourceType> getResource(const ResourceName& p_ResourceName,Enum p_Priority)
		{
			K15_PROFILE(ResourceManager_GetResource);

			ResourceCache::iterator iter = m_ResourceDataCache.find(p_ResourceName);
			ResourceHandle<ResourceType> handle(K15_INVALID_RESOURCE_ID);

			if(iter == m_ResourceDataCache.end())
			{
				if(cacheResource<ResourceType>(p_FileName,p_Priority))
				{
					iter = m_ResourceDataCache.find(p_ResourceName);

					m_Resources.push_back(resource);
					handle.setResourceID(m_Resource.size() - 1);
				}
			}

			return handle;
		}

		template<class ResourceType> bool cacheResource(const ResourceName& p_ResourceName,Enum p_Priority)
		{
			K15_PROFILE(ResourceManager_CacheResource);

			bool cachedResource = false;
			RawData resourceData = {0};
			ResourceFileBase* resourceFile = 0;
			ResourceType resource = K15_NEW ResourceType(p_ResourceName);
			//we need to load the resource from one of the resource files.
			for(ResourceFileList::iterator iter = m_ResoureFiles.begin();iter != m_ResourceFiles.end();++iter)
			{
				resourceFile = (*iter);
				//try to open the resource file
				
				if(!resourceFile->open())
				{
					//log error and load debug resoure
					_LogError("Could not open asset file " + resourceFile->getResourceFileName());
				}
				else
				{
					if(resourceFile->hasResource(p_ResourceName))
					{
						if(resourceFile->getResource(p_ResourceName,resourceData))
						{
							break;
						}
					}
				}

				if(!resourceData.data)
				{
					resource->loadDebug(resourceData);
				}

				//check if actual data has been written
				if(resourceData.data)
				{
					//load Resource using resource data from the resource file
					if((cachedResource = resource->load(resourceData)) == true)
					{
						m_ResourceDataChache.insert(Pair(ResourceName,ResourceBase*)(p_ResourceName,resource));
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

		bool isResourceInCache(const ResourceName& p_ResourceName);
		void addResourceFile(ResourceFileBase *p_ResourceFile);

		ResourceBase* getResourceByID(ResourceID p_ResourceID);

		const ResourceList& getResources();
		const ResourceFileList& getResourceFileList();
	
	private:
		void deleteResource(ResourceBase *p_Resource);

		void closeOpenResourceFiles();
		void clearResourceCache();
		void clearResources();

		ResourceFileList m_ResoureFiles;
		ResourceCache m_ResourceDataCache;
		ResourceList m_Resources;
	};
	#include "K15_ResourceManager.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Resourcemanager_h_