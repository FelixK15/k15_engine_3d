/**
 * @file K15_ObjectSizes.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/12
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

#ifdef K15_64_BIT
#	define K15_SIZE_MATERIAL 116
#	define K15_SIZE_MEMORY_BLOCK 28
#	define K15_SIZE_RENDER_OPERATION 36
#	define K15_SIZE_PROFILING_NODE 48
#else
#	define K15_SIZE_MATERIAL 92
#	define K15_SIZE_MEMORY_BLOCK 20
#	define K15_SIZE_RENDER_OPERATION 20
#	define K15_SIZE_ALPHA_STATE 32
# define K15_SIZE_PROFILING_NODE 152
#endif //K15_64_BIT