/**
 * @file K15_Heap.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
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

inline size_t Heap::GetFreeMemory() const
{ 
	return m_iSize - m_iUsedMemory; 
}

inline size_t Heap::GetUsedMemory() const
{ 
	return m_iUsedMemory; 
}

inline size_t Heap::GetHeapSize() const
{ 
	return m_iSize; 
}