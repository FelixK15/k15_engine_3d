/**
 * @file TD_MoveComponent.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _TD_MoveComponent_h_
#define _TD_MoveComponent_h_

#include "TD_Prerequisities.h"

#include "K15_GameObjectComponentBase.h"

namespace TowerDefense {
	class MoveComponent : public GameObjectComponentBase
	{
	public:
		MoveComponent(const Vector3& p_Speed);
		~MoveComponent();

		virtual void update(const GameTime& p_GameTime);

	private:
		Vector3 m_Speed;
	}; //MoveComponent class declaration
} // end of TowerDefense namespace

#endif //_TD_MoveComponent_h_