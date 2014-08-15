/**
 * @file K15_MemoryHeader.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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
 *
 * 
 */

#ifndef _K15Engine_Core_MemoryBlock_h_
#define _K15Engine_Core_MemoryBlock_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Core {
	struct MemoryBlock
	{
		MemoryBlock();
		~MemoryBlock();

        bool Used;          // Is the memory of this block still in use?
        byte* Memory;       // Pointer to the memory this block manages
        size_t Size;        // Size of the memory this block manages
        MemoryBlock* Next;  // Pointer to the next block or NULL if last block
	};
}}//end of K15_Engine::Core namespace

#endif //_K15Engine_Core_MemoryBlock_h_
