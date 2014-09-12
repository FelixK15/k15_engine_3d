/**
 * @file K15_GameStateManager.h
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
 *
 * @section DESCRIPTION
 *
 */

#ifndef _K15Engine_Core_GameStateManager_h_
#define _K15Engine_Core_GameStateManager_h_

#include "K15_Prerequisites.h"
#include "K15_Singleton.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API GameStateManager : public CoreAllocatedObject,
							 public Singleton<GameStateManager>
	{
	public:
		static uint32 const MAX_GAMESTATES = 32;
		typedef FixedArray(GameStateBase*, MAX_GAMESTATES) GameStateArray;

	public:
		GameStateManager();
		virtual ~GameStateManager();

		void pushGameState(GameStateBase* p_GameState);
		void popCurrentGameState();

		INLINE void update(const GameTime& p_GameTime);

		INLINE GameStateBase* getCurrentGameState() const;

	private:
		void update_R(const GameTime& p_GameTime, int32 p_Index);

	private:
		GameStateArray m_GameStates;
		int32 m_CurrentGameStateIndex;
	}; // end of GameStateManager class declaration
	#include "K15_GameStateManager.inl"
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_GameStateManager_h_