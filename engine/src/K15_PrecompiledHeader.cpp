/**
 * @file K15_PrecompiledHeader.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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

#pragma message("Compiling precompiled headers...Should only be done once per build.")

#ifdef K15_OS_WINDOWS
#	pragma message("Compiling for Windows...")
#elif K15_OS_LINUX
#	pragma message("Compiling for Linux...")
#elif K15_OS_APPLE
#	pragma message("Compiling for Mac...")
#endif //K15_OS_WINDOWS

