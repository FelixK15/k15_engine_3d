/**
 * @file K15_DepthState.h
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

#ifndef _K15Engine_Renderer_DepthState_h_
#define _K15Engine_Renderer_DepthState_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API DepthState
	{
	public:
		DepthState();
		virtual ~DepthState();

		INLINE bool getEnabled() const;
		INLINE float getBias() const;
		INLINE Enum getFunction() const;

		INLINE void setEnabled(bool p_Enabled);
		INLINE void setBias(float p_Bias);
		void setFunction(Enum p_Function);

		bool operator!=(const DepthState& p_Other) const;

	private:
		expose Enum m_Function;
		expose float m_Bias;
		expose bool m_Enabled;
	};// end of AlphaState class declaration
#include "K15_DepthState.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Renderer_DepthState_h_