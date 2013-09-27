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
#ifndef _K15Engine_System_Resourcemanager_h_
#define _K15Engine_System_Resourcemanager_h_

#include "K15_Prerequisites.h"
#include "K15_Application.h"
#include "K15_AllocatedObject.h"

#include "K15_ResourceHandle.h"
#include "K15_ResourceBase.h"

namespace K15_Engine { namespace System { 

	class K15_API_EXPORT ResourceManager : public ApplicationAllocatedObject,
							public Singleton<ResourceManager>  //Singleton?
	{
	public:
		typedef K15_DynamicArray(ResourceBase*)			ResourceList;
		typedef K15_FixedArray(ResourceFileBase*,20)	ResourceFileList;
		typedef K15_HashMap(ResourceName,ResourceBase*)	ResourceCache;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		void update(const GameTime &gtTime);

		template<class ResourceType> ResourceHandle<ResourceType> getResource(const String& ResourceName,Enum p_Priority);
		bool cacheResource(const ResourceName& p_ResourceName,Enum p_Priority);

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
}} //end of K15_Engine::System namespace

#endif //_K15Engine_System_Resourcemanager_h_