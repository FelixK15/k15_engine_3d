/**
 * @file K15_GpuProgram.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/28
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
void GpuProgramImplBase::setGpuProgram(GpuProgram* p_GpuProgram)
{
	m_GpuProgram = p_GpuProgram;
}
/*********************************************************************************/
GpuProgram* GpuProgramImplBase::getGpuProgram() const
{
	return m_GpuProgram;
}
/*********************************************************************************/

/*********************************************************************************/
uint32 GpuProgram::getAmountUniforms() const
{
  return m_UsedUniforms;
}
/*********************************************************************************/
bool GpuProgram::isCompiled() const
{
	return m_Compiled;
}
/*********************************************************************************/
const String& GpuProgram::getError() const
{
	return m_Error;
}
/*********************************************************************************/
const String& GpuProgram::getShaderCode() const
{
	return m_ShaderCode;
}
/*********************************************************************************/
const RawData* GpuProgram::getBinaryCode() const
{
	return &m_BinaryCode;
}
/*********************************************************************************/
Enum GpuProgram::getStage() const
{
	return m_Stage;
}
/*********************************************************************************/
void GpuProgram::setError(const String& p_Error)
{
	m_Error = p_Error;
}
/*********************************************************************************/
GpuProgramImplBase* GpuProgram::getImpl()
{
	return m_Impl;
}
/*********************************************************************************/
void GpuProgram::setCompiled(bool p_Compiled)
{
	m_Compiled = p_Compiled;
}
/*********************************************************************************/