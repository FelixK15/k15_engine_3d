/**
 * @file K15_GpuProgramBatch.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/05
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
GpuProgramBatch* GpuProgramBatchImplBase::getProgramBatch() const
{
	return m_ProgramBatch;
}
/*********************************************************************************/
void GpuProgramBatchImplBase::setProgramBatch(GpuProgramBatch* p_Batch)
{
	m_ProgramBatch = p_Batch;
}
/*********************************************************************************/

/*********************************************************************************/
void GpuProgramBatch::setError(const String& p_Error)
{
	m_Error = p_Error;
}
/*********************************************************************************/
const String& GpuProgramBatch::getError() const
{
	return m_Error;
}
/*********************************************************************************/
const GpuProgramBatch::GpuProgramList& GpuProgramBatch::getGpuProgramList() const
{
	return m_Programs;
}
/*********************************************************************************/
GpuProgramBatchImplBase* GpuProgramBatch::getImpl() const
{
	return m_Impl;
}
/*********************************************************************************/
void GpuProgramBatch::setImpl(GpuProgramBatchImplBase* p_Impl)
{
	m_Impl = p_Impl;
	m_Impl->setProgramBatch(this);
}
/*********************************************************************************/