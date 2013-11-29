/**
 * @file K15_Material.inl
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
const ColorRGBA& Material::getSpecular() const
{
	return m_Specular;
}
/*********************************************************************************/
const ColorRGBA& Material::getDiffuse() const
{
	return m_Diffuse;
}
/*********************************************************************************/
const ColorRGBA& Material::getAmbient() const
{
	return m_Ambient;
}
/*********************************************************************************/
float Material::getShininess() const
{
	return m_Shininess;
}
/*********************************************************************************/
GpuProgram* Material::getProgram(Enum p_Stage) const
{
	return m_Programs[p_Stage];
}
/*********************************************************************************/
Texture* Material::getDiffuseMap() const
{
	return m_DiffuseMap;
}
/*********************************************************************************/
TextureSampler* Material::getDiffuseSampler() const
{
	return m_DiffuseSampler;
}
/*********************************************************************************/
inline void Material::setSpecular(const ColorRGBA& p_Specular)
{
	m_Specular = p_Specular;
}
/*********************************************************************************/
inline void Material::setDiffuse(const ColorRGBA& p_Diffuse)
{
	m_Diffuse = p_Diffuse;
}
/*********************************************************************************/
inline void Material::setAmbient(const ColorRGBA& p_Ambient)
{
	m_Ambient = p_Ambient;
}
/*********************************************************************************/
inline void Material::setShininess(float p_Hightlight)
{
	m_Shininess = p_Hightlight;
}
/*********************************************************************************/