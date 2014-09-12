/**
 * @file K15_EngineTestModule.h
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

#ifndef _K15Engine_TestApplication_TestModule_h_
#define _K15Engine_TestApplication_TestModule_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_GameStateBase.h"

#include "K15_InputManager.h"

namespace K15_Engine { namespace Test {
	class TestGameState : public GameStateBase,
						  public GeneralAllocatedObject
	{
	public:
		TestGameState();
		virtual ~TestGameState();

		virtual void initialize(GameStateBase* p_PrevGameState);
		virtual void shutdown();
		virtual void update(const GameTime& p_GameTime);

		bool onMouseMoveX(InputEvent* p_InputEvent);
		bool onMouseMoveY(InputEvent* p_InputEvent);
	}; // end of TestModule class declaration
}} // end of K15_Engine::Test namespace

#endif //_K15Engine_TestApplication_TestModule_h_