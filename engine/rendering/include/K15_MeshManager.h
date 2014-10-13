/**
 * @file K15_MeshManager.h
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
 *
 * @section DESCRIPTION
 *
 */

#ifndef _K15Engine_Rendering_MeshManager_h_
#define _K15Engine_Rendering_MeshManager_h_

#include "K15_Prerequisites.h"
#include "K15_Application.h"
#include "K15_AllocatedObject.h"
#include "K15_Singleton.h"

namespace K15_Engine { namespace Rendering {
	struct K15_CORE_API MeshCacheEntry
	{
		typedef DynamicArray(MeshInstance*) MeshInstances;

		Mesh* Mesh;
		MeshInstances ActiveInstances;
	};
	/*********************************************************************************/
	class K15_CORE_API MeshManager : public CoreAllocatedObject, 
									 public Singleton<MeshManager>
	{
	public:
		typedef DynamicArray(MeshCacheEntry) MeshCache;
	public:
		MeshManager();
		~MeshManager();

		MeshInstance* getMesh(const ResourceName& p_ResourceName);
		INLINE void clear();

	private:
		MeshCache m_MeshCache;

	}; // end of MeshManager class declaration
	#include "K15_MeshManager.inl"
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_MeshManager_h_