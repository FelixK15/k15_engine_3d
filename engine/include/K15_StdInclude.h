/**
 * @file K15_StdInlcude.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/10
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

#ifndef __K15_STANDARDINCLUDE__
#define __K15_STANDARDINCLUDE__

typedef char S8;
typedef short S16;
typedef int S32;
typedef long long S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef unsigned char byte;

typedef U8 BOOL8;

#include "K15_Memory.h"
#include "K15_UsefulMacros.h"
#include "K15_CStdIncludes.h"

#endif //__K15_STANDARDINCLUDE__