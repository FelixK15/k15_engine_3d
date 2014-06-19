/**
 * @file K15_ResourceFileBase.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/15
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

#include "K15_ResourceArchiveBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceArchiveBase,Object);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceArchiveBase::ResourceArchiveBase(const String& p_FileName)
		: Object(p_FileName),
		m_FileName(p_FileName),
		m_isOpen(false)
	{

	}
	/*********************************************************************************/
	ResourceArchiveBase::~ResourceArchiveBase()
	{

	}
	/*********************************************************************************/
	bool ResourceArchiveBase::open()
	{
		if(!isOpen())
		{
			m_isOpen = _open();
		}

		K15_ASSERT(isOpen(),StringUtil::format("Could not open ResourceFile<%s> \"%s\". Error:\"%s\".",
			getName().c_str(),getFileName().c_str(),getError().c_str()));

		return m_isOpen;
	}
	/*********************************************************************************/
	bool ResourceArchiveBase::close()
	{
		K15_ASSERT(isOpen(),StringUtil::format("Trying to close ResourceFile<%s> although it has never been opened.",
			getName().c_str(),getFileName().c_str()));

		m_isOpen = !_close();

		K15_ASSERT(!isOpen(),StringUtil::format("Could not close ResourceFile<%s> \"%s\". Error:\"%s\".",
			getName().c_str(),getFileName().c_str(),getError().c_str()));

		return !m_isOpen;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace