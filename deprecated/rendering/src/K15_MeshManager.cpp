/**
 * @file K15_MeshManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
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

#include "K15_MeshManager.h"
#include "K15_Mesh.h"
#include "K15_MeshInstance.h"
#include "K15_ResourceManager.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	MeshManager::MeshManager()
		: m_MeshCache()
	{

	}
	/*********************************************************************************/
	MeshManager::~MeshManager()
	{
		clear();
	}
	/*********************************************************************************/
	MeshInstance* MeshManager::getMesh(const ResourceName& p_ResourceName)
	{
		for(uint32 i = 0; i < m_MeshCache.size(); ++i)
		{
			MeshCacheEntry& cacheEntry = m_MeshCache.at(i);

			if(cacheEntry.Mesh->getName() == p_ResourceName)
			{
				MeshInstance* meshInstance = K15_NEW MeshInstance(cacheEntry.Mesh);
				cacheEntry.ActiveInstances.push_back(meshInstance);
				return meshInstance;
			}
		}

		//load base mesh
		if(Mesh* mesh = g_ResourceManager->getResource<Mesh>(p_ResourceName.c_str()))
		{
			MeshCacheEntry cacheEntry;
			cacheEntry.Mesh = mesh;

			m_MeshCache.push_back(cacheEntry);

			return getMesh(p_ResourceName);
		}

		/// todo: Dummy Mesh?

		return 0;
	}
	/*********************************************************************************/
	void MeshManager::clear()
	{
		m_MeshCache.clear();
	}
	/*********************************************************************************/
}} // end of K15_Engine::Rendering namespace