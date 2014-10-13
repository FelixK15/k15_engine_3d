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

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <cstdlib>

#define K15_EPSILON 1e-5
#define GLM_FORCE_RADIANS

namespace K15_Engine { namespace Math {
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Quaternion;
}}

//we won't use SIMD for android.
#ifndef __ANDROID__
	#define K15_SIMD_SUPPORT
#else
	#define K15_NEON_SUPPORT
#endif //__ANDROID__))

#if defined K15_SIMD_SUPPORT
//	#include <tmmintrin.h>
#elif defined K15_NEON_SUPPORT
//	#include <arm_neon.h>
#endif //K15_SIMD_SUPPORT

namespace K15_Engine { namespace Math {
#if defined K15_MATH_DOUBLE_PRECISION
	typedef double Real;
#elif defined K15_MATH_HALF_PRECISION
	typedef half Real;
#else
	typedef float Real;
#endif //K15_MATH_DOUBLE_PRECISION
}}

#endif //_K15Engine_Math_Prerequisites_h_
