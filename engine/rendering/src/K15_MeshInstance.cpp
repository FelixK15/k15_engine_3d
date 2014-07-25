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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_MeshInstance.h"
#include "K15_SubMeshInstance.h"
#include "K15_SubMesh.h"
#include "K15_VertexBuffer.h"

namespace K15_Engine { namespace Rendering { 
  /*********************************************************************************/
  MeshInstance::MeshInstance(Mesh* p_Model)
    : m_AABB(),
    m_Mesh(p_Model),
    m_SubModelInstances()
  {
    
  }
  /*********************************************************************************/
  MeshInstance::~MeshInstance()
  {

  }
  /*********************************************************************************/
  void MeshInstance::_calculateAABB()
  {
    SubMeshInstance* submeshinstance = 0;
    SubMesh* submesh = 0;
    VertexBuffer* vertexBuffer = 0;

    DynamicArray(AABB) submeshBoundingBoxes;

    for(SubMeshInstanceArray::iterator iter = m_SubModelInstances.begin();
        iter != m_SubModelInstances.end(); ++iter)
    {
      submeshinstance = (*iter);
      
      submeshBoundingBoxes.push_back(submeshinstance->getAABB());
    }
  }
  /*********************************************************************************/
}} // end of K15_Engine::Rendering