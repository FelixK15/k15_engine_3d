/**
 * @file K15_UsefulMacros.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/23
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

#ifndef __K15_USEFULMACROS__
#define __K15_USEFULMACROS__

#ifndef TRUE
#define TRUE (1)
#define FALSE (0)
#endif

#define HAS_FLAG(flag,var) (flag & var) == flag

#ifdef _REAL_NULL_PTR
#undef NULL
#define 0 nullptr
#endif

#define GIGABYTE 1073741824
#define MEGABYTE 1048576
#define KILOBYTE 1024

#endif //__K15_STANDARDINCLUDE__