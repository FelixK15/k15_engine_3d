/**
 * @file K15_GpuProgramParameter.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/12
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
Enum GpuProgramParameter::getParameterType() const
{
  return m_VariableType;
}
/*********************************************************************************/
const String& GpuProgramParameter::getParameterName() const
{
  return m_VariableName;
}
/*********************************************************************************/
void GpuProgramParameter::setParameterName(const String& p_Name)
{
  m_VariableName = p_Name;
  setName(p_Name);
}
/*********************************************************************************/
void GpuProgramParameter::setParameterType(Enum p_VariableType)
{
  m_VariableType = p_VariableType;
}
/*********************************************************************************/