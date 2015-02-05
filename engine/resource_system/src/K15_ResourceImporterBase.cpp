/**
 * @file K15_ResourceImporterBase.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/14
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

#include "K15_ResourceImporterBase.h"
#include "K15_IOUtil.h"
#include "K15_RawData.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core, ResourceImporterBase, Object);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceImporterBase::ResourceImporterBase(const String& p_ResourceTypeName)
		: Object(),
		m_Error(),
		m_ResourceTypeName(p_ResourceTypeName),
		m_Initialized(false)
	{

	}
	/*********************************************************************************/
	ResourceImporterBase::~ResourceImporterBase()
	{

	}
	/*********************************************************************************/
	bool ResourceImporterBase::canLoad(const String& p_FileName)
	{
		ExtensionSet extensions;
		getExtensionList(extensions);

		String::size_type posSeparator = p_FileName.find_last_of('.');
		String extension = p_FileName.substr(posSeparator);

		for(ExtensionSet::iterator iter = extensions.begin();iter != extensions.end();++iter)
		{
			if(extension == (*iter))
			{
				return true;
			}
		}
	
		return false;
	}
	/*********************************************************************************/
	bool ResourceImporterBase::canLoadBaseOnSignature(const RawData& p_Data)
	{
		bool match = false;
		byte* signatureBuffer = (byte*)alloca(512);
		//get set of magic numbers that identifies the signature of the resource
		MagicNumberSet magicnumbers;
		getMagicNumber(magicnumbers);

		for(MagicNumberSet::iterator iter = magicnumbers.begin();iter != magicnumbers.end();++iter)
		{
			//move stream pos to begin of the file.
			if(iter->size == 0 || !iter->data)
			{
				K15_LOG_ERROR_MESSAGE("Resource<%s> - Invalid magic number.",
					m_ResourceTypeName.c_str());
				continue;
			}

			if(iter->size > p_Data.size)
			{
				K15_LOG_ERROR_MESSAGE("Resource<%s> - Magic number greater than file content.",
					m_ResourceTypeName.c_str());
				continue;
			}

			//read signature
			memcpy(signatureBuffer,p_Data.data,iter->size);

			//compare signature from file and magic number (byte by byte)
			for(uint32 i = 0;i < iter->size;++i)
			{
				if(signatureBuffer[i] != iter->data[i])
				{
					// no match - try next
					break;
				}

				//match found - exit loop.
				match = true;
				break;
			}
		}

		return match;
	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterBase::load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		ResourceBase* resource = 0;

		if((resource = _load(p_ResourceData,p_ResourceType)) != 0)
		{
			return resource;
		}

		setError(StringUtil::format("Resource<%s> - resource importer could not import resource. Error:\"%s\".",
			m_ResourceTypeName.c_str(),getError().c_str()));

		return 0;
	}
	/*********************************************************************************/
	void ResourceImporterBase::initialize()
	{
		if(m_Initialized)
		{
			K15_LOG_ERROR_MESSAGE("ResourceImporter \"%s\" has already been initialized.",getName().c_str());
		}
		else
		{
			m_Initialized = _initialize();

			if(!m_Initialized)
			{
				K15_LOG_ERROR_MESSAGE("Could not initialize ResourceImporter \"%s\". Error:\"%s\".",
					getTypeName().c_str(),getError().c_str());
			}
		}
	}
	/*********************************************************************************/
	void ResourceImporterBase::shutdown()
	{
		m_Initialized = !_shutdown();

		if(m_Initialized)
		{
			K15_LOG_ERROR_MESSAGE("Error shutting down ResourceImporter \"%s\". Error:\"%s\".",
				getTypeName().c_str(),getError().c_str());
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace