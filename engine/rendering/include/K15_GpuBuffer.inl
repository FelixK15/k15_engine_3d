/**
 * @file K15_GpuBuffer.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
void GpuBuffer::setLockOption(Enum p_LockOption)
{
	m_LockOption = p_LockOption;
}
/*********************************************************************************/
void GpuBuffer::setType(Enum p_BufferType)
{
	m_BufferType = p_BufferType;
}
/*********************************************************************************/
void GpuBuffer::setUsageOption(Enum p_UsageOption)
{
	m_UsageOption = p_UsageOption;
}
/*********************************************************************************/
void GpuBuffer::setAccessOption(Enum p_AccessOption)
{
  m_AccessOption = p_AccessOption;
}
/*********************************************************************************/
Enum GpuBuffer::getLockOption() const
{
	return m_LockOption;
}
/*********************************************************************************/
Enum GpuBuffer::getUsageOption() const
{
	return m_UsageOption;
}
/*********************************************************************************/
Enum GpuBuffer::getType() const
{
	return m_BufferType;
}
/*********************************************************************************/
Enum GpuBuffer::getAccessOption() const
{
	return m_AccessOption;
}
/*********************************************************************************/
bool GpuBuffer::isLocked() const
{
	return m_Locked;
}
/*********************************************************************************/
uint32 GpuBuffer::getSize() const
{
	return m_Size;
}
/*********************************************************************************/
uint32 GpuBuffer::getUsedSize() const
{
  return m_UsedSize;
}
/*********************************************************************************/
uint32 GpuBuffer::getFreeSize() const
{
	return m_Size - m_UsedSize;
}
/*********************************************************************************/
GpuBufferImplBase* GpuBuffer::getImpl()
{
	return m_Impl;
}
/*********************************************************************************/
bool GpuBuffer::isDirty() const
{
	return m_Dirty;
}
/*********************************************************************************/
void GpuBuffer::setDirty(bool p_Dirty)
{
	m_Dirty = p_Dirty;
}
/*********************************************************************************/