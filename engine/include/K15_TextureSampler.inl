/**
 * @file K15_TextureSampler.inl
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
void TextureSamplerImplBase::setTextureSampler(TextureSampler* p_Sampler)
{
  m_Sampler = p_Sampler;
}
/*********************************************************************************/
TextureSampler* TextureSamplerImplBase::getTextureSampler() const
{
  return m_Sampler;
}
/*********************************************************************************/

/*********************************************************************************/
const ColorRGBA& TextureSampler::getBorderColor() const
{
  return m_BorderColor;
}
/*********************************************************************************/
Enum TextureSampler::getMinFilterMode() const
{
  return m_MinFilterMode;
}
/*********************************************************************************/
Enum TextureSampler::getMagFilterMode() const
{
  return m_MagFilterMode;
}
/*********************************************************************************/
Enum TextureSampler::getUWrappingMode() const
{
  return m_UWrappingMode;
}
/*********************************************************************************/
Enum TextureSampler::getVWrappingMode() const
{
  return m_VWrappingMode;
}
/*********************************************************************************/
Enum TextureSampler::getWWrappingMode() const
{
  return m_WWrappingMode;
}
/*********************************************************************************/
void TextureSampler::setMinFilterMode(Enum p_MinFilterMode)
{
  m_MinFilterMode = p_MinFilterMode;
  m_Impl->setMinFilterMode(p_MinFilterMode);
}
/*********************************************************************************/
void TextureSampler::setMagFilterMode(Enum p_MagFilterMode)
{
  m_MagFilterMode = p_MagFilterMode;
  m_Impl->setMagFilterMode(p_MagFilterMode);
}
/*********************************************************************************/
void TextureSampler::setUWrappingMode(Enum p_UWrappingMode)
{
  m_UWrappingMode = p_UWrappingMode;
  m_Impl->setUWrappingMode(p_UWrappingMode);
}
/*********************************************************************************/
void TextureSampler::setVWrappingMode(Enum p_VWrappingMode)
{
  m_VWrappingMode = p_VWrappingMode;
  m_Impl->setVWrappingMode(p_VWrappingMode);
}
/*********************************************************************************/
void TextureSampler::setWWrappingMode(Enum p_WWrappingMode)
{
  m_WWrappingMode = p_WWrappingMode;
  m_Impl->setWWrappingMode(p_WWrappingMode);
}
/*********************************************************************************/
void TextureSampler::setBorderColor(const ColorRGBA& p_BorderColor)
{
  m_BorderColor = p_BorderColor;
}
/*********************************************************************************/
const TextureSamplerImplBase* TextureSampler::getImpl() const
{
	return m_Impl;
}
/*********************************************************************************/