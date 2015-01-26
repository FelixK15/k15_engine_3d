/**
 * @file K15_ResourceArchiveFileSystem.cpp
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

#include "K15_ResourceArchiveFileSystem.h"
#include "K15_RawData.h"
#include "K15_IOUtil.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core, ResourceArchiveFileSystem, ResourceArchiveBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceArchiveFileSystem::ResourceArchiveFileSystem(const String& p_AbsoluteRootDirectory)
	: ResourceArchiveBase(p_AbsoluteRootDirectory),
	m_RootDirectory(IOUtil::convertToUnixFilePath(p_AbsoluteRootDirectory))
	{
		if(m_RootDirectory.back() != '/')
		{
			m_RootDirectory += '/';
		}

		K15_ASSERT(IOUtil::folderExists(m_RootDirectory), 
				   StringUtil::format("Directory \"%s\" does not exists.", m_RootDirectory.c_str()));
	}
	/*********************************************************************************/
	ResourceArchiveFileSystem::~ResourceArchiveFileSystem()
	{

	}
	/*********************************************************************************/
	bool ResourceArchiveFileSystem::getResource(const String& p_ResourceName, RawData* p_Data)
	{
		String resourcePath = m_RootDirectory + (p_ResourceName.front() == '/' ? p_ResourceName.substr(1) : p_ResourceName);
		FILE* resourceFile = fopen(resourcePath.c_str(), "rb");

		if(!resourceFile)
		{
			setError(strerror(errno));
			return false;
		}

		uint32 fileSize = IOUtil::getFileSize(resourcePath);
		p_Data->data = K15_NEW_SIZE(BaseAllocatedObject::Allocators[BaseAllocatedObject::AC_RESOURCE], fileSize) byte;
		p_Data->size = fileSize;

		uint32 bytesRead = fread(p_Data->data, 1, fileSize, resourceFile);

		fclose(resourceFile);

		return bytesRead == fileSize;
	}
	/*********************************************************************************/
	bool ResourceArchiveFileSystem::hasResource(const String& p_ResourceName)
	{
		String resourcePath = m_RootDirectory + (p_ResourceName.front() == '/' ? p_ResourceName.substr(1) : p_ResourceName);
		FILE* resourceFile = fopen(resourcePath.c_str(), "rb");

		if(!resourceFile)
		{
			return false;
		}

		fclose(resourceFile);

		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveFileSystem::_open()
	{
		return true;
	}
	/*********************************************************************************/
	bool ResourceArchiveFileSystem::_close()
	{
		return true;
	}
/*********************************************************************************/
}}// end of K15_Engine::Core namespace