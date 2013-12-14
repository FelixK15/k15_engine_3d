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
GpuProgram* MaterialPass::getProgram(Enum p_Stage) const
{
	return m_Programs[p_Stage];
}
/*********************************************************************************/
void MaterialPass::setProgram(GpuProgram* p_Program, Enum p_Stage)
{
	m_Programs[p_Stage] = p_Program;
}
/*********************************************************************************/
const ColorRGBA& MaterialPass::getSpecular() const
{
	return m_Specular;
}
/*********************************************************************************/
const ColorRGBA& MaterialPass::getDiffuse() const
{
	return m_Diffuse;
}
/*********************************************************************************/
const ColorRGBA& MaterialPass::getAmbient() const
{
	return m_Ambient;
}
/*********************************************************************************/
float MaterialPass::getShininess() const
{
	return m_Shininess;
}
/*********************************************************************************/
void MaterialPass::setSpecular(const ColorRGBA& p_Specular)
{
	m_Specular = p_Specular;
}
/*********************************************************************************/
void MaterialPass::setDiffuse(const ColorRGBA& p_Diffuse)
{
	m_Diffuse = p_Diffuse;
}
/*********************************************************************************/
void MaterialPass::setAmbient(const ColorRGBA& p_Ambient)
{
	m_Ambient = p_Ambient;
}
/*********************************************************************************/
void MaterialPass::setShininess(float p_Hightlight)
{
	m_Shininess = p_Hightlight;
}
/*********************************************************************************/
Texture* MaterialPass::getDiffuseMap() const
{
	return m_DiffuseMap;
}
/*********************************************************************************/
TextureSampler* MaterialPass::getDiffuseSampler() const
{
	return m_DiffuseSampler;
}
/*********************************************************************************/
void MaterialPass::setDiffuseMap(Texture* p_DiffuseMap)
{
	m_DiffuseMap = p_DiffuseMap;
}
/*********************************************************************************/
void MaterialPass::setDiffuseSampler(TextureSampler* p_DiffuseSampler)
{
	m_DiffuseSampler = p_DiffuseSampler;
}
/*********************************************************************************/
bool MaterialPass::isTransparent() const
{
	return m_Transparent;
}
/*********************************************************************************/
void MaterialPass::setTransparent(bool p_Transparent)
{
	m_Transparent = p_Transparent;
}
/*********************************************************************************/
bool MaterialPass::isLightningEnabled() const
{
	return m_LightningEnabled;
}
/*********************************************************************************/
void MaterialPass::setLightningEnabled(bool p_LightReceive) 
{
	m_LightningEnabled = p_LightReceive;
}
/*********************************************************************************/
Enum MaterialPass::getCullingMode() const
{
	return m_CullingMode;
}
/*********************************************************************************/
void MaterialPass::setCullingMode(Enum p_CullingMode)
{
	m_CullingMode = p_CullingMode;
}
/*********************************************************************************/
Enum MaterialPass::getDepthTestFunction() const
{
	return m_DepthTestFunction;
}
/*********************************************************************************/
void MaterialPass::setDepthTestFunction(Enum p_DepthTestFunction)
{
	m_DepthTestFunction = p_DepthTestFunction;
}
/*********************************************************************************/
bool MaterialPass::isDepthTestEnabled() const
{
	return m_DepthTestEnabled;
}
/*********************************************************************************/
void MaterialPass::setDepthTestEnabled(bool p_DepthTestEnabled)
{
	m_DepthTestEnabled = p_DepthTestEnabled;
}
/*********************************************************************************/
AlphaState* MaterialPass::getAlphaState() const
{
	return m_AlphaState;
}
/*********************************************************************************/
void MaterialPass::setAlphaState(AlphaState* p_AlphaState)
{
	m_AlphaState = p_AlphaState;
}
/*********************************************************************************/

/*********************************************************************************/
uint32 Material::getAmountPasses() const
{
	return m_Passes.size();
}
/*********************************************************************************/
MaterialPass* Material::getPass(uint32 p_Index) const
{
	K15_ASSERT(p_Index < m_Passes.size(),"Trying to access MaterialPass out of bounds.");

	return m_Passes.at(p_Index);
}
/*********************************************************************************/