/**
 * @file K15_ResourceHandle.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/15
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
#ifndef _K15Engine_System_ResourceHandle_h_
#define _K15Engine_System_ResourceHandle_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System { 

	template<class ResourceType>
	class K15_API_EXPORT ResourceHandle
	{
	public:
		ResourceHandle();
		ResourceHandle(ResourceID p_ResourceID);
		ResourceHandle(const ResourceHandle<ResourceType> &rsHandle);
		~ResourceHandle();

		void setResourceID(ResourceID p_ResourceID);
		const ResourceID getResourceID() const;

		const ResourceHandle<ResourceType> &operator=(const ResourceHandle<ResourceType> &rsHandle);

		operator ResourceType*() const;

	private:
		ResourceType* getResource() const;

	private:
		ResourceID m_ResourceID;
	};
#include "K15_ResourceHandle.inl"
}} //end of K15_Engine::System namespace

#endif //_K15Engine_System_ResourceHandle_h_