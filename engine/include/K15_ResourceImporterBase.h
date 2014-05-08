/**
 * @file K15_ResourceImporterBase.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_ResourceLoaderBase_h_
#define _K15Engine_Core_ResourceLoaderBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_Object.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API ResourceImporterBase : public Object
	{
		K15_DECLARE_RTTI;

	public:
		typedef UniqueSet(String) ExtensionSet;
		typedef DynamicArray(RawData) MagicNumberSet;

	protected:
		ResourceImporterBase(const String& p_ResourceTypeName);
		virtual ~ResourceImporterBase();

	public:
		virtual void getExtensionList(ExtensionSet& p_ExtensionSet) = 0;
		virtual void getMagicNumber(MagicNumberSet& p_MagicNumber) = 0;

		void initialize();
		void shutdown();

		bool canLoad(const String& p_FileName);
		bool canLoadBaseOnSignature(const RawData& p_Data);
		ResourceBase* load(const RawData& p_ResourceData, const Rtti& p_ResourceType);

		INLINE const String& getError() const;
		INLINE const String& getResourceTypeName() const;

		INLINE bool isInitialized() const;
	protected:
		INLINE void setError(const String& p_Error);

		virtual ResourceBase* _load(const RawData& p_ResourceData, const Rtti& p_ResourceType) = 0;
		virtual bool _initialize() { return true; }
		virtual bool _shutdown() { return true; }

	private:
		String m_Error;
		String m_ResourceTypeName;
		bool m_Initialized;
	}; //ResourceImporterBase class declaration
#	include "K15_ResourceImporterBase.inl"
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ResourceLoaderBase_h_