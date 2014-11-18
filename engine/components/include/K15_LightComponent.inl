/**
 * @file K15_LightComponent.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/04
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
Enum LightComponent::getLightType() const
{
	return m_LightType;
}
/*********************************************************************************/
float LightComponent::getRadius() const
{
	return m_Range;
}
/*********************************************************************************/
float LightComponent::getConeAngle() const
{
	return m_ConeAngle;
}
/*********************************************************************************/
float LightComponent::getConstantAttenuation() const
{
	return m_Attenuation.x;
}
/*********************************************************************************/
float LightComponent::getLinearAttenuation() const
{
	return m_Attenuation.y;
}
/*********************************************************************************/
float LightComponent::getQuadricAttenuation() const
{
	return m_Attenuation.z;
}
/*********************************************************************************/
float LightComponent::getSpecularExponent() const
{
	return m_SpecularExponent;
}
/*********************************************************************************/
const Vector3& LightComponent::getDirection() const
{
	return m_Direction;
}
/*********************************************************************************/
const Vector3& LightComponent::getAttenuation() const
{
	return m_Attenuation;
}
/*********************************************************************************/
const ColorRGBA& LightComponent::getDiffuseColor() const
{
	return m_DiffuseColor;
}
/*********************************************************************************/
const ColorRGBA& LightComponent::getSpecularColor() const
{
  return m_SpecularColor;
}
/*********************************************************************************/
bool LightComponent::isActive() const
{
	return m_Active;
}
/*********************************************************************************/
void LightComponent::setDirty(bool p_Dirty)
{
	m_Dirty = p_Dirty;
}
/*********************************************************************************/
void LightComponent::setLightType(Enum p_LightType)
{
	K15_ASSERT(p_LightType < LT_COUNT,"Invalid light type.");
	m_LightType = p_LightType;
}
/*********************************************************************************/
void LightComponent::setRadius(float p_Radius)
{
	m_Range = p_Radius;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setConstantAttenuation(float p_ConstAttenuation)
{
	m_Attenuation.x = p_ConstAttenuation;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setLinearAttenuation(float p_LinearAttenuation)
{
	m_Attenuation.y = p_LinearAttenuation;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setQuadricAttenuation(float p_QuadricAttenuation)
{
	m_Attenuation.z = p_QuadricAttenuation;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setConeAngle(float p_ConeAngle)
{
	m_ConeAngle = p_ConeAngle;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setSpecularExponent(float p_SpecularExponent)
{
	m_SpecularExponent = p_SpecularExponent;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setActive(bool p_Active)
{
	m_Active = p_Active;
}
/*********************************************************************************/
void LightComponent::setDiffuseColor(const ColorRGBA& p_DiffuseColor)
{
	m_DiffuseColor = p_DiffuseColor;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setSpecularColor(const ColorRGBA& p_SpecularColor)
{
	m_SpecularColor = p_SpecularColor;
	m_Dirty = true;
}
/*********************************************************************************/
void LightComponent::setDirection(const Vector3& p_Direction)
{
	m_Direction = p_Direction;
	m_Dirty = true;
}
/*********************************************************************************/