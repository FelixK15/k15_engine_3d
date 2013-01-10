/**
 * @file K15_DX11_Shader.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/23
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

#ifndef __K15_DX11_SHADER__
#define __K15_DX11_SHADER__

#include "K15_ShaderImpl.h"

struct ID3D10Blob;
struct ID3D11Device;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;

namespace K15_Renderer
{
	class DX11_ShaderImpl : public ShaderImpl
	{
	public:
		DX11_ShaderImpl(ID3D11Device *pDevice);
		virtual ~DX11_ShaderImpl();

		virtual bool Compile(const char *pDefines);

		ID3D10Blob *GetCompiledShader();
		ID3D11VertexShader *GetVertexShader();
		ID3D11PixelShader *GetPixelShader();
		ID3D11GeometryShader *GetGeometryShader();
		
	private:
		ID3D11VertexShader *m_pVertexShader;
		ID3D11PixelShader *m_pPixelShader;
		ID3D11GeometryShader *m_pGeometryShader;

		ID3D10Blob *m_pCompiledShader;
		ID3D11Device *m_pDevice;
	};
}

#endif //__K15_DX11_SHADER__