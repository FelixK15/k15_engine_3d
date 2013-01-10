/**
 * @file K15_Texture2D.h
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
#pragma once

#ifndef __K15_TEXTURE2D__
#define __K15_TEXTURE2D__

#include "K15_Resource.h"
#include "K15_Texture2DImpl.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class Texture2D : public Resource
	{
		DECLARE_RTTI;

	public:
		Texture2D();
		virtual ~Texture2D();

		inline bool HasAlpha() const;
		inline U32 GetWidth() const;
		inline U32 GetHeight() const;

		inline virtual bool Load(byte *pBuffer,U32 iSize,const String &sExtension);
		inline U32 GetAmountMipMaps() const;

	protected:
		Texture2DImpl *m_pTexture2DImpl;
	};
	#include "..\src\K15_Texture2D.inl"
	typedef Pointer<Texture2D> TexturePtr;
}

#endif //__K15_TEXTURE2D__