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
		setName(_ON(MemoryProfilingTask));

	}
	/*********************************************************************************/
	MemoryProfilingTask::~MemoryProfilingTask()
	{

	}
	/*********************************************************************************/
	void MemoryProfilingTask::update(const GameTime& p_GameTime)
	{
	
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace