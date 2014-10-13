/**
 * @file K15_Node.cpp
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_NodeComponent.h"
#include "K15_GameObject.h"

#include "K15_ModelComponent.h"
#include "K15_MatrixUtil.h"
#include "K15_MathUtil.h"
#include "K15_QuaternionUtil.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,NodeComponent,Core::Object);
	/*********************************************************************************/
	NodeComponent::NodeComponent(GameObject* p_Parent)
		: GameObjectComponentBase(_TN(NodeComponent)),
		m_NeedUpdate(false),
		m_Parent(p_Parent),
		m_OriginOrientation(),
		m_OriginPosition(),
		m_Position(),
		m_Children(),
		m_Scale(1.0f,1.0f,1.0f),
		m_Orientation(),
		m_OriginScale(1.0f,1.0f,1.0f),
		m_Transformation()
	{

	}
	/*********************************************************************************/
	NodeComponent::~NodeComponent()
	{

	}
	/*********************************************************************************/
	void NodeComponent::setNeedUpdate(bool p_Value)
	{
		m_NeedUpdate = p_Value;

		for(ChildObjects::iterator iter = m_Children.begin();iter != m_Children.end();++iter)
		{
			(*iter)->getNodeComponent()->setNeedUpdate(p_Value);
		}
	}
	/*********************************************************************************/
	const Matrix4& NodeComponent::getTransformation()
	{
		if(needUpdate())
		{
			//update from parent
			if(m_Parent)
			{
// 				m_Scale = m_OriginScale * m_Parent->getScale();
// 				m_Orientation = m_OriginOrientation * m_Parent->getOrientation();
// 				m_Position = m_OriginPosition + m_Parent->getPosition();
			}
			else
			{
// 				m_Scale = m_OriginScale;
// 				m_Orientation = m_OriginOrientation;
// 				m_Position = m_OriginPosition;
			}

			m_Transformation = MatrixUtil::scale(m_Scale);
			m_Transformation *= m_Orientation.toRotationMatrix();
			m_Transformation *= MatrixUtil::translate(m_Position);

			m_NeedUpdate = false;
		}

		return m_Transformation;
	}
	/*********************************************************************************/
	void NodeComponent::rotate(const Vector3& p_Axis, float p_Radians)
	{
		Quaternion rotation = QuaternionUtil::fromAxisAngles(p_Axis, p_Radians);

		rotation.normalize();
		m_Orientation *= rotation;

		m_NeedUpdate = true;
	}
	/*********************************************************************************/
	void NodeComponent::setTransformation(const Matrix4& p_Transformation)
	{
		m_Position.x = m_Transformation._4_1;
		m_Position.y = m_Transformation._4_2;
		m_Position.z = m_Transformation._4_3;

		m_Scale.x = MathUtil::sqrt(m_Transformation._1_1 * m_Transformation._1_1 + 
			m_Transformation._2_1 * m_Transformation._2_1 +
			m_Transformation._3_1 * m_Transformation._3_1);

		m_Scale.y = MathUtil::sqrt(m_Transformation._1_2 * m_Transformation._1_2 + 
			m_Transformation._2_2 * m_Transformation._2_2 +
			m_Transformation._3_2 * m_Transformation._3_2);

		m_Scale.y = MathUtil::sqrt(m_Transformation._1_3 * m_Transformation._1_3 + 
			m_Transformation._2_3 * m_Transformation._2_3 +
			m_Transformation._3_3 * m_Transformation._3_3);

		m_Orientation = QuaternionUtil::fromTransformation(p_Transformation);

		m_NeedUpdate = true;
	}
	/*********************************************************************************/
	void NodeComponent::lookAt(const Vector3& p_Position)
	{
		setTransformation(MatrixUtil::lookAt(m_Position, m_Position, Vector3::Up));
	}
	/*********************************************************************************/
	AABB NodeComponent::_calculateAABB()
	{
		AABB boundingBox;
		if(ModelComponent* model = getGameObject()->getModelComponent())
		{
			boundingBox = model->getAABB();
		}

		Vector3 min = boundingBox.getMin();
		Vector3 max = boundingBox.getMax();

		min += m_Position;
		max += m_Position;

		boundingBox.setMax(max);
		boundingBox.setMin(min);

		return boundingBox;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace