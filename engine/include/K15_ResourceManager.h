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

#pragma once

#ifndef __K15_RESOURCEMANAGER__
#define __K15_RESOURCEMANAGER__

#include "K15_StdInclude.h"
#include "K15_Singleton.h"

#include "K15_Resource.h"
#include "K15_ResourceHandle.h"
#include "K15_HashMap.h"

#define g_pResourceManager K15_EngineV2::ResourceManager::GetInstance()

namespace K15_EngineV2
{
	class ResourceFile;

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		void Initialize();
		void Update(const GameTime &gtTime);
		void Shutdown();

		template<class T> ResourceHandlePtr GetResource(String &sFileName,Resource::ResourcePriority rpPriority);
		template<class T> bool CacheResource(String &sFileName,Resource::ResourcePriority rpPriority);

		bool IsResourceInCache(const String &sName);
		void SetResourceFile(ResourceFile *pResourceFile);

	private:
		void _DeleteResource(Resource *pResource);

		ResourceFile *m_pResoureFile;

		HashMap<const char*,Resource*,350> m_hmResources;
		DynamicArray<Resource*> m_arrResource;
	};
	#include "..\src\K15_ResourceManager.inl"
}

#endif //__K15_RESOURCEMANAGER__