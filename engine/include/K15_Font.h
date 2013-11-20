/**
 * @file K15_Font.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/12
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

#ifndef _K15Engine_Core_Font_h_
#define _K15Engine_Core_Font_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceBase.h"
#include "K15_ResourceHandle.h"

struct FT_Face;

namespace K15_Engine { namespace Core {
	class K15_CORE_API Font : public ResourceBase
	{
	public:
		Font();
		virtual ~Font();

		virtual void loadDebug(RawData& p_Data);
		virtual bool internalLoad(const RawData& p_Data);

	private:
		ResourceHandle<TrueTypeFont> m_Font;
		ResourceHandle<Texture> m_Texture;
		ObjectName m_FontName;
		uint8 m_StartGlyph;
		uint8 m_EndGlyph;
		uint8 m_GlyphSize;
		uint8 m_Resolution;
	};// end of Font class declaration
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Font_h_