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
 */

/*********************************************************************************/
GameStateBase* GameStateManager::getCurrentGameState() const
{
	if(m_CurrentGameStateIndex < 0) return 0;

	return m_GameStates[m_CurrentGameStateIndex];
}
/*********************************************************************************/
void GameStateManager::update(const GameTime& p_GameTime)
{
	update_R(p_GameTime, m_CurrentGameStateIndex - 1);
}
/*********************************************************************************/
