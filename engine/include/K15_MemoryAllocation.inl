/**
 * @file K15_MemoryAllocation.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/11
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
 *
 * @section DESCRIPTION
 */

inline unsigned int MemoryAllocation::GetSize() const
{
	return m_iSize;
}

inline const char *MemoryAllocation::GetFile() const
{
	return m_sFile;
}

inline unsigned int MemoryAllocation::GetLineNumber() const
{
	return m_iLineNumber;
}