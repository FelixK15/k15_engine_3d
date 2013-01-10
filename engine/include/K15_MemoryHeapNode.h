/**
 * @file K15_MemoryHeapNode.h
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
#pragma once

#ifndef __K15_MEMORYHEAPNODE__
#define __K15_MEMORYHEAPNODE__

namespace K15_EngineV2
{
	struct K15ENGINE2_API MemoryHeapNode
	{
		void *Pointer;		/*Address of the allocated memory*/
		unsigned int Size;	/*Size of the allocated memory*/
		bool IsFree;		/*Is the block free?*/
	};
}

#endif //__K15_MEMORYHEAPNODE__