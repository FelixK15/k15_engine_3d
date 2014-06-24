/**
 * @file K15_NetworkManager.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/08
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

#ifndef _K15Engine_Core_NetworkManager_h_
#define _K15Engine_Core_NetworkManager_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API NetworkManager : public CoreAllocatedObject,
										public Singleton<NetworkManager>
	{
	public:
		NetworkManager();
		~NetworkManager();

		bool init();
		void shutdown();

	private:
		bool m_Initialized;
	};// end of NetworkManager class declaration
}}// end of K15_Engine::Core namespace

#endif //