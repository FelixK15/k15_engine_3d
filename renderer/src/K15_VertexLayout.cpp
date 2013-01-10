/**
 * @file K15_VertexLayout.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/24
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
#include "K15_Renderer.h"

#include "K15_VertexLayout.h"

using namespace K15_Renderer;

IMPLEMENT_RTTI("K15_Renderer",VertexLayout,Bindable);

const U32 SemanticSize[] = {
	1,
	1,
	2,
	2,
	4,
	4,
	4,
	4,
	4,
	8,
	8,
	8,
	12,
	12,
	12,
	16,
	16,
	16,
};

VertexLayout::VertexLayout(U8 iNumAttributes,va_list pArguments)
	: m_iAmountAttributes(iNumAttributes)
{
	U8 arrSlotCount[VertexLayout::VS_SEMANTICCOUNT] = {0};
	m_pLayout = K15_NEW SingleLayout[iNumAttributes];

	for(U8 i = 0;i < iNumAttributes;++i){
		int iFormat = va_arg(pArguments,int);
		int iSemanticType = va_arg(pArguments,int);
		FormatType ftType = (FormatType)iFormat;
		VertexSemantic stType = (VertexSemantic)iSemanticType;

		m_pLayout[i].Semantic = stType;
		m_pLayout[i].Format = ftType;
		m_pLayout[i].Size = SemanticSize[iFormat];
		m_pLayout[i].Slot = arrSlotCount[iSemanticType]++;

		m_iVertexSize += SemanticSize[iFormat];
	}
}

VertexLayout::~VertexLayout()
{
	K15_DELETE[] m_pLayout;
}