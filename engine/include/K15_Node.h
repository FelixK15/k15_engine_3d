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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADERS

// #include "K15_Quaternion.h"
// #include "K15_Matrix4.h"

namespace K15_Engine { namespace Core { 
	class K15_CORE_API Node : public Object, public GeneralAllocatedObject
	{
	public:
		/*********************************************************************************/
		K15_DECLARE_RTTI;
		typedef DynamicArray(Node*) ChildNodes;
		/*********************************************************************************/
	public:
		Node(const ObjectName& p_Name = ObjectName::BLANK,Node* p_Parent = 0);
		~Node();

		INLINE void lookAt(const Vector3& p_Position);

		INLINE void setPosition(const Vector3& p_Position);
		INLINE void setOrientation(const Quaternion& p_Orientation);
		INLINE void setScale(const Vector3& p_Scale);

		INLINE void translate(const Vector3& p_Translation);
		INLINE void translate(float x, float y, float z);

		INLINE void scale(const Vector3& p_Scale);
		INLINE void scale(float x, float y, float z);

/*		INLINE void rotate(const Quaternion& p_Rotation);*/
		INLINE void rotate(const Vector3& p_Axis, float p_Radians);

		INLINE void roll(float p_Angle);
		INLINE void pitch(float p_Angle);
		INLINE void yaw(float p_Angle);

		INLINE const Vector3& getPosition() const;
		INLINE const Vector3& getScale() const;

		/*INLINE const Quaternion& getOrientation() const;*/
		
		const Matrix4& getTransformation();

		INLINE bool needUpdate() const;
		void setNeedUpdate(bool p_Value);

		Node* createChild(const ObjectName& p_Name = ObjectName::BLANK);

		void addChild(Node* p_Child);
		void removeChild(Node* p_Child);
		void removeChild(const ObjectName& p_Name);

		void setParent(Node* p_Parent);
		INLINE Node* getParent() const;

	protected:
		Node* m_Parent;
		ChildNodes m_Children;
		Matrix4 m_Transformation;
		Matrix4 m_Orientation, m_OriginOrientation;
/*		Vector4 m_Orientation, m_OriginOrientation;*/
		/*Quaternion m_Orientation, m_OriginOrientation;*/
		Vector3 m_Position, m_OriginPosition;
		Vector3 m_Scale, m_OriginScale;
		bool m_NeedUpdate;
	};// end of Node class declaration
#	include "K15_Node.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Node_h_