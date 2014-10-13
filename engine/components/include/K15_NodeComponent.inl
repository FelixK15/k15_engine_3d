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
void NodeComponent::setPosition(const Vector3& p_Position)
{
	m_Position = p_Position;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::setOrientation(const Quaternion& p_Orientation)
{
	m_Orientation = p_Orientation;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::setScale(const Vector3& p_Scale)
{
	m_Scale = p_Scale;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::translate(const Vector3& p_Translation)
{
	m_Position += p_Translation;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::translate(const Vector4& p_Translation)
{
	translate(p_Translation.x,p_Translation.y,p_Translation.z);
}
/*********************************************************************************/
void NodeComponent::translate(float x, float y, float z)
{
	m_Position += Vector3(x,y,z);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::scale(const Vector3& p_Scale)
{
	m_Scale += p_Scale;
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::scale(float x, float y, float z)
{
	m_Scale += Vector3(x,y,z);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::rotate(const Quaternion& p_Rotation)
{
	m_Orientation *= p_Rotation;
}
/*********************************************************************************/
void NodeComponent::roll(float p_Radians)
{
	static Vector3 const zAxis = Vector3(0.0f, 0.0f, 1.0f);
	rotate(zAxis, p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::pitch(float p_Radians)
{
	static Vector3 const xAxis = Vector3(1.0f, 0.0f, 0.0f);
	rotate(xAxis, p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
void NodeComponent::yaw(float p_Radians)
{
	static Vector3 const yAxis = Vector3(0.0f, 1.0f, 0.0f);
	rotate(yAxis, p_Radians);
	m_NeedUpdate = true;
}
/*********************************************************************************/
const Vector3& NodeComponent::getPosition() const
{
	return m_Position;
}
/*********************************************************************************/
const Vector3& NodeComponent::getScale() const
{
	return m_Scale;
}
/*********************************************************************************/
const Quaternion& NodeComponent::getOrientation() const
{
	return m_Orientation;
}
/*********************************************************************************/
bool NodeComponent::needUpdate() const
{
	return m_NeedUpdate;
}
/*********************************************************************************/
Vector3 NodeComponent::getLookAt() const
{
	return Vector3(m_Transformation._3_1, m_Transformation._3_2, m_Transformation._3_3);
}
/*********************************************************************************/
GameObject* NodeComponent::getParent() const
{
	return m_Parent;
}
/*********************************************************************************/