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
bool GpuProgramParameter::isAutoParameter() const
{
  return m_AutoParameter;
}
/*********************************************************************************/
Enum GpuProgramParameter::getType() const
{
  return m_Type;
}
/*********************************************************************************/
uint32 GpuProgramParameter::getSize() const
{
  return m_Size;
}
/*********************************************************************************/
const ObjectName& GpuProgramParameter::getAutoName() const
{
  return m_AutoName;
}
/*********************************************************************************/
const String& GpuProgramParameter::getName() const
{
  return m_Name;
}
/*********************************************************************************/
int32 GpuProgramParameter::getBufferIndex() const
{
  return m_BufferIndex;
}
/*********************************************************************************/
uint32 GpuProgramParameter::getOffset() const
{
  return m_Offset;
}
/*********************************************************************************/
uint32 GpuProgramParameter::getRegisterIndex() const
{
  return m_RegisterIndex;
}
/*********************************************************************************/
void GpuProgramParameter::setAutoParameter(bool p_Value)
{
  m_AutoParameter = p_Value;
}
/*********************************************************************************/
void GpuProgramParameter::setType(Enum p_VariableType)
{
  m_Type = p_VariableType;
}
/*********************************************************************************/
void GpuProgramParameter::setSize(uint32 p_Size)
{
  m_Size = p_Size;
}
/*********************************************************************************/
void GpuProgramParameter::setName(const String& p_Name)
{
  m_Name = p_Name;
}
/*********************************************************************************/
void GpuProgramParameter::setAutoName(const ObjectName& p_Name)
{
  m_AutoName = p_Name;
}
/*********************************************************************************/
void GpuProgramParameter::setBufferIndex(int32 p_Block)
{
  m_BufferIndex = p_Block;
}
/*********************************************************************************/
void GpuProgramParameter::setOffset(uint32 p_Offset)
{
  m_Offset = p_Offset;
}
/*********************************************************************************/
void GpuProgramParameter::setRegisterIndex(uint32 p_Location)
{
  m_RegisterIndex = p_Location;
}
/*********************************************************************************/