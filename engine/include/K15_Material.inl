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
GpuProgramBatch* MaterialPass::getProgramBatch() const
{
	return m_ProgramBatch;
}
/*********************************************************************************/
void MaterialPass::setProgram(GpuProgram* p_Program, Enum p_Stage)
{
	m_Programs[p_Stage] = p_Program;
}
/*********************************************************************************/
void MaterialPass::setProgramBatch(GpuProgramBatch* p_ProgramBatch)
{
	m_ProgramBatch = p_ProgramBatch;
}
/*********************************************************************************/
ColorRGBA& MaterialPass::getSpecular()
{
	return m_Specular;
}
/*********************************************************************************/
ColorRGBA& MaterialPass::getDiffuse()
{
	return m_Diffuse;
}
/*********************************************************************************/
ColorRGBA& MaterialPass::getAmbient()
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
void MaterialPass::setFillMode(Enum p_FillMode)
{
  m_FillMode = p_FillMode;
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
Enum MaterialPass::getFillMode() const
{
  return m_FillMode;
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
DepthState& MaterialPass::getDepthState()
{
	return m_DepthState;
}
/*********************************************************************************/
void MaterialPass::setDepthState(const DepthState& p_DepthState)
{
	m_DepthState = p_DepthState;
}
/*********************************************************************************/
AlphaState& MaterialPass::getAlphaState()
{
	return m_AlphaState;
}
/*********************************************************************************/
void MaterialPass::setAlphaState(const AlphaState& p_AlphaState)
{
	m_AlphaState = p_AlphaState;
}
/*********************************************************************************/
bool MaterialPass::isEnabled() const
{
	return m_PassEnabled;
}
/*********************************************************************************/
void MaterialPass::setEnabled(bool p_Enabled)
{
	m_PassEnabled = p_Enabled;
}
/*********************************************************************************/

/*********************************************************************************/
uint32 Material::getAmountPasses() const
{
	return m_Passes.size();
}
/*********************************************************************************/
MaterialPass* Material::getPass(uint32 p_Index, bool p_EnablePass)
{
	K15_ASSERT(p_Index < m_Passes.size(),"Trying to access MaterialPass out of bounds.");

	if(p_EnablePass)
	{
		m_Passes[p_Index].setEnabled(true);
	}
	
	return &m_Passes[p_Index];
}
/*********************************************************************************/
void Material::enablePass(uint32 p_Index)
{
	K15_ASSERT(p_Index < m_Passes.size(),"Trying to access MaterialPass out of bounds.");
	
	m_Passes[p_Index].setEnabled(true);
}
/*********************************************************************************/
void Material::disablePass(uint32 p_Index)
{
	K15_ASSERT(p_Index < m_Passes.size(),"Trying to access MaterialPass out of bounds.");

	m_Passes[p_Index].setEnabled(false);
}
/*********************************************************************************/