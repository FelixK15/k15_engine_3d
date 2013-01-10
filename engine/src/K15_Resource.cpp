/**
 * @file K15_Resource.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_Resource.h"
using namespace K15_EngineV2;

Resource::Resource()
	: m_dCreated(g_pSystem->TimeSinceStart())
{

}

Resource::~Resource()
{

}

void Resource::_DecreaseResourceReference()
{
	assert((m_iResourceReferences - 1) != 0);
	--m_iResourceReferences;

	if(m_iResourceReferences == 0){
		m_dLastUsed = g_pSystem->TimeSinceStart();
	}
}
