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
inline void TextureSamplerImplBase::setTextureSampler(TextureSampler* p_Sampler)
{
  m_Sampler = p_Sampler;
}
/*********************************************************************************/
inline TextureSampler* TextureSamplerImplBase::getTextureSampler() const
{
  return m_Sampler;
}
/*********************************************************************************/

/*********************************************************************************/
inline const ColorRGBA& TextureSampler::getBorderColor() const
{
  return m_BorderColor;
}
/*********************************************************************************/
inline Enum TextureSampler::getMinFilterMode() const
{
  return m_MinFilterMode;
}
/*********************************************************************************/
inline Enum TextureSampler::getMagFilterMode() const
{
  return m_MagFilterMode;
}
/*********************************************************************************/
inline Enum TextureSampler::getUWrappingMode() const
{
  return m_UWrappingMode;
}
/*********************************************************************************/
inline Enum TextureSampler::getVWrappingMode() const
{
  return m_VWrappingMode;
}
/*********************************************************************************/
inline Enum TextureSampler::getWWrappingMode() const
{
  return m_WWrappingMode;
}
/*********************************************************************************/
inline void TextureSampler::setMinFilterMode(Enum p_MinFilterMode)
{
  m_MinFilterMode = p_MinFilterMode;
  p_Impl->setMinFilterMode(p_MinFilterMode);
}
/*********************************************************************************/
inline void TextureSampler::setMagFilterMode(Enum p_MagFilterMode)
{
  m_MagFilterMode = p_MagFilterMode;
  p_Impl->setMagFilterMode(p_MagFilterMode);
}
/*********************************************************************************/
inline void TextureSampler::setUWrappingMode(Enum p_UWrappingMode)
{
  m_UWrappingMode = p_UWrappingMode;
  p_Impl->setUWrappingMode(p_UWrappingMode);
}
/*********************************************************************************/
inline void TextureSampler::setVWrappingMode(Enum p_VWrappingMode)
{
  m_VWrappingMode = p_VWrappingMode;
  p_Impl->setVWrappingMode(p_VWrappingMode);
}
/*********************************************************************************/
inline void TextureSampler::setWWrappingMode(Enum p_WWrappingMode)
{
  m_WWrappingMode = p_WWrappingMode;
  p_Impl->setWWrappingMode(p_WWrappingMode);
}
/*********************************************************************************/
inline void TextureSampler::setBorderColor(const ColorRGBA& p_BorderColor)
{
  m_BorderColor = p_BorderColor;
}
/*********************************************************************************/