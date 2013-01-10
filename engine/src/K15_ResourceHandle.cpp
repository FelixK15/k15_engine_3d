/**
 * @file K15_ResourceHandle.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/15
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
#include "K15_ResourceHandle.h"
#include "K15_Resource.h"

using namespace K15_EngineV2;

ResourceHandle::ResourceHandle(Resource *pResource)
	: m_pResource(pResource)
{
	assert(pResource);
	pResource->_IncreseResourceReference();
}

ResourceHandle::ResourceHandle( const ResourceHandle &rsHandle )
	: m_pResource(rsHandle.m_pResource)
{
	assert(m_pResource);
	m_pResource->_IncreseResourceReference();
}

ResourceHandle::~ResourceHandle()
{
	m_pResource->_DecreaseResourceReference();
}

const ResourceHandle &ResourceHandle::operator=(const ResourceHandle &rsHandle)
{
	if(m_pResource){
		m_pResource->_DecreaseResourceReference();
	}

	m_pResource = rsHandle.m_pResource;

	m_pResource->_IncreseResourceReference();
	
	return *this;
}

ResourceHandle::operator Resource*() const
{
	return m_pResource;
}