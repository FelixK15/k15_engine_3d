/**
 * @file K15_MemoryPools.cpp
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

#include "K15_PrecompiledHeader.h"

#include "K15_MemoryPools.h"
#include "K15_Material.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  MaterialAllocator::MaterialAllocator()
    : PoolAllocator<K15_SIZE_MATERIAL>(MemoryPoolsAllocator,MaterialCount,_N(MaterialAllocator))
  {

  }
  /*********************************************************************************/
  MaterialAllocator::~MaterialAllocator()
  {

  }
  /*********************************************************************************/

  /*********************************************************************************/
  MemoryPools::MemoryPools()
    : BlockAllocator(ApplicationAllocator,10 * MEGABYTE,_N(MemoryPools))
  {
    _LogNormal("Creating Material Memory Pool...");
    m_MaterialAllocator = K15_NEW MaterialAllocator();
  }
  /*********************************************************************************/
  MemoryPools::~MemoryPools()
  {
    _LogNormal("Destroying Material memory pool...");
    K15_DELETE m_MaterialAllocator;
  }
  /*********************************************************************************/
}} // end of K15_Engine::Core namespace