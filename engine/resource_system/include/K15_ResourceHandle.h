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
#ifndef _K15Engine_Core_ResourceHandle_h_
#define _K15Engine_Core_ResourceHandle_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_ResourceManager.h"

namespace K15_Engine { namespace Core { 
  /*********************************************************************************/
  typedef int ResourceID;
  /*********************************************************************************/
	template<class ResourceType>
	class ResourceHandle
	{
	public:
		ResourceHandle();
		ResourceHandle(ResourceID p_ResourceID);
		ResourceHandle(const ResourceHandle<ResourceType> &rsHandle);
		~ResourceHandle();

		const String& getResourceName() const;
		void setResourceName(const String& p_ResourceName);

		bool isValid() const;

		void setResourceID(ResourceID p_ResourceID);
		const ResourceID getResourceID() const;

		const ResourceHandle<ResourceType> &operator=(const ResourceHandle<ResourceType> &rsHandle);

		ResourceType* operator->();

	//private:
		ResourceType* getResource();

	private:
		ResourceID m_ResourceID;
		String m_ResourceName;
	};// end of ResourceHandle class
  /*********************************************************************************/
  #include "K15_ResourceHandle.inl"
  /*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_ResourceHandle_h_