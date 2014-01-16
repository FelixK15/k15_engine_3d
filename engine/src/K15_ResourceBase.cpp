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

#include "K15_PrecompiledHeader.h"

#include "K15_ResourceBase.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceBase,Object);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceBase::ResourceBase()
		: Object(),
		  m_Created(0.0),
		  m_LastUsed(0.0),
		  m_ReferenceCount(0),
		  m_RawData(0),
		  m_RawDataSize(0),
		  m_ResourceManager(0),
		  m_ResourceFile(0),
		  m_MarkAsUnreferenced(false)

	{

	}
	/*********************************************************************************/
	ResourceBase::ResourceBase(const ObjectName& p_AssetName)
		: Object(p_AssetName),
		  m_Created(0.0),
		  m_LastUsed(0.0),
		  m_ReferenceCount(0),
		  m_RawData(0),
		  m_RawDataSize(0),
		  m_ResourceManager(0),
		  m_ResourceFile(0),
		  m_MarkAsUnreferenced(false)

	{

	}
	/*********************************************************************************/
	ResourceBase::~ResourceBase()
	{

	}
	/*********************************************************************************/
	bool ResourceBase::load(const RawData& p_Data)
	{
		m_RawData = p_Data.data;
		m_RawDataSize = p_Data.size;

		return internalLoad(p_Data);
	}
	/*********************************************************************************/
}} //end of K15_Engine::Core namespace