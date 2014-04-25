/**
 * @file K15_TextureImpl.cpp
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

#include "K15_WGL_TextureSamplerImpl.h"

namespace K15_Engine { namespace Rendering { namespace WGL {
  /*********************************************************************************/
  const GLenum TextureSamplerImpl::GLFilterModeConverter[TextureSampler::TFM_COUNT] = {
    GL_LINEAR,    //TFM_LINEAR
    GL_NEAREST    //TFM_NEAREST
  }; //GLFilterModeConverter
  /*********************************************************************************/
  const GLenum TextureSamplerImpl::GLWrappingModeConverter[TextureSampler::TWM_COUNT] = {
    GL_REPEAT,            //TWM_REPEAT,
    GL_CLAMP,             //TWM_CLAMP
    GL_CLAMP_TO_BORDER,   //TWM_BORDERCOLOR
    GL_MIRRORED_REPEAT    //TWM_MIRROR
  }; //GLWrappingModeConverter
  /*********************************************************************************/

  /*********************************************************************************/
  TextureSamplerImpl::TextureSamplerImpl()
    : m_SamplerHandle(0)
  {
    glGenSamplers(1,&m_SamplerHandle);

  }
  /*********************************************************************************/
  TextureSamplerImpl::~TextureSamplerImpl()
  {
    glDeleteSamplers(1,&m_SamplerHandle);
  }
  /*********************************************************************************/
  void TextureSamplerImpl::setMinFilterMode(Enum p_MinFilterMode)
  {
    glSamplerParameteri(m_SamplerHandle, GL_TEXTURE_MIN_FILTER, GLFilterModeConverter[p_MinFilterMode]);
  }
  /*********************************************************************************/
  void TextureSamplerImpl::setMagFilterMode(Enum p_MagFilterMode)
  {
    glSamplerParameteri(m_SamplerHandle, GL_TEXTURE_MAG_FILTER, GLFilterModeConverter[p_MagFilterMode]);
  }
  /*********************************************************************************/
  void TextureSamplerImpl::setUWrappingMode(Enum p_WrappingMode)
  {
    glSamplerParameteri(m_SamplerHandle, GL_TEXTURE_WRAP_S, GLWrappingModeConverter[p_WrappingMode]);
  }
  /*********************************************************************************/
  void TextureSamplerImpl::setVWrappingMode(Enum p_WrappingMode)
  {
    glSamplerParameteri(m_SamplerHandle, GL_TEXTURE_WRAP_T, GLWrappingModeConverter[p_WrappingMode]);
  }
  /*********************************************************************************/
  void TextureSamplerImpl::setWWrappingMode(Enum p_WrappingMode)
  {
    glSamplerParameteri(m_SamplerHandle, GL_TEXTURE_WRAP_R, GLWrappingModeConverter[p_WrappingMode]);
  }
  /*********************************************************************************/
}}}//end of K15_Engine::Rendering::WGL namespace
