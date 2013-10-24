/**
 * @file K15_PhysicsTask.h
 * @author  Felix Klinge <f.klinge@k15games.de>
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_PhysicsTask.h"
#include "K15_PhysicsProcessBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const uint32 PhysicsTask::TaskPriority = 2;
	/*********************************************************************************/

	/*********************************************************************************/
	PhysicsTask::PhysicsTask()
		: TaskBase(),
		  m_PhysicsProcess(0)
	{

	}
	/*********************************************************************************/
	PhysicsTask::~PhysicsTask()
	{
		K15_DELETE m_PhysicsProcess;
	}
	/*********************************************************************************/
	void PhysicsTask::setPhysicsProcess(PhysicsProcessBase* p_PhysicsProcess)
	{
		m_PhysicsProcess = p_PhysicsProcess;
	}
	/*********************************************************************************/
	PhysicsProcessBase* PhysicsTask::getPhysicsProcess() const
	{
		return m_PhysicsProcess;
	}
	/*********************************************************************************/
	void PhysicsTask::update(const GameTime& p_GameTime)
	{
		if(m_PhysicsProcess)
		{
			m_PhysicsProcess->stepSimulation(p_GameTime);
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace