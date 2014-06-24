/**
 * @file K15_GameState.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/03
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

#ifndef _K15Engine_Core_GameState_h_
#define _K15Engine_Core_GameState_h_

#include "K15_Prerequisites.h"
#include "K15_Object.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API GameStateBase : public Object
	{
	public:
		virtual void initialize(GameStateBase* p_PrevGameState) = 0;
		virtual void shutdown() = 0;
		virtual void update(const GameTime& p_GameTime) = 0;
	protected:
		GameStateBase(const ObjectName& p_Name);
		virtual ~GameStateBase();

	};// end of GameStateBase class declaration
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_GameState_h_