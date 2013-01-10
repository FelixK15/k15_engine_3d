/**
 * @file K15_Texture2DImpl.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/20
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

#ifndef __K15_TEXTURE2DIMPL__
#define __K15_TEXTURE2DIMPL__

#include "K15_StdInclude.h"
#include "K15_String.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class Texture2DImpl
	{
	public:
		virtual ~Texture2DImpl();

		inline bool HasAlpha() const;
		inline U32 GetWidth() const;
		inline U32 GetHeight() const;
		inline U32 GetAmountMipMaps() const;

		//inline PixelFormat GetPixelFormat() const;
		//virtual void SetAmountMipMaps(U32 iMapMaps) = 0;
		virtual bool Load(byte *pBuffer,U32 iSize,const String &sExtension) = 0;
		
	protected:
		Texture2DImpl();

	protected:
		bool m_bAlpha;
		U32 m_iWidth;
		U32 m_iHeight;
		U32 m_iAmountMipMaps;
	};
	#include "..\src\K15_Texture2DImpl.inl"
}

#endif //__K15_TEXTURE2DIMPL__