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
 */

/*********************************************************************************/
inline void Node::setPosition(const Math::Vector3& p_Position)
{
	m_Position = p_Position;
}
/*********************************************************************************/
inline void Node::setOrientation(const Math::Quaternion& p_Orientation)
{
	m_Orientation = p_Orientation;
}
/*********************************************************************************/
inline void Node::setScale(const Math::Vector3& p_Scale)
{
	m_Scale = p_Scale;
}
/*********************************************************************************/
inline void Node::translate(const Math::Vector3& p_Translation)
{
	m_Position += p_Translation;
}
/*********************************************************************************/
inline void Node::translate(float x, float y, float z)
{
	m_Position += Math::Vector3(x,y,z);
}
/*********************************************************************************/
inline void Node::scale(const Math::Vector3& p_Scale)
{
	m_Scale += p_Scale;
}
/*********************************************************************************/
inline void Node::scale(float x, float y, float z)
{
	m_Scale += Math::Vector3(x,y,z);
}
/*********************************************************************************/
inline void Node::rotate(const Math::Quaternion& p_Rotation)
{
	m_Orientation *= p_Rotation;
}
/*********************************************************************************/
inline void Node::rotate(const Math::Vector3& p_Axis, float p_Angle)
{
	m_Orientation *= Math::Quaternion(p_Angle,p_Axis);
}
/*********************************************************************************/
inline void Node::roll(float p_Angle)
{
	rotate(Math::Vector3::Forward,p_Angle);
}
/*********************************************************************************/
inline void Node::pitch(float p_Angle)
{
	rotate(Math::Vector3::Right,p_Angle);
}
/*********************************************************************************/
inline void Node::yaw(float p_Angle)
{
	rotate(Math::Vector3::Up,p_Angle);
}
/*********************************************************************************/
inline const Math::Vector3& Node::getPosition() const
{
	return m_Position;
}
/*********************************************************************************/
inline const Math::Vector3& Node::getScale() const
{
	return m_Scale;
}
/*********************************************************************************/
inline const Math::Quaternion& Node::getOrientation() const
{
	return m_Orientation;
}
/*********************************************************************************/
inline bool Node::needUpdate() const
{
	return m_NeedUpdate;
}
/*********************************************************************************/
inline Node* Node::getParent() const
{
	return m_Parent;
}
/*********************************************************************************/