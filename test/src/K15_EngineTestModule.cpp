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
 */

#include "K15_EngineTestModule.h"

#include "K15_InputManager.h"

namespace K15_Engine { namespace Test {
	/*********************************************************************************/
	TestGameState::TestGameState()
		: GameStateBase("TestGameState")
	{
		g_InputManager->addInputBinding(_ON(LookHorizontal), K15_InputHandler(TestGameState, onMouseMoveX, this));
		g_InputManager->addInputBinding(_ON(LookVertical), K15_InputHandler(TestGameState, onMouseMoveY, this));
	}
	/*********************************************************************************/
	TestGameState::~TestGameState()
	{
		g_InputManager->removeInputBinding(_ON(LookHorizontal));
		g_InputManager->removeInputBinding(_ON(LookVertical));
	}
	/*********************************************************************************/
	void TestGameState::initialize(GameStateBase* p_PrevGameState)
	{

	}
	/*********************************************************************************/
	void TestGameState::shutdown()
	{

	}
	/*********************************************************************************/
	void TestGameState::update(const GameTime& p_GameTime)
	{

	}
	/*********************************************************************************/
	bool TestGameState::onMouseMoveX(InputEvent* p_InputEvent)
	{
		printf("x : %.3f\n" , p_InputEvent->getValue());
		return true;
	}
	/*********************************************************************************/
	bool TestGameState::onMouseMoveY(InputEvent* p_InputEvent)
	{
		printf("y : %.3f\n" , p_InputEvent->getValue());
		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Test namespace
