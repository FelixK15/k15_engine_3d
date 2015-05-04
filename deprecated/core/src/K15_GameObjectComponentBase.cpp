/**
 * @file K15_GameObjectComponentBase.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
#include "K15_GameObjectComponentBase.h"
#include "K15_AABB.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,GameObjectComponentBase,Core::Object);
	/*********************************************************************************/

	/*********************************************************************************/
	GameObjectComponentBase::GameObjectComponentBase( const ObjectName& p_Name)
		: Object(p_Name),
		m_GameObject(0)
	{

	}
	/*********************************************************************************/
	GameObjectComponentBase::~GameObjectComponentBase()
	{

	}
	/*********************************************************************************/
	const AABB& GameObjectComponentBase::getAABB()
	{
		static AABB aabb;

		if(_AABBBNeedsUpdate())
		{
			aabb = _calculateAABB();
		}

		return aabb;
	}
	/*********************************************************************************/
	AABB GameObjectComponentBase::_calculateAABB()
	{
		return AABB();	
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace