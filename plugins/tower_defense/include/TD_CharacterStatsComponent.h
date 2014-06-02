/**
 * @file TD_CharacterStatsComponent.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/28
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

#ifndef _TD_CharacterStatsComponent_h_
#define _TD_CharacterStatsComponent_h_

#include "TD_Prerequisities.h"

#include "K15_GameObjectComponentBase.h"

namespace TowerDefense {
	class CharacterStatsComponent : public GameObjectComponentBase
	{
		K15_DECLARE_RTTI;

	public:
		CharacterStatsComponent(int m_MaxHealth, int m_Gold);
		~CharacterStatsComponent();

		INLINE int getHealth() const;
		INLINE int getGold() const;
		INLINE int getMaxHealth() const;

		INLINE void decreaseHealth(int p_Health);
		INLINE void setHealth(int p_Health);
		INLINE void setGold(int p_Gold);

		INLINE void resetHealth();

	private:
		int m_MaxHealth;
		int m_Health;
		int m_Gold;
	}; // CharacterStatsComponent class declaration
#	include "TD_CharacterStatsComponent.inl"
} // end of TowerDefense namespace

#endif //_TD_CharacterStatsComponent_h_