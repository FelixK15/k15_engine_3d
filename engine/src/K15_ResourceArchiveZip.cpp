/**
 * @file K15_ZipResourceFile.cpp
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

#include "K15_ResourceArchiveZip.h"
#include "K15_RawData.h"

#include "unzip.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceArchiveZip,ResourceArchiveBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceArchiveZip::ResourceArchiveZip(const String& p_ZipFile)
		: ResourceArchiveBase(p_ZipFile),
		m_ZipFile(0)
	{

	}
	/*********************************************************************************/
	ResourceArchiveZip::~ResourceArchiveZip()
	{

	}
	/*********************************************************************************/
	bool ResourceArchiveZip::getResource(const String& p_ResourceName, RawData* p_Data)
	{
		unzLocateFile(m_ZipFile, p_ResourceName.c_str(),1);

		//try to open file in archive
		if(unzOpenCurrentFile(m_ZipFile) != UNZ_OK)
		{
			setError(g_Application->getLastError());
			return false;
		}

		unz_file_info resourceInfo;

		if(unzGetCurrentFileInfo(m_ZipFile,&resourceInfo,(char*)p_ResourceName.c_str(),0,0,0,0,0) != UNZ_OK)
		{
			setError("Could not retrieve file informations.");
			unzCloseCurrentFile(m_ZipFile);
			return false;
		}

		p_Data->data = K15_NEW_SIZE(BaseAllocatedObject::Allocators[BaseAllocatedObject::AC_RESOURCE],resourceInfo.uncompressed_size) byte;
		p_Data->size = resourceInfo.uncompressed_size;

		unzReadCurrentFile(m_ZipFile,p_Data->data,p_Data->size);
		unzCloseCurrentFile(m_ZipFile);
		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveZip::hasResource(const String& p_ResourceName)
	{
		return unzLocateFile(m_ZipFile,p_ResourceName.c_str(),1) == UNZ_OK;
	}
	/*********************************************************************************/
	bool ResourceArchiveZip::_open()
	{
		m_ZipFile = unzOpen(getFileName().c_str());
		if(!m_ZipFile)
		{
			setError(g_Application->getLastError());
			return false;
		}

		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveZip::_close()
	{
		if(unzClose(m_ZipFile) != UNZ_OK)
		{
			setError(g_Application->getLastError());
			return false;
		}

		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace