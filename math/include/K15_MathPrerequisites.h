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

#include <cmath>
#include <cstring>
#include <cassert>
#include <cstdlib>

//we'll use SIMD math
#define K15_SIMD_SUPPORT

#if defined K15_SIMD_SUPPORT

# include <nmmintrin.h> //SSE4.2
#endif //K15_SIMD_SUPPORT

namespace K15_Engine { namespace Math {
  const float Pi = 4.0f*atan(1.0f);
  const float HalfPi = Pi * 0.5f;
  const float TwoPi = Pi + Pi;
  const float DegreeToRadian = Pi / 180.f;
  const float RadianToDegree = 180.f / Pi;
}} // end of K15_Engine::Math namespace
#endif //_K15Engine_Math_Prerequisites_h_