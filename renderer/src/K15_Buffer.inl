/**
 * @file K15_Buffer.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

inline U32 Buffer::GetBufferSize() const
{
	return m_iBufferSize;
}

inline U32 Buffer::GetStride() const
{
	return m_iStride;
}

inline Buffer::BufferUsage Buffer::GetBufferUsage() const
{
	return m_buUsage;
}

inline Buffer::BufferBinding Buffer::GetBufferBinding() const
{
	return m_bbBinding;
}

inline Buffer::BufferAccess Buffer::GetBufferAccess() const
{
	return m_baAccess;
}

inline FormatType Buffer::GetFormat() const
{
	return m_ftType;
}

inline void *Buffer::GetData() const
{
	return m_pData;
}

inline void Buffer::SetData(void *pData)
{
	Memory::MemCopy(m_pData,pData,m_iBufferSize);
}