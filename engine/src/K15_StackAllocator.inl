/**
 * @file K15_Allocator.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

template <class Allocator>
Allocator::Allocator(Allocator* pAllocator,size_t iSize)
  : m_pMemory(0),
    m_iUsedMemory(0),
    m_iMemorySize(0)
{
  m_pMemory     = pAllocator->allocate(iSize);
  m_iMemorySize = iSize;
}