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
#include "K15_Node.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,Node,Core::Object);
	/*********************************************************************************/
	Node::Node(const ObjectName& p_Name,Node* p_Parent)
		: Object(p_Name),
		  m_NeedUpdate(false),
		  m_Parent(p_Parent),
		  m_OriginOrientation(),
		  m_OriginPosition(),
		  m_Position(),
		  m_Scale(1.0f,1.0f,1.0f),
		  m_Orientation(),
		  m_OriginScale(1.0f,1.0f,1.0f),
		  m_Transformation()
	{

	}
	/*********************************************************************************/
	Node::~Node()
	{

	}
	/*********************************************************************************/
	void Node::setNeedUpdate(bool p_Value)
	{
		m_NeedUpdate = p_Value;

		for(ChildNodes::iterator iter = m_Children.begin();iter != m_Children.end();++iter)
		{
			(*iter)->setNeedUpdate(p_Value);
		}
	}
	/*********************************************************************************/
	const Matrix4& Node::getTransformation()
	{
// 		if(needUpdate())
// 		{
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

			// Ordering:
			//    1. Scale
			//    2. Rotate
			//    3. Translate

			// Set up final matrix with scale, rotation and translation
// 			m_Transformation = glm::scale(glm::mat4(1.0f),m_Scale);
// 			m_Transformation *= m_Orientation;
// 			m_Transformation *= glm::translate(glm::mat4(1.0f),m_Position);

			m_Transformation = glm::scale(glm::mat4(1.0f),m_Scale);
			m_Transformation *= glm::translate(glm::mat4(1.0f),m_Position);
			m_Transformation *= m_Orientation;

// 			m_Transformation._1_1 = m_Scale.x * rotation._1_1; 
// 			m_Transformation._1_2 = m_Scale.y * rotation._1_2; 
// 			m_Transformation._1_3 = m_Scale.z * rotation._1_3;
// 			m_Transformation._1_4 = m_Position.x;
// 
// 			m_Transformation._2_1 = m_Scale.x * rotation._2_1; 
// 			m_Transformation._2_2 = m_Scale.y * rotation._2_2; 
// 			m_Transformation._2_3 = m_Scale.z * rotation._2_3;
// 			m_Transformation._2_4 = m_Position.y;
// 
// 			m_Transformation._3_1 = m_Scale.x * rotation._3_1; 
// 			m_Transformation._3_2 = m_Scale.y * rotation._3_2; 
// 			m_Transformation._3_3 = m_Scale.z * rotation._3_3;
// 			m_Transformation._3_4 = m_Position.z;
// 
// 			// No projection term
// 			m_Transformation._4_1 = m_Transformation._4_2 = m_Transformation._4_3 = 0.0f;
// 			m_Transformation._4_4 = 1.0f;

			m_NeedUpdate = false;
		/*}*/

		return m_Transformation;
	}
	/*********************************************************************************/
	void Node::_calcLookAt()
	{
		static Vector4 viewNormal(0.0f,0.0f,-1.0f,0.0f);
		viewNormal = m_Orientation * viewNormal;
		viewNormal = glm::normalize(viewNormal);
		m_LookAt.x = viewNormal.x;
		m_LookAt.y = viewNormal.y;
		m_LookAt.z = viewNormal.z;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace