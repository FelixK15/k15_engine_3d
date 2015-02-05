/**
 * @file K15_NetworkManager.cpp
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_NetworkManager.h"

#include "enet/enet.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	NetworkManager::NetworkManager()
		: m_Initialized(false)
	{

	}
	/*********************************************************************************/
	NetworkManager::~NetworkManager()
	{

	}
	/*********************************************************************************/
	bool NetworkManager::init()
	{
		if(enet_initialize() == 0)
		{
			m_Initialized = true;
			return true;
		}

		K15_LOG_ERROR_MESSAGE("Could not initialize enet network library.");
		return false;
	}
	/*********************************************************************************/
	void NetworkManager::shutdown()
	{
		if(m_Initialized)
		{
			enet_deinitialize();
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace