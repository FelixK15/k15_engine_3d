/**
 * @file K15_TextureSampler.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_TextureSampler_h_
#define _K15Engine_Renderer_TextureSampler_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering { 
  /*********************************************************************************/
  class K15_CORE_API TextureSamplerImplBase : public RenderingAllocatedObject
  {
  public:
    virtual void setMinFilterMode(Enum p_MinFilterMode) = 0;
    virtual void setMagFilterMode(Enum p_MagFilterMode) = 0;

    virtual void setUWrappingMode(Enum p_WrappingMode) = 0;
    virtual void setVWrappingMode(Enum p_WrappingMode) = 0;
    virtual void setWWrappingMode(Enum p_WrappingMode) = 0;

    INLINE void setTextureSampler(TextureSampler* p_Sampler);
    INLINE TextureSampler* getTextureSampler() const;

  protected:
    TextureSampler* m_Sampler;
  };// end of TextureSamplerImplBase class declaration
  /*********************************************************************************/

  /*********************************************************************************/
  class K15_CORE_API TextureSampler : public RenderingAllocatedObject
  {
  public:
    /*********************************************************************************/
    enum eTextureWrappingMode
    {
      TWM_REPEAT = 0,
      TWM_CLAMP,
      TWM_BORDERCOLOR,
      TWM_MIRROR,

      TWM_COUNT
    }; //TextureWrappingMode
    /*********************************************************************************/
    enum eTextureFilterMode
    {
      TFM_LINEAR = 0,
      TFM_NEAREST,

      TFM_COUNT
    }; //TextureFilterMode
    /*********************************************************************************/

  public:
    TextureSampler();
    ~TextureSampler();

    INLINE const ColorRGBA& getBorderColor() const;

    INLINE Enum getMinFilterMode() const;
    INLINE Enum getMagFilterMode() const;

    INLINE Enum getUWrappingMode() const;
    INLINE Enum getVWrappingMode() const;
    INLINE Enum getWWrappingMode() const;

    INLINE Enum getSlot() const;

    Enum getWrappingMode(uint8 p_Axis) const;

    INLINE void setMinFilterMode(Enum p_MinFilterMode);
    INLINE void setMagFilterMode(Enum p_MagFilterMode);

    INLINE void setUWrappingMode(Enum p_UWrappingMode);
    INLINE void setVWrappingMode(Enum p_VWrappingMode);
    INLINE void setWWrappingMode(Enum p_WWrappingMode);

    INLINE void setBorderColor(const ColorRGBA& p_BorderColor);

    INLINE void setSlot(Enum p_Slot);

    void setWrappingMode(uint8 p_Axis,Enum p_WrappingMode);

	INLINE TextureSamplerImplBase* getImpl() const;

  private:
    TextureSamplerImplBase* m_Impl;
    ColorRGBA m_BorderColor;

    Enum m_Slot;

    Enum m_UWrappingMode;
    Enum m_VWrappingMode;
    Enum m_WWrappingMode;

    Enum m_MinFilterMode;
    Enum m_MagFilterMode;
  };// end of TextureSampler class declaration
# include "K15_TextureSampler.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_TextureSampler_h_