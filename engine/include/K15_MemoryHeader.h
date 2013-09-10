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
#ifndef _K15Engine_System_Memory_MemoryHeader_h_
#define _K15Engine_System_Memory_MemoryHeader_h_

namespace K15_Engine { namespace System { namespace Memory {
{
	struct MemoryHeader
	{
		const char *File;		  /*File the allocation occurred*/
    const char *Function; /*Function the allocation occured*/
		unsigned int Line;		/*Line at which the new call happened*/
		unsigned int Size;		/*Size of the allocated memory*/
		bool IsArray;			    /*Was the memory allocated by new[]?*/
	};
}}}

#endif //_K15Engine_System_Memory_MemoryHeader_h_