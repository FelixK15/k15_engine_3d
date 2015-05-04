/**
 * @file K15_Object.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
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

#include "K15_Object.h"

namespace K15_Engine { namespace Core { 
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI(Core,Object);
	/*********************************************************************************/

	/*********************************************************************************/
	Object::Object()
		: m_Name(ObjectName::BLANK)
	{

	}
	/*********************************************************************************/
	Object::Object(const ObjectName& p_Name)
		: m_Name(p_Name)
	{

	}
	/*********************************************************************************/
	Object::~Object()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace