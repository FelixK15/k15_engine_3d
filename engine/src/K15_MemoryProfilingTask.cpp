/**
 * @file K15_MemoryProfilingTask.cpp
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
#include "K15_MemoryProfilingTask.h"

#include "enet/enet.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI(K15_Engine::Core,MemoryProfilingTask)
	/*********************************************************************************/

	/*********************************************************************************/
	ENetAddress g_Address;
	ENetHost* g_Server;
	/*********************************************************************************/

	/*********************************************************************************/
	MemoryProfilingTask::MemoryProfilingTask()
		: TaskBase()
	{
		setName(_N(MemoryProfilingTask));

		g_Address.host = ENET_HOST_ANY;
		g_Address.port = 1212;

		g_Server = enet_host_create(&g_Address,10,2,0,0);

		if(!g_Server)
		{
			_LogError("Could not initialize enet server.");
		}
	}
	/*********************************************************************************/
	MemoryProfilingTask::~MemoryProfilingTask()
	{
		if(g_Server)
		{
			enet_host_destroy(g_Server);
			g_Server = 0;
		}
	}
	/*********************************************************************************/
	void MemoryProfilingTask::update(const GameTime& p_GameTime)
	{
		//todo
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace