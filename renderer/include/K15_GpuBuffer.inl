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

inline void GpuBuffer::setShadowCopyEnabled(bool p_Enabled)
{
	m_ShadowCopyEnabled = p_Enabled;
}
/*********************************************************************************/
inline void GpuBuffer::setLockOption(Enum p_LockOption)
{
	m_LockOption = p_LockOption;
}
/*********************************************************************************/
inline void GpuBuffer::setUsageOption(Enum p_UsageOption)
{
	m_UsageOption = p_UsageOption;
}
/*********************************************************************************/
inline Enum GpuBuffer::getLockOption() const
{
	return m_LockOption;
}
/*********************************************************************************/
inline Enum GpuBuffer::getUsageOption() const
{
	return m_UsageOption;
}
/*********************************************************************************/
inline Enum GpuBuffer::getType() const
{
	return m_BufferType;
}
/*********************************************************************************/
inline Enum GpuBuffer::getAccessOption() const
{
	return m_AccessOption;
}
/*********************************************************************************/
inline bool GpuBuffer::getShadowCopyEnabled() const
{
	return m_ShadowCopyEnabled;
}
/*********************************************************************************/
inline byte* GpuBuffer::getShadowCopy() const
{
	return m_ShadowCopy;
}
/*********************************************************************************/
inline uint32 GpuBuffer::getShadowCopySize() const
{
	return m_ShadowCopySize;
}
/*********************************************************************************/
inline bool GpuBuffer::isLocked()
{
	return m_Locked;
}
/*********************************************************************************/
inline uint32 GpuBuffer::getSize()
{
	return m_Size;
}
/*********************************************************************************/