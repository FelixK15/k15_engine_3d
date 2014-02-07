/**
 * @file K15_AlphaState.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/13
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

#ifndef _K15Engine_Renderer_AlphaState_h_
#define _K15Engine_Renderer_AlphaState_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_ColorRGBA.h"
#include "K15_Serializer.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API AlphaState : public SerializableObject<AlphaState>
	{
	public:
		/*********************************************************************************/
		enum eBlendFunction
		{
			BF_ZERO = 0,
			BF_ONE,

			BF_SRC_COLOR,
			BF_ONE_MINUS_SRC_COLOR,

			BF_DST_COLOR,
			BF_ONE_MINUS_DST_COLOR,

			BF_SRC_ALPHA,
			BF_ONE_MINUS_SRC_ALPHA,

			BF_DST_ALPHA,
			BF_ONE_MINUS_DST_ALPHA,

			BF_CONSTANT_COLOR,
			BF_ONE_MINUS_CONSTANT_COLOR,

			BF_CONSTANT_ALPHA,
			BF_ONE_MINUS_CONSTANT_ALPHA,

			BF_COUNT
		}; //BlendMode
		/*********************************************************************************/
    enum eBlendOperation
    {
      BO_ADD = 0,
      BO_SUBTRACT,
      BO_REVERSE_SUBTRACT,
      BO_MIN,
      BO_MAX,

      BO_COUNT
    }; //BlendEquation
    /*********************************************************************************/
	public:
		AlphaState();
		virtual ~AlphaState();

    INLINE bool getEnabled() const;
    INLINE Enum getDestinationBlendFunction() const;
    INLINE Enum getSourceBlendFunction() const;
    INLINE Enum getBlendOperation() const;
    INLINE const ColorRGBA& getConstantColor() const;
    
    INLINE void setEnabled(bool p_Enabled);
    INLINE void setDestinationBlendFunction(Enum p_DestinationBlendFunction);
    INLINE void setSourceBlendFunction(Enum p_SourceBlendFunction);
    INLINE void setBlendOperation(Enum p_BlendOperation);
    INLINE void setConstantColor(const ColorRGBA& p_ConstantColor);

    bool operator !=(const AlphaState& p_Other) const;

  private:
    expose ColorRGBA m_ConstantColor;
    expose Enum m_DestinationBlendFunction;
    expose Enum m_SourceBlendFunction;
    expose Enum m_BlendOperation;
    expose bool m_Enabled;
	};// end of AlphaState class declaration
#include "K15_AlphaState.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Renderer_AlphaState_h_