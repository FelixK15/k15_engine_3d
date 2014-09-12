/**
 * @file K15_GameStateManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_GameStateManager.h"
#include "K15_GameStateBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	GameStateManager::GameStateManager()
		: Singleton(),
		m_CurrentGameStateIndex(-1),
		m_GameStates()
	{

	}
	/*********************************************************************************/
	GameStateManager::~GameStateManager()
	{

	}
	/*********************************************************************************/
	void GameStateManager::pushGameState(GameStateBase* p_GameState)
	{
		K15_ASSERT(p_GameState, "GameState is NULL");

		GameStateBase* currentGameState = getCurrentGameState();

		p_GameState->initialize(currentGameState);
		
		m_GameStates[++m_CurrentGameStateIndex] = p_GameState;
	}
	/*********************************************************************************/
	void GameStateManager::popCurrentGameState()
	{
		K15_ASSERT(m_CurrentGameStateIndex >= 0, "No GameState to pop.");

		GameStateBase* currentGameState = getCurrentGameState();

		// delete current game state
		currentGameState->shutdown();
		K15_DELETE currentGameState;

		m_CurrentGameStateIndex -= 1;
	}
	/*********************************************************************************/
	void GameStateManager::update_R(const GameTime& p_GameTime, int32 p_Index)
	{
		if(p_Index < 0 || p_Index > m_CurrentGameStateIndex) 
		{
			return;
		}

		GameStateBase* gameState = m_GameStates[p_Index];

		gameState->update(p_GameTime);

		if(gameState->getUpdateNextGameState())
		{
			update_R(p_GameTime, p_Index - 1);
		}
	}
	/*********************************************************************************/
}} // end of K15_Engine::Core namespace