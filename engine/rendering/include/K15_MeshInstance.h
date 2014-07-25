/**
 * @file K15_MeshInstance.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/14
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

#ifndef _K15Engine_Renderer_MeshInstance_h_
#define _K15Engine_Renderer_MeshInstance_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_AABB.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API MeshInstance : public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef DynamicArray(SubMeshInstance*) SubMeshInstanceArray;
		/*********************************************************************************/

	public:
		MeshInstance(Mesh* p_Model);
		~MeshInstance();

		INLINE Mesh* getMesh() const;
    INLINE SubMeshInstance* getSubMeshInstance(int32 p_Index) const;
    INLINE SubMeshInstanceArray& getSubMeshInstances() const;
    INLINE int32 getSubMeshInstanceCount() const;

    INLINE const AABB& getAABB() const;

  private:
    void _calculateAABB();

	private:
    AABB m_AABB;
		Mesh* m_Mesh;
		SubMeshInstanceArray m_SubModelInstances;
		//Animations
	}; // end of ModelInstance class declaration
}} //end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_MeshInstance_h_