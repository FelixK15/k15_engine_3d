/**
 * @file K15_MemoryAllocation.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
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

#include "K15_MemoryAllocation.h"

using namespace K15_EngineV2;

MemoryAllocation::MemoryAllocation()
{
	m_sFile = NULL;
	m_iLineNumber = 0;
	m_iSize = 0;
}

MemoryAllocation::MemoryAllocation( unsigned int iSize,int iLineNumber,const char* sFile )
{
	m_iSize = iSize;
	m_iLineNumber = iLineNumber;
	m_sFile = sFile;
}
