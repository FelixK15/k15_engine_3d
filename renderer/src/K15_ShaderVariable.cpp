/**
 * @file K15_ShaderVariable.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#include "K15_ShaderVariable.h"
#include "K15_Renderer.h"

using namespace K15_Renderer;
using namespace K15_EngineV2;

ShaderVariable::ShaderVariable()
	: m_iFloatPackCount(0),
	  m_pData(NULL),
	  m_pBuffer(NULL),
	  m_iRegisterNumber(0),
	  m_ufFrequenzy(UpdateFrequenzy::UF_MANUAL)
{

}

ShaderVariable::ShaderVariable(U32 iRegisterNumber,U32 iFloatPackCount,UpdateFrequenzy ufFrequenzy,float *pData)
	: m_iFloatPackCount(iFloatPackCount),
	  m_iRegisterNumber(iRegisterNumber),
	  m_ufFrequenzy(ufFrequenzy)
{
	assert(pData);
	assert(m_iFloatPackCount == 0);

	_Allocate(0);
	SetData(pData);
}

ShaderVariable::ShaderVariable(const ShaderVariable &svVariable)
	: m_iFloatPackCount(svVariable.m_iFloatPackCount),
	  m_iRegisterNumber(svVariable.m_iRegisterNumber)
{
	assert(svVariable.m_pData);
	assert(m_iFloatPackCount == 0);

	_Allocate(0);
	if(svVariable.m_pData != NULL){
		SetData(svVariable.m_pData);
	}
}

ShaderVariable::~ShaderVariable()
{
	if(m_pData){
		K15_DELETE[] m_pData;
	}
}

void ShaderVariable::SetByteSize( U32 iByteSize )
{
	assert(iByteSize >= 16);
	assert(iByteSize % 16 == 0);

	U32 iNewRegisterCount = iByteSize / 16;

	if(m_iFloatPackCount != iNewRegisterCount){
		U32 iOldRegisterCount = m_iFloatPackCount;
		m_iFloatPackCount = iNewRegisterCount;
		_Allocate(iOldRegisterCount);
	}
}

void ShaderVariable::SetSingleElementCount( U32 iNumElements )
{
	assert(iNumElements >= 4);
	assert(iNumElements % 4 == 0);

	U32 iNewRegisterCount = iNumElements * 0.25f;

	if(m_iFloatPackCount != iNewRegisterCount){
		U32 iOldRegisterCount = m_iFloatPackCount;
		m_iFloatPackCount = iNewRegisterCount;
		_Allocate(iOldRegisterCount);
	}
}

void ShaderVariable::SetPackElementCount( U32 iNumRegisters )
{
	assert(iNumRegisters > 0);

	if(iNumRegisters != m_iFloatPackCount){
		U32 iOldRegisterCount = m_iFloatPackCount;
		m_iFloatPackCount = iNumRegisters;
		_Allocate(iOldRegisterCount);
	}
}

void ShaderVariable::_Allocate(U32 iOldRegister)
{
	U32 iNumBytes = m_iFloatPackCount * 16;
	U32 iNumElements = m_iFloatPackCount * 4;
	float *pNewData = new float[iNumElements];

	if(m_pData){
		Memory::MemCopy(pNewData,m_pData,iOldRegister);
		K15_DELETE[] m_pData;
		m_pData = NULL;
	}

	m_pBuffer = g_pRenderer->CreateBuffer(pNewData,iNumBytes,iNumBytes,
		FormatType::FT_UNKNOWN,Buffer::BU_DYNAMIC,Buffer::BA_WRITE,Buffer::BB_CONSTANT_BUFFER);
	m_pData = pNewData;
}

void ShaderVariable::SetData( float *pData )
{
	assert(m_pData && pData);
	U32 iNumElements = m_iFloatPackCount * 4;
	Memory::MemCopy(m_pData,pData,iNumElements);

	g_pRenderer->UpdateBuffer(m_pBuffer,pData,true);
}

void ShaderVariable::SetData( U32 iFloatPackAmount, float *pData )
{
	SetPackElementCount(iFloatPackAmount);
	SetData(pData);
}

const ShaderVariable &ShaderVariable::operator=( float *pData )
{
	SetData(pData);
	return *this;
}