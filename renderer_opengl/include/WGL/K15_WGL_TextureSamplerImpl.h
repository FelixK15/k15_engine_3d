/**
 * @file K15_TextureSamplerImpl.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_RendererWGL_TextureSamplerImpl_h_
#define _K15Engine_RendererWGL_TextureSamplerImpl_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_TextureSampler.h"

namespace K15_Engine { namespace Rendering { namespace WGL {
  class TextureSamplerImpl : public TextureSamplerImplBase
  {
  public:
    /*********************************************************************************/
    static const GLenum GLFilterModeConverter[TextureSampler::TFM_COUNT];
    static const GLenum GLWrappingModeConverter[TextureSampler::TWM_COUNT];
    /*********************************************************************************/
  public:
    TextureSamplerImpl();
    ~TextureSamplerImpl();

    virtual void setMinFilterMode(Enum p_MinFilterMode) OVERRIDE;
    virtual void setMagFilterMode(Enum p_MagFilterMode) OVERRIDE;

    virtual void setUWrappingMode(Enum p_WrappingMode) OVERRIDE;
    virtual void setVWrappingMode(Enum p_WrappingMode) OVERRIDE;
    virtual void setWWrappingMode(Enum p_WrappingMode) OVERRIDE;

    INLINE GLuint getHandle() const;
  private:
    GLuint m_SamplerHandle;
  };// end of TextureSamplerImpl class declaration
#	include "K15_WGL_TextureSamplerImpl.inl"
}}}//end of K15_Engine::Rendering::WGL namespace

#endif //_K15Engine_RendererWGL_TextureSamplerImpl_h_