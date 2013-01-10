/**
 * @file K15_Memory.inl
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

inline unsigned int Memory::GetAllocatedMemory()
{
	return ms_iAllocatedMemory;
}

inline unsigned int Memory::GetApplicationMemory()
{
	return ms_iApplicationMemory;
}

inline unsigned int Memory::GetSystemMemory()
{
	return ms_iSystemMemory;
}

inline void Memory::SetApplicationMemory(size_t iMemory)
{
	ms_iApplicationMemory = iMemory;
}

inline void Memory::SetSystemMemory(size_t iMemory)
{
	ms_iSystemMemory = iMemory;
}

inline void Memory::EnableMemorySerialization()
{
	ms_bMemorySerialization = true;
}

inline unsigned int Memory::GetAmountOfMissingDeleteStatements()
{
	return ms_iAmountAllocations;
}