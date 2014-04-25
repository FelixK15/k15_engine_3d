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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#	include "K15_AllocatedObject.h"
#	include "K15_LogManager.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_ResourceArchiveBase.h"
/*#include "K15_ResourceHandle.h"*/
#include "K15_ResourceBase.h"
#include "K15_ResourceImporterBase.h"
#include "K15_ProfilingManager.h"

namespace K15_Engine { namespace Core { 

	class K15_CORE_API ResourceManager : public CoreAllocatedObject,
										 public Singleton<ResourceManager>  //Singleton?
	{
	public:
		typedef DynamicArray(ResourceImporterBase*) ResourceImporterList;
		typedef DynamicArray(ResourceBase*)			ResourceList;
		typedef DynamicArray(ResourceArchiveBase*)	ResourceArchiveList;
		typedef HashMap(ResourceName,ResourceBase*)	ResourceCache;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		void update(const GameTime &gtTime);

		template<class ResourceType> ResourceType* getResource(const String& p_ResourceName,Enum p_Priority = ResourceBase::RP_NORMAL);

		bool cacheResource(const String& p_ResourceName,Enum p_Priority);
		bool isResourceInCache(const String& p_ResourceName);
		
		INLINE void addResourceFile(ResourceArchiveBase *p_ResourceFile);
		INLINE void addResourceImporter(ResourceImporterBase* p_ResourceImporter);

		//INLINE ResourceBase* getResourceByID(ResourceID p_ResourceID);

		INLINE const ResourceList& getResources() const;
		INLINE const ResourceArchiveList& getResourceFileList() const;
	
	private:
		void deleteResource(ResourceBase *p_Resource);

		void closeOpenResourceFiles();
		void clearResourceCache();
		void clearResources();

		ResourceImporterList m_ResourceImporters;
		ResourceArchiveList m_ResoureFiles;
		ResourceCache m_ResourceDataCache;
		ResourceList m_Resources;
	}; //end of ResourceManager class declaration
	#include "K15_ResourceManager.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Resourcemanager_h_