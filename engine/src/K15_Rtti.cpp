/**
 * @file K15_Rtti.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/07
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

#include "K15_Rtti.h"

using namespace K15_EngineV2;

Rtti::Rtti(const String &sName,const Rtti *pBaseType)
{
	m_sName = sName;
	m_pBaseType = pBaseType;
}

Rtti::~Rtti()
{

}

bool Rtti::IsDerivedFrom(const Rtti &rtType) const
{
	bool bDerived = false;

	const Rtti *pSearch = this;
	while(pSearch){
		if(pSearch == &rtType){
			bDerived = true;
			break;
		}

		pSearch = pSearch->m_pBaseType;
	}
	
	return bDerived;
}