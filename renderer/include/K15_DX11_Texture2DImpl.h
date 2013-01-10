/**
 * @file K15_DX11_Texture2D.h
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

#ifndef __K15_DX11_TEXTURE2DIMPL__
#define __K15_DX11_TEXTURE2DIMPL__

#include "K15_Texture2DImpl.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11Device;

namespace K15_Renderer
{
	class DX11_Texture2DImpl : public Texture2DImpl
	{
	public:
		DX11_Texture2DImpl(ID3D11Device *pDevice);
		virtual ~DX11_Texture2DImpl();

		/**
		 * Loads the texture using the D3DX11CreateShaderResourceViewFromMemory function.
		 * @note Width, Height and AmountMitMaps will get set during this call (if texture was loaded successfully)
		 *
		 * @param pBuffer - Content of the texture file.
		 * @param iSize - Size of the texture file.
		 * @param sExtension - Extension of the texture file.
		 * 
		 * @return bool - true if Texture was loaded successfully and false if not.
		 */
		virtual bool Load(byte *pBuffer,U32 iSize,const String &sExtension);

		ID3D11ShaderResourceView *GetResourceView();
		ID3D11Texture2D *GetTexture2D();
	private:
		ID3D11Texture2D *m_pTexture2D;
		ID3D11ShaderResourceView *m_pResourceView;
		ID3D11Device *m_pDevice;
	};
}

#endif //__K15_DX11_TEXTURE2D__