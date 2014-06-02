/**
 * @file TD_CharacterStatsComponent.inl
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
 */

/*********************************************************************************/
int CharacterStatsComponent::getHealth() const
{
	return m_Health;
}
/*********************************************************************************/
int CharacterStatsComponent::getGold() const
{
	return m_Gold;
}
/*********************************************************************************/
int CharacterStatsComponent::getMaxHealth() const
{
	return m_MaxHealth;
}
/*********************************************************************************/
void CharacterStatsComponent::decreaseHealth(int p_Health)
{
	m_Health -= p_Health;
}
/*********************************************************************************/
void CharacterStatsComponent::setHealth(int p_Health)
{
	m_Health = p_Health;
}
/*********************************************************************************/
void CharacterStatsComponent::setGold(int p_Gold)
{
	m_Gold = p_Gold;
}
/*********************************************************************************/
void CharacterStatsComponent::resetHealth()
{
	m_Health = m_MaxHealth;
}
/*********************************************************************************/
