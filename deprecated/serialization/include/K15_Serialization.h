/**
 * @file K15_Serialization.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#ifndef _K15Engine_Serialization_Serialization_h_
#define _K15Engine_Serialization_Serialization_h_

#include "K15_Prerequisites.h"

#if defined K15_DEBUG
#	include "K15_JsonSerializer.h"

#else
#	include "K15_SerialWriter.h"
#	include "K15_SerialReader.h"
#	include "K15_SerialSizer.h"
#endif //K15_DEBUG

#endif //_K15Engine_Serialization_Serialization_h_