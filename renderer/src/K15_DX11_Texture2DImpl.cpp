/**
 * @file K15_DX11_Texture2DImpl.cpp
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
 */

#include "K15_DX11_Texture2DImpl.h"
#include "K15_UsefulMacros.h"
#include "K15_LogManager.h"

#include "D3DX11.h"
#include "Dxerr.h"

using namespace K15_Renderer;

DX11_Texture2DImpl::DX11_Texture2DImpl(ID3D11Device *pDevice)
	:	m_pResourceView(NULL),
		m_pTexture2D(NULL),
		m_pDevice(pDevice)
{

}

DX11_Texture2DImpl::~DX11_Texture2DImpl()
{
	RELEASE_COM(m_pResourceView);
}

bool DX11_Texture2DImpl::Load(byte *pBuffer,U32 iSize,const String &sExtension)
{
	RELEASE_COM(m_pResourceView);

	HRESULT result = D3DX11CreateShaderResourceViewFromMemory(m_pDevice,pBuffer,iSize,NULL,NULL,&m_pResourceView,NULL);

	if(result != S_OK){
		K15_LogNormalMessage("Could not create Texture2D");
		K15_LogNormalMessage(DXGetErrorString(result));
		K15_LogNormalMessage(DXGetErrorDescription(result));
		return false;
	}

	m_pResourceView->GetResource((ID3D11Resource**)&m_pTexture2D);

	D3D11_TEXTURE2D_DESC textureDesc;
	m_pTexture2D->GetDesc(&textureDesc);

	m_iAmountMipMaps = textureDesc.MipLevels;
	m_iHeight = textureDesc.Height;
	m_iWidth = textureDesc.Width;
	
	//@todo check for alpha format.

	return true;
}

ID3D11ShaderResourceView *DX11_Texture2DImpl::GetResourceView()
{
	return m_pResourceView;
}

ID3D11Texture2D *DX11_Texture2DImpl::GetTexture2D()
{
	return m_pTexture2D;
}
