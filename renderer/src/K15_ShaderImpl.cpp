/**
 * @file K15_ShaderImpl.cpp
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

#include "K15_ShaderImpl.h"
using namespace K15_Renderer;

ShaderImpl::ShaderImpl()
	:	m_bCompiled(false),
		m_stType(ShaderType::ST_NONE),
{

}

ShaderImpl::~ShaderImpl()
{
	//Delete the shader variables.
	for(U32 i = 0;i < m_hmShaderVariables.Size();++i){
		for(HashItem<const char*,ShaderVariable*> *pItem = m_hmShaderVariables.GetBucketItem(i);pItem;pItem = pItem->GetNext()){
			K15_DELETE pItem->GetValue();
		}
	}

	m_hmShaderVariables.Clear();
}

ShaderVariable *ShaderImpl::GetShaderVariable( const char *pVariableName )
{
	HashItem<const char*,ShaderVariable*> *hiItem = m_hmShaderVariables.Get(pVariableName);
	ShaderVariable *pShaderVariable = NULL;

	if(hiItem){
		pShaderVariable = hiItem->GetValue();
	}

	return pShaderVariable;
}

void ShaderImpl::AddShaderVariable( const char *pVariableName,ShaderVariable *pShaderVariable )
{
	m_hmShaderVariables.Insert(pVariableName,pShaderVariable);
}

bool ShaderImpl::Load(byte *pBuffer,U32 iSize,const String &sExtension)
{
	if(!m_sShaderCode.IsEmpty()){
		m_sShaderCode.Clear();
	}

	m_sShaderCode = (const char*)pBuffer;

	return true;
}

