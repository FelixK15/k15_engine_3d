/**
 * @file K15_DepthState.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
bool DepthState::getEnabled() const
{
  return m_Enabled;
}
/*********************************************************************************/
float DepthState::getBias() const
{
  return m_Bias;
}
/*********************************************************************************/
Enum DepthState::getFunction() const
{
  return m_Function;
}
/*********************************************************************************/
void DepthState::setEnabled(bool p_Enabled)
{
  m_Enabled = p_Enabled;
}
/*********************************************************************************/
void DepthState::setBias (float p_Bias)
{
  m_Bias = p_Bias;
}
/*********************************************************************************/