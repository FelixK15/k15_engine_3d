/**
 * @file K15_MaterialData.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/15
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

#include "K15_MaterialData.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Rendering,MaterialData,Object);
	/*********************************************************************************/

	/*********************************************************************************/
	MaterialData::MaterialData()
		: Object(),
		m_NameValueMap()
	{

	}
	/*********************************************************************************/
	MaterialData::MaterialData(const ObjectName& p_Name)
		: Object(p_Name),
		m_NameValueMap()
	{

	}
	/*********************************************************************************/
	MaterialData::~MaterialData()
	{

	}
	/*********************************************************************************/
	const String& MaterialData::getValue(const String& p_Name) const
	{
		NameValueMap::const_iterator iter = m_NameValueMap.find(p_Name);
		
		K15_ASSERT(iter != m_NameValueMap.end(),StringUtil::format("MaterialData %s has no variable with the name %s.",
			m_Name.c_str(),p_Name.c_str()));
		
		return iter->second;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace