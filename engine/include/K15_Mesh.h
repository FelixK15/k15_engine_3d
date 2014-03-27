/**
 * @file K15_Mesh.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifndef _K15Engine_Rendering_Mesh_h_
#define _K15Engine_Rendering_Mesh_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_AABB.h"
#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API Mesh : public ResourceBase, public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef DynamicArray(SubMesh*) SubMeshArray;
		/*********************************************************************************/
	public:
		Mesh();
		Mesh(const ObjectName& p_Name);
		virtual ~Mesh();

		INLINE const AABB& getAABB(bool p_Calculate = false);
		INLINE const SubMeshArray& getSubMeshes() const;

		void calculateAABB();

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

		INLINE void addSubMesh(SubMesh* p_SubMesh);
  private:
		SubMeshArray m_SubMeshes;
		AABB m_AxisAlignedBoundingBox;
	};// end of Mesh class declaration
#	include "K15_Mesh.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_Mesh_h_