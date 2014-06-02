/**
 * @file TD_GameState.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/19
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

#ifndef _TD_MainGameState_h_
#define _TD_MainGameState_h_

#include "TD_Prerequisities.h"
#include "K15_Mouse.h"
#include "K15_GameStateBase.h"
#include "K15_AllocatedObject.h"

namespace TowerDefense 
{
	class MainGameState : public GameStateBase, public GeneralAllocatedObject
	{
	public:
		static const int AMOUNT_ENEMIES = 10;
		typedef FixedArray(GameObject*,AMOUNT_ENEMIES) GameObjectFixedArray;

	public:
		MainGameState();
		~MainGameState();
		
		virtual void initialize(GameStateBase* p_PrevGameState) OVERRIDE;
		virtual void shutdown() OVERRIDE;
		virtual void update(const GameTime& p_GameTime) OVERRIDE;

		bool onEventMousePressed(GameEvent* p_Event);

	private:
		void _checkCollision(GameObject* p_Enemy);
		bool _isInGoal(GameObject* p_Enemy);

	private:
		GameObjectFixedArray m_Enemies;
		GameObject* m_Camera;
		Level* m_Level;
		float m_SpawnRateTimer;
		float m_SpawnRate;
		int m_WaveCounter;
	};// end of MainGameState class declaration
}// end of TowerDefense namespace

#endif //_TD_MainGameState_h_