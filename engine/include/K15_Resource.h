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

#pragma once

#ifndef __K15_RESOURCE__
#define __K15_RESOURCE__

#include "K15_Object.h"

namespace K15_EngineV2
{
	typedef U32 ResourceID;

	class Resource : public Object
	{
		friend class ResourceManager;
		friend class ResourceHandle;

	public:
		enum ResourcePriority 
		{
			RP_HIGH,	/*!<High priority - stays in the cache for quite some time after it isn't used anymore*/
			RP_NORMAL,	/*!<Normal priority - stays in the cache for a short while after it isn't used anymore*/
			RP_LOW		/*!<Low priority - gets deleted once it isn't used anymore*/
		};

		virtual ~Resource();

		virtual bool Load(byte *pBuffer,U32 iSize,const String &sExtension) = 0;

		inline void SetFilename(const String& sFileName);
		const inline String &GetFilename() const;
		const inline double LastUsed() const;

		const inline U32 GetResourceReferences() const;

		const inline ResourcePriority GetPriority() const;

	protected:
		Resource();
		
	private:
		void inline _IncreseResourceReference();
		void _DecreaseResourceReference();

	private:
		String m_sFilename;
		U32 m_iResourceReferences;
		ResourcePriority m_rpPriority;
		const double m_dCreated;
		double m_dLastUsed;
	};
	#include "../src/K15_Resource.inl"
}

#endif //__K15_RESOURCE__