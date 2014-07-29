/**
 * @file K15_GameObject.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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

#ifndef _K15Engine_Core_GameObject_h_
#define _K15Engine_Core_GameObject_h_

#include "K15_Prerequisites.h"
#include "K15_Object.h"
#include "K15_NodeComponent.h"
#include "K15_GameObjectComponentBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API GameObject : public Object, public GeneralAllocatedObject
	{
	public:
		/*********************************************************************************/
        K15_DECLARE_RTTI
		typedef DynamicArray(GameObjectComponentBase*) ComponentArray;
		/*********************************************************************************/
	public:
		GameObject();
		GameObject(const ObjectName& p_Name);
		~GameObject();

		const AABB& getAABB();

		void addComponent(GameObjectComponentBase* p_Component);
		void onAddedToRenderQueue(RenderQueue* p_RenderQueue);
	
		INLINE NodeComponent* getNodeComponent();
		INLINE CameraComponent* getCameraComponent();

		GameObjectComponentBase* getComponentByName(const ObjectName& p_TypeName) const;
	
		template<class ComponentType>
		ComponentType* getComponentByType() const;

		INLINE const ComponentArray& getComponents() const;

		void update(const GameTime& p_GameTime);

		void lookAt(const Vector3& p_Position);

		void setPosition(const Vector3& p_Position);
		//void setOrientation(const Quaternion& p_Orientation);
		void setScale(const Vector3& p_Scale);

		void translate(const Vector3& p_Translation);
		void translate(float x, float y, float z);

		void scale(const Vector3& p_Scale);
		void scale(float x, float y, float z);

		void rotate(const Vector3& p_Axis, float p_Radians);

		void roll(float p_Radians);
		void pitch(float p_Radians);
		void yaw(float p_Radians);

		const Vector3& getPosition() const;
		const Vector3& getScale() const;

		const Matrix4& getOrientation() const;
		const Vector3& getLookAt() const;

		const Matrix4& getTransformation();

	private:
		ComponentArray m_Components;
		NodeComponent* m_NodeComponent;
		CameraComponent* m_CameraComponent;

	};// end of GameObject class declaration
#include "K15_GameObject.inl"
	K15_PTR(GameObject);
	K15_WEAKPTR(GameObject);
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_GameObject_h_
