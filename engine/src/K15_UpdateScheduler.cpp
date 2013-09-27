/**
 * @file K15_UpdateScheduler.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/08/13
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

#include "K15_UpdateScheduler.h"

namespace K15_EngineV2 { namespace System {
	UpdateScheduler::UpdateScheduler()
		: m_arrUpdateFunctions(),
		  m_arrUnregisteredIDs()
	{

	}

	UpdateScheduler::~UpdateScheduler()
	{
		DeleteAllUpdateFunctions();
	}

	UpdateSchedulerID UpdateScheduler::RegisterUpdateFunction(UpdateFunctor* pFunction)
	{
		UpdateSchedulerID iID = m_arrUpdateFunctions.Size();
		m_arrUpdateFunctions.PushBack(pFunction);

		return iID;
	}

	void UpdateScheduler::UnregisterUpdateFunction(UpdateSchedulerID iID)
	{
		m_arrUnregisteredIDs.PushBack(iID);
	}

	void UpdateScheduler::DeleteAllUpdateFunctions()
	{
		UpdateFunctor* pCurrentFunctor = NULL;
		for(int i = 0;i < m_arrUpdateFunctions.Size();++i){
			pCurrentFunctor = m_arrUpdateFunctions.Get(i);
			delete pCurrentFunctor;
		}

		m_arrUpdateFunctions.Clear();
		m_arrUnregisteredIDs.Clear();
	}

	void UpdateScheduler::Update(const GameTime& gtDelta)
	{
		UpdateFunctor* pCurrentFunctor = NULL;
		for(int i = 0;i < m_arrUpdateFunctions.Size();++i){
			if(!m_arrUnregisteredIDs.HasItem(i)){
				pCurrentFunctor = m_arrUpdateFunctions.Get(i);
				(*pCurrentFunctor)(gtDelta);
			}
		}
	}
}}