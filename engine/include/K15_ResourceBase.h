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
#ifndef _K15Engine_Core_ResourceBase_h_
#define _K15Engine_Core_ResourceBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_RawData.h"

namespace K15_Engine { namespace Core { 

	class K15_CORE_API ResourceBase : public Object
	{
	public:
		/*********************************************************************************/
		K15_DECLARE_RTTI;
		/*********************************************************************************/

	public:
		enum eResourcePriority 
		{
			RP_HIGH,	/*!<High priority - stays in the cache for quite some time after it isn't used anymore*/
			RP_NORMAL,	/*!<Normal priority - stays in the cache for a short while after it isn't used anymore*/
			RP_LOW		/*!<Low priority - gets deleted once it isn't used anymore*/
		};

		virtual ~ResourceBase();

		bool load(const RawData& p_Data);

		virtual void loadDebug(RawData& p_Data) = 0;

		INLINE Enum getPriority() const;
		INLINE void setPriority(Enum p_Priority);

		INLINE double getLastUsed() const;
		INLINE double getCreated() const;

		INLINE void increseResourceReference();
		INLINE void decreaseResourceReference();

		INLINE uint32 getResourceReferences() const;

		INLINE bool isMarkedAsUnreferenced() const;
		INLINE void setMarkedAsUnreferenced(bool p_MarkAsUnreferenced);

		INLINE ResourceManager* getResourceManager() const;
		INLINE ResourceFileBase* getResourceFile() const;

		INLINE void setResourceManager(ResourceManager* p_ResourceManager);
		INLINE void setResourceFile(ResourceFileBase* p_ResourceFile);

	protected:
		ResourceBase();
		ResourceBase(const ObjectName& p_AssetName);

		INLINE byte* getRawData();
		INLINE uint32 getRawDataSize();

		virtual bool internalLoad(const RawData&) = 0;

	protected:
		byte* m_RawData;
		uint32 m_RawDataSize;
		bool m_MarkAsUnreferenced;
		ResourceManager* m_ResourceManager;
		ResourceFileBase* m_ResourceFile;
		eResourcePriority m_Priority;
		double m_Created;
		double m_LastUsed;
		uint32 m_ReferenceCount;
	};
	#include "K15_ResourceBase.inl"
}} //end of K15_Engine::Core namespace

#endif //__K15_RESOURCE__