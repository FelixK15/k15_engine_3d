/**
 * @file TD_MoveComponent.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/27
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

#include "TD_MoveComponent.h"

#include "K15_GameObject.h"
#include "K15_GameTime.h"

namespace TowerDefense {
	/*********************************************************************************/
	MoveComponent::MoveComponent(const Vector3& p_Speed)
		: GameObjectComponentBase(_TN(MoveComponent)),
		m_Speed(p_Speed)
	{

	}
	/*********************************************************************************/
	MoveComponent::~MoveComponent()
	{

	}
	/*********************************************************************************/
	void MoveComponent::update(const GameTime& p_GameTime)
	{
		Node& node = getGameObject()->getNode();
		Vector3 velocity = m_Speed * (float)p_GameTime.getDeltaTime();
		node.translate(velocity);
	}
	/*********************************************************************************/
}// end of TowerDefense namespace