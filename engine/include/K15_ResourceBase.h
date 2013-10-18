/**
 * @file K15_Resource.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#ifndef _K15Engine_System_ResourceBase_h_
#define _K15Engine_System_ResourceBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceData.h"

namespace K15_Engine { namespace System { 

	class K15_API_EXPORT ResourceBase
	{
	public:
		static TypeName Type;

		enum eResourcePriority 
		{
			RP_HIGH,	/*!<High priority - stays in the cache for quite some time after it isn't used anymore*/
			RP_NORMAL,	/*!<Normal priority - stays in the cache for a short while after it isn't used anymore*/
			RP_LOW		/*!<Low priority - gets deleted once it isn't used anymore*/
		};

		virtual ~ResourceBase();

		bool load(const ResourceData& p_Data);

		virtual void loadDebug(ResourceData& p_Data) = 0;

		const TypeName& getResourceType() const;

		inline const ResourceName& getAssetName() const;

		inline Enum getPriority() const;
		inline void setPriority(Enum p_Priority);

		inline double getLastUsed() const;
		inline double getCreated() const;

		inline void increseResourceReference();
		inline void decreaseResourceReference();

		inline uint32 getResourceReferences() const;

		inline bool isMarkedAsUnreferenced() const;
		inline void setMarkedAsUnreferenced(bool p_MarkAsUnreferenced);

		inline ResourceManager* getResourceManager() const;
		inline ResourceFileBase* getResourceFile() const;

		inline void setResourceManager(ResourceManager* p_ResourceManager);
		inline void setResourceFile(ResourceFileBase* p_ResourceFile);
	public:
		static const TypeName& getType();
		static void loadDebugResource(ResourceData& p_ResourceData);

	protected:
		ResourceBase();
		ResourceBase(const ResourceName& p_AssetName);

		inline byte* getRawData();
		inline uint32 getRawDataSize();

		virtual bool internalLoad(const ResourceData&) = 0;

	protected:
		byte* m_RawData;
		uint32 m_RawDataSize;
		bool m_MarkAsUnreferenced;
		ResourceManager* m_ResourceManager;
		ResourceFileBase* m_ResourceFile;
		ResourceName m_AssetName;
		eResourcePriority m_Priority;
		double m_Created;
		double m_LastUsed;
		uint32 m_ReferenceCount;
	};
	#include "K15_ResourceBase.inl"
}} //end of K15_Engine::System namespace

#endif //__K15_RESOURCE__