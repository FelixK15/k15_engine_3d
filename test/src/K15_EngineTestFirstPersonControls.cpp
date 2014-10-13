/**
 * @file K15_EngineTestFirstPersonControls.cpp
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

#include "K15_EngineTestFirstPersonControls.h"

#include "K15_GameTime.h"
#include "K15_GameObject.h"
#include "K15_NodeComponent.h"

#include "K15_QuaternionUtil.h"

#include "K15_Vector3.h"

#include "K15_MathUtil.h"
#include "K15_InputManager.h"
#include "K15_LogManager.h"

namespace K15_Engine { namespace Test {
	/*********************************************************************************/
	ObjectName const FirstPersonControlComponent::InputMoveLeft			= _ON(MoveLeft);
	ObjectName const FirstPersonControlComponent::InputMoveRight		= _ON(MoveRight);
	ObjectName const FirstPersonControlComponent::InputMoveForward		= _ON(MoveForward);
	ObjectName const FirstPersonControlComponent::InputMoveBackward		= _ON(MoveBackward);

	ObjectName const FirstPersonControlComponent::InputLookHorizontal	= _ON(LookHorizontal);
	ObjectName const FirstPersonControlComponent::InputLookVertical		= _ON(LookVertical);

	ObjectName const FirstPersonControlComponent::InputReset			= _ON(Reset);
	/*********************************************************************************/
	FirstPersonControlComponent::FirstPersonControlComponent()
		: GameObjectComponentBase(_ON(FirstPersonControlComponent)),
		m_Up(Vector3(0.f, 1.f, 0.f)),
		m_Right(Vector3(1.f, 0.f ,0.f)),
		m_Forward(Vector3(0.f, 0.f, -1.f)),
		m_MoveVector(Vector3::Zero),
		m_Speed(30.f),
		m_Yaw(0.f),
		m_Pitch(0.f)
	{
		g_InputManager->addInputBinding(InputMoveLeft, K15_InputHandler(FirstPersonControlComponent, moveLeft, this));
		g_InputManager->addInputBinding(InputMoveRight,	K15_InputHandler(FirstPersonControlComponent, moveRight, this));
		g_InputManager->addInputBinding(InputMoveForward, K15_InputHandler(FirstPersonControlComponent, moveForward, this));
		g_InputManager->addInputBinding(InputMoveBackward, K15_InputHandler(FirstPersonControlComponent, moveBackward, this));

		g_InputManager->addInputBinding(InputLookHorizontal, K15_InputHandler(FirstPersonControlComponent, yaw, this));
		g_InputManager->addInputBinding(InputLookVertical, K15_InputHandler(FirstPersonControlComponent, pitch, this));
	}
	/*********************************************************************************/
	FirstPersonControlComponent::~FirstPersonControlComponent()
	{
		g_InputManager->removeInputBinding(InputMoveLeft);
		g_InputManager->removeInputBinding(InputMoveRight);
		g_InputManager->removeInputBinding(InputMoveForward);
		g_InputManager->removeInputBinding(InputMoveBackward);

		g_InputManager->removeInputBinding(InputLookHorizontal);
		g_InputManager->removeInputBinding(InputLookVertical);
	}
	/*********************************************************************************/
	void FirstPersonControlComponent::update(const GameTime& p_GameTime)
	{
		NodeComponent* node = getGameObject()->getNodeComponent();
		if(node && m_RotationDirty)
		{	
			node->yaw(m_Yaw);
			node->pitch(m_Pitch);
			m_Yaw = m_Pitch = 0.f;
		}
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::moveForward(InputEvent* p_Arg)
	{
		getGameObject()->translate(0.f, 0.f, -p_Arg->getValue());
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::moveBackward(InputEvent* p_Arg)
	{
		getGameObject()->translate(0.f, 0.f, p_Arg->getValue());
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::moveLeft(InputEvent* p_Arg)
	{
		getGameObject()->translate(-p_Arg->getValue(), 0.f, 0.f);
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::moveRight(InputEvent* p_Arg)
	{
		getGameObject()->translate(p_Arg->getValue(), 0.f, 0.f);
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::yaw(InputEvent* p_Arg)
	{
		m_Yaw = p_Arg->getValue();
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
	bool FirstPersonControlComponent::pitch(InputEvent* p_Arg)
	{
		m_Pitch = p_Arg->getValue();
		m_RotationDirty = true;
		return true;
	}
	/*********************************************************************************/
}} // end of K15_Engine::Test namespace