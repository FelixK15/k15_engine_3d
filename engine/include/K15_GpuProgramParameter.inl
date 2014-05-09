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
Enum GpuProgramParameter::getIdentifier() const
{
  return m_Identifier;
}
/*********************************************************************************/
Enum GpuProgramParameter::getUpdateFrequency() const
{
	return m_UpdateFrequency;
}
/*********************************************************************************/
uint32 GpuProgramParameter::getSize() const
{
  return m_Size;
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
void GpuProgramParameter::setType(Enum p_VariableType)
{
  m_Type = p_VariableType;
}
/*********************************************************************************/
void GpuProgramParameter::setUpdateFrequency(Enum p_UpdateFrequency)
{
	m_UpdateFrequency = p_UpdateFrequency;
}
/*********************************************************************************/
void GpuProgramParameter::setSize(uint32 p_Size)
{
  m_Size = p_Size;
}
/*********************************************************************************/
void GpuProgramParameter::setIdentifier(Enum p_Identifier)
{
  if(m_Identifier < PI_COUNT)
  {
      m_Name = UniformIdentifierName[m_Identifier];
      m_AutoParameter = true;
  }

  if(m_Identifier == GpuProgramParameter::PI_MODEL_MATRIX)
  {
	  m_UpdateFrequency = GpuProgramParameter::UF_PER_MESH;
  }
  else
  {
	  m_UpdateFrequency = GpuProgramParameter::UF_PER_FRAME;
  }

  m_Identifier = p_Identifier;
}
/*********************************************************************************/
void GpuProgramParameter::setAutoParameter(bool p_Value)
{
	m_AutoParameter = p_Value;
}
/*********************************************************************************/
void GpuProgramParameter::setName(const String& p_Name)
{
  m_Name = p_Name;
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
void* GpuProgramParameter::getData() const
{
	return m_Data;
}
/*********************************************************************************/
void GpuProgramParameter::setData(void* p_UserData)
{
	m_Data = p_UserData;
}
/*********************************************************************************/
void GpuProgramParameter::setGpuProgram(GpuProgram* p_Program)
{
	m_Program = p_Program;
}
/*********************************************************************************/
GpuProgram* GpuProgramParameter::getGpuProgram() const
{
	return m_Program;
}
/*********************************************************************************/