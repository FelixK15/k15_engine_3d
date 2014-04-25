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
  m_Dirty = true;
}
/*********************************************************************************/
void CameraComponent::setProjectionType(Enum p_ProjectionType)
{
  m_ProjectionType = p_ProjectionType;
  m_Dirty = true;
}
/*********************************************************************************/
void CameraComponent::setFarClipDistance(float p_Far)
{
  m_FarClipDistance = p_Far;
  m_Dirty = true;
}
/*********************************************************************************/
void CameraComponent::setNearClipDistance(float p_Near)
{
  m_NearClipDistance = p_Near;
  m_Dirty = true;
}
/*********************************************************************************/
const CameraComponent::FrustumPoints& CameraComponent::getFrustumPoints() const
{
  return m_FrustumPoints;
}
/*********************************************************************************/