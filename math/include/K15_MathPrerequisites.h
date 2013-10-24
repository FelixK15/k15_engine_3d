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

#include <math.h>

//we'll use SIMD math
#define K15_SIMD_SUPPORT

#if defined K15_SIMD_SUPPORT
# include <xmmintrin.h> //SSE
#endif //K15_SIMD_SUPPORT

#if defined K15_MATH_USE_DOUBLE_PRECISION
  typedef double Real;
#else
  typedef float Real;
#endif //K15_MATH_USE_DOUBLE_PRECISION

const Real _Pi = 4.0*atan(1.0);
const Real _HalfPi = _Pi * 0.5;
const Real _TwoPi = _Pi + _Pi;

#endif //_K15Engine_Math_Prerequisites_h_