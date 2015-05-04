/**
 * @file K15_CameraComponent.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
bool CameraComponent::getActive() const
{
  return m_Active;
}
/*********************************************************************************/
float CameraComponent::getZoom() const
{
	return m_Zoom;
}
/*********************************************************************************/
void CameraComponent::setZoom(float p_Zoom)
{
	if(p_Zoom > 0.0f)
	{
		m_Zoom = p_Zoom;
		m_ProjMatrixDirty = true;
	}
}
/*********************************************************************************/
float CameraComponent::getFov() const
{
  return m_Fov;
}
/*********************************************************************************/
float CameraComponent::getFarClipDistance() const
{
  return m_FarClipDistance;
}
/*********************************************************************************/
float CameraComponent::getNearClipDistance() const
{
  return m_NearClipDistance;
}
/*********************************************************************************/
void CameraComponent::setFieldOfView(float p_Fov)
{
  m_Fov = p_Fov;
  m_ViewMatrixDirty = true;
}
/*********************************************************************************/
void CameraComponent::setFarClipDistance(float p_Far)
{
  m_FarClipDistance = p_Far;
  m_ProjMatrixDirty = true;
}
/*********************************************************************************/
void CameraComponent::setNearClipDistance(float p_Near)
{
  m_NearClipDistance = p_Near;
  m_ProjMatrixDirty = true;
}
/*********************************************************************************/
void CameraComponent::setProjectionType(Enum p_ProjectionType)
{
	m_ProjectionType = p_ProjectionType;
	m_ProjMatrixDirty = true;
	getProjectionMatrix();
}
/*********************************************************************************/
void CameraComponent::setProjectionMatrixDirty(bool p_Dirty)
{
	m_ProjMatrixDirty = p_Dirty;
}
/*********************************************************************************/
void CameraComponent::setViewMatrixDirty(bool p_Dirty)
{
	m_ViewMatrixDirty = p_Dirty;
}
/*********************************************************************************/
bool CameraComponent::isProjectionMatrixDirty() const
{
	return m_ProjMatrixDirty;
}
/*********************************************************************************/