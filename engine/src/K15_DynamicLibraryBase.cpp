/**
 * @file K15_DynamicLibrary.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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

#include "K15_DynamicLibraryBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	DynamicLibraryBase::DynamicLibraryBase()
		: m_FileName(),
		  m_Loaded(false)
	{

	}
	/*********************************************************************************/
	DynamicLibraryBase::DynamicLibraryBase(const String& p_FileName)
		: m_FileName(p_FileName),
		  m_Loaded(false)
	{
		m_Loaded = load();
	}
	/*********************************************************************************/
	DynamicLibraryBase::~DynamicLibraryBase()
	{

	}
	/*********************************************************************************/
	const String& DynamicLibraryBase::getFileName() const
	{
		return m_FileName;
	}
	/*********************************************************************************/
	void DynamicLibraryBase::setFileName(const String& p_FileName)
	{
		m_FileName = p_FileName;
	}
	/*********************************************************************************/
	bool DynamicLibraryBase::isLoaded() const
	{
		return m_Loaded;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace