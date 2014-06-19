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
void Node::lookAt(const Vector3& p_Position)
{
	m_Orientation = glm::lookAt(m_Position,p_Position,Vector3(0.0f,1.0f,0.0f));
	m_NeedUpdate = true;
	_calcLookAt();
}
/*********************************************************************************/
void Node::setPosition(const Vector3& p_Position)
{
	m_Position = p_Position;
	m_NeedUpdate = true;
}
/*********************************************************************************/
// void Node::setOrientation(const Quaternion& p_Orientation)
// {
// 	m_Orientation = p_Orientation;
// }
/*********************************************************************************/
void Node::setScale(const Vector3& p_Scale)
{
	m_Scale = p_Scale;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::translate(const Vector3& p_Translation)
{
	m_Position += p_Translation;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::translate(const Vector4& p_Translation)
{
	translate(p_Translation.x,p_Translation.y,p_Translation.z);
}
/*********************************************************************************/
void Node::translate(float x, float y, float z)
{
	m_Position += Vector3(x,y,z);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::scale(const Vector3& p_Scale)
{
	m_Scale += p_Scale;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::scale(float x, float y, float z)
{
	m_Scale += Vector3(x,y,z);
	m_NeedUpdate = true;
}
/*********************************************************************************/
// void Node::rotate(const Quaternion& p_Rotation)
// {
// 	m_Orientation *= p_Rotation;
// }
/*********************************************************************************/
void Node::rotate(const Vector3& p_Axis, float p_Radians)
{
	m_Orientation *= glm::rotate(glm::mat4(1.0f),p_Radians,p_Axis);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::roll(float p_Radians)
{
	rotate(Vector3(0.0f,0.0f,1.0f),p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::pitch(float p_Radians)
{
	rotate(Vector3(1.0f,0.0f,0.0f),p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void Node::yaw(float p_Radians)
{
	rotate(Vector3(0.0f,1.0f,0.0f),p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
const Vector3& Node::getPosition() const
{
	return m_Position;
}
/*********************************************************************************/
const Vector3& Node::getScale() const
{
	return m_Scale;
}
/*********************************************************************************/
const Matrix4& Node::getOrientation() const
{
	return m_Orientation;
}
/*********************************************************************************/
bool Node::needUpdate() const
{
	return m_NeedUpdate;
}
/*********************************************************************************/
const Vector3& Node::getLookAt() const
{
	return m_LookAt;
}
/*********************************************************************************/
Node* Node::getParent() const
{
	return m_Parent;
}
/*********************************************************************************/