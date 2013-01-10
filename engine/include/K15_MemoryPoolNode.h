/**
 * @file K15_MemoryPoolNode.h
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
 *
 * @section DESCRIPTION
 *
 * The MemoryPoolNode is used by the MemoryPool class to keep track of allocated memory.
 */
#pragma once

#ifndef __K15_MEMORYPOOLNODE__
#define __K15_MEMORYPOOLNODE__

namespace K15_EngineV2
{
	struct MemoryPoolNode
	{
		void *Pointer;				/*Address of the allocted memory*/
		MemoryPoolNode *Next;		/*Address to the next free node*/
	};
}

#endif //__K15_MEMORYPOOLNODE__