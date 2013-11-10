/**
 * @file K15_Math_Prerequisites.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Math_Prerequisites_h_
#define _K15Engine_Math_Prerequisites_h_

#include "K15_Prerequisites.h"
#include <math.h>

//we'll use SIMD math
#define K15_SIMD_SUPPORT

#if defined K15_SIMD_SUPPORT

# include <nmmintrin.h> //SSE4.2
#endif //K15_SIMD_SUPPORT

const float _Pi = 4.0f*atan(1.0f);
const float _HalfPi = _Pi * 0.5f;
const float _TwoPi = _Pi + _Pi;

#endif //_K15Engine_Math_Prerequisites_h_