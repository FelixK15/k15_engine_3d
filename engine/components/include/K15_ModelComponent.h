/**
 * @file K15_ModelComponent.h
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

#ifndef _K15Engine_Core_ModelComponent_h_
#define _K15Engine_Core_ModelComponent_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_GameObjectComponentBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API ModelComponent : public GameObjectComponentBase
	{
	K15_DECLARE_RTTI;

	public:
		ModelComponent();
		ModelComponent(const String& p_ModelResourceName);
		ModelComponent(MeshInstance* p_ModelInstance);
		~ModelComponent();

		INLINE MeshInstance* getModelInstance() const;
		INLINE void setModelInstance(MeshInstance* p_Model);
		
		virtual void onAddedToRenderQueue(RenderQueue* p_RenderQueue);

	protected:
		virtual AABB _calculateAABB() OVERRIDE;

	private:
		MeshInstance* m_ModelInstance;
	}; //end of ModelComponent class declaration
	# include "K15_ModelComponent.inl"
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ModelComponent_h_