/**
 * @file K15_AlphaState.inl
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
bool AlphaState::getEnabled() const
{
  return m_Enabled;
}
/*********************************************************************************/
Enum AlphaState::getDestinationBlendFunction() const
{
	return m_DestinationBlendFunction;
}
/*********************************************************************************/
Enum AlphaState::getSourceBlendFunction() const
{
	return m_SourceBlendFunction;
}
/*********************************************************************************/
Enum AlphaState::getBlendOperation() const
{
	return m_BlendOperation;
}
/*********************************************************************************/
const ColorRGBA& AlphaState::getConstantColor() const
{
	return m_ConstantColor;
}
/*********************************************************************************/
void AlphaState::setEnabled(bool p_Enabled)
{
  m_Enabled = p_Enabled;
}
/*********************************************************************************/
void AlphaState::setDestinationBlendFunction(Enum p_DestinationBlendFunction)
{
	K15_ASSERT(p_DestinationBlendFunction < AlphaState::BF_COUNT,"Invalid blend function.");
	m_DestinationBlendFunction = p_DestinationBlendFunction;
}
/*********************************************************************************/
void AlphaState::setSourceBlendFunction(Enum p_SourceBlendFunction)
{
	K15_ASSERT(p_SourceBlendFunction < AlphaState::BF_COUNT,"Invalid blend function.");
	m_SourceBlendFunction = p_SourceBlendFunction;
}
/*********************************************************************************/
void AlphaState::setBlendOperation(Enum p_BlendEquation)
{
	K15_ASSERT(m_BlendOperation < AlphaState::BO_COUNT,"Invalid blend equation.");
	m_BlendOperation = m_BlendOperation;
}
/*********************************************************************************/
void AlphaState::setConstantColor(const ColorRGBA& p_ConstantColor)
{
	m_ConstantColor = p_ConstantColor;
}
/*********************************************************************************/