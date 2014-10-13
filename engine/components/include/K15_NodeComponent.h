/**
 * @file K15_Node.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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

#ifndef _K15Engine_Core_Node_h_
#define _K15Engine_Core_Node_h_

#include "K15_Prerequisites.h"
#include "K15_GameObjectComponentBase.h"

#include "K15_Quaternion.h"
#include "K15_Matrix4.h"
#include "K15_Vector3.h"
#include "K15_Vector4.h"
#include "K15_MatrixUtil.h"

namespace K15_Engine { namespace Core { 
	class K15_CORE_API NodeComponent : public GameObjectComponentBase
	{
	public:
		/*********************************************************************************/
		K15_DECLARE_RTTI;
		typedef DynamicArray(GameObject*) ChildObjects;
		/*********************************************************************************/
	public:
		NodeComponent(GameObject* p_Parent = 0);
		~NodeComponent();

		void lookAt(const Vector3& p_Position);

		INLINE void setPosition(const Vector3& p_Position);
		INLINE void setOrientation(const Quaternion& p_Orientation);
		INLINE void setScale(const Vector3& p_Scale);
		void setTransformation(const Matrix4& p_Transformation);

		INLINE void translate(const Vector3& p_Translation);
		INLINE void translate(const Vector4& p_Translation);
		INLINE void translate(float x, float y, float z);

		INLINE void scale(const Vector3& p_Scale);
		INLINE void scale(float x, float y, float z);

		INLINE void rotate(const Quaternion& p_Rotation);
		void rotate(const Vector3& p_Axis, float p_Radians);

		//Axis must also be rotated!
		INLINE void roll(float p_Radians);
		INLINE void pitch(float p_Radians);
		INLINE void yaw(float p_Radians);

		INLINE const Vector3& getPosition() const;
		INLINE const Vector3& getScale() const;

		INLINE const Quaternion& getOrientation() const;
		INLINE Vector3 getLookAt() const;
		
		const Matrix4& getTransformation();

		INLINE bool needUpdate() const;
		void setNeedUpdate(bool p_Value);

		void addChild(GameObject* p_Child);
		void removeChild(GameObject* p_Child);
		void removeChild(const ObjectName& p_Name);

		void setParent(NodeComponent* p_Parent);
		INLINE GameObject* getParent() const;

		virtual AABB _calculateAABB();

	protected:
		GameObject* m_Parent;
		ChildObjects m_Children;
		Matrix4 m_Transformation;
		Quaternion m_Orientation, m_OriginOrientation;
		//Matrix4 m_Orientation, m_OriginOrientation;
/*		Vector4 m_Orientation, m_OriginOrientation;*/
		/*Quaternion m_Orientation, m_OriginOrientation;*/
		Vector3 m_Position, m_OriginPosition;
		Vector3 m_Scale, m_OriginScale;
		bool m_NeedUpdate;
	};// end of Node class declaration
#	include "K15_NodeComponent.inl"

  K15_PTR(NodeComponent);
  K15_WEAKPTR(NodeComponent);
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Node_h_