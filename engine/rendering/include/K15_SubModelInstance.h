/**
 * @file K15_SubModelInstance.h
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

#ifndef _K15Engine_Renderer_SubModelInstance_h_
#define _K15Engine_Renderer_SubModelInstance_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API SubMeshInstance : public RenderingAllocatedObject
	{
	public:
		SubMeshInstance(SubMesh* p_Model);
		~SubMeshInstance();

		INLINE SubMesh* getSubMesh() const;
		INLINE Material* getMaterial() const;
		INLINE RenderOperation* getRenderOperation() const;

	private:
		SubMesh* m_SubMesh;
		Material* m_Material;
		RenderOperation* m_RenderOperation;
		//Animations
	}; // end of ModelInstance class declaration
}} //end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_SubModelInstance_h_