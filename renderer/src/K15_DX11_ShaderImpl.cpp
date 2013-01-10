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
 */

#include "K15_DX11_ShaderImpl.h"
#include "K15_UsefulMacros.h"
#include "K15_LogManager.h"

#include "D3DX11async.h"
#include "d3d11.h"
#include "dxerr.h"

using namespace K15_Renderer;

D3D10_SHADER_MACRO* CreateDX11ShaderMacro(String sDefines)
{
	U32 iAmountDefines = 0;
	for(U32 i = 0;i < sDefines.Size();++i){
		if(sDefines[i] == '-'){
			++iAmountDefines;
		}
	}

	U32 i = 0;
	S32 iPosition = 0;
	S32 iNextPosition = 0;

	D3D10_SHADER_MACRO *pShaderDefines = K15_NEW D3D10_SHADER_MACRO[iAmountDefines];
	iPosition = sDefines.FindFirstOf('-');
	while(iPosition != -1){
		sDefines = sDefines.SubString(++iPosition);
		iNextPosition = sDefines.FindFirstOf('-');
		
		if(iNextPosition == -1){
			iNextPosition = sDefines.Size();
		}

		String sCurrentDefineName = sDefines.SubString(0,iNextPosition);
		String sCurrentDefineValue = "";

		S32 iDefineValuePos = sCurrentDefineName.FindFirstOf('=');
		if(iDefineValuePos != -1){
			sCurrentDefineValue = sCurrentDefineName.SubString(iDefineValuePos,sCurrentDefineName.Size());
		}

		pShaderDefines[i].Name = sCurrentDefineName.C_Str();
		pShaderDefines[i].Definition = sCurrentDefineValue.C_Str();
		++i;
	}

	return pShaderDefines;
}

DX11_ShaderImpl::DX11_ShaderImpl(ID3D11Device *pDevice)
	:	m_pCompiledShader(NULL),
		m_pVertexShader(NULL),
		m_pPixelShader(NULL),
		m_pGeometryShader(NULL),
		m_pDevice(pDevice)
{

}

DX11_ShaderImpl::~DX11_ShaderImpl()
{
	RELEASE_COM(m_pCompiledShader);
	RELEASE_COM(m_pVertexShader);
	RELEASE_COM(m_pPixelShader);
	RELEASE_COM(m_pGeometryShader);
}

bool DX11_ShaderImpl::Compile(const char *pDefines)
{
	if(m_stType == ShaderImpl::ST_NONE){
		m_sErrorString = "Could not compile shader. ShaderType was not set.";
		WriteDefaultLog(m_sErrorString);
		return false;
	}

	D3D10_SHADER_MACRO *pShaderDefines = CreateDX11ShaderMacro(pDefines);
	ID3D10Blob *pErrorMessage;
	bool bReturn = false;

	HRESULT result = D3DX11CompileFromMemory(m_sShaderCode.C_Str(),m_sShaderCode.Size(),
		"",pShaderDefines,
		NULL,"main",NULL,0,0,NULL,
		&m_pCompiledShader,&pErrorMessage,NULL);
	
	//Did the shader compile successfully?
	if(result == S_FALSE){
		if(pErrorMessage){
			//Shader was not compiled successfully.
			//Copy Error message to error string.
			m_sErrorString = (const char*)pErrorMessage->GetBufferPointer();

			WriteDefaultLog(String("Could not compile shader."));
			WriteDefaultLog(m_sErrorString);
		}
	}else{
		bReturn = true;
		//Clear error message if shader was compiled successfully.
		if(!m_sErrorString.IsEmpty()){
			m_sErrorString.Clear();
		}

		if(m_stType == ShaderImpl::ST_VERTEXSHADER){
			RELEASE_COM(m_pVertexShader);
			m_pDevice->CreateVertexShader(m_pCompiledShader->GetBufferPointer(),m_pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader);
		}else if(m_stType = ShaderImpl::ST_FRAGMENTSHADER){
			RELEASE_COM(m_pPixelShader);
			m_pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(),m_pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader);
		}else{ //Geometryshader
			RELEASE_COM(m_pGeometryShader);
			m_pDevice->CreateGeometryShader(m_pCompiledShader->GetBufferPointer(),m_pCompiledShader->GetBufferSize(),NULL,&m_pGeometryShader);
		}
	}

	K15_DELETE[] pShaderDefines;

	return bReturn;
}

ID3D10Blob *DX11_ShaderImpl::GetCompiledShader()
{
	return m_pCompiledShader;
}

ID3D11VertexShader *DX11_ShaderImpl::GetVertexShader()
{
	return m_pVertexShader;
}

ID3D11PixelShader *DX11_ShaderImpl::GetPixelShader()
{
	return m_pPixelShader;
}

ID3D11GeometryShader *DX11_ShaderImpl::GetGeometryShader()
{
	return m_pGeometryShader;
}