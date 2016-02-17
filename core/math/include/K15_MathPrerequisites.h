#ifndef _K15_Math_Prerequisties_h_
#define _K15_Math_Prerequisties_h_

#include "K15_Common.h"

#pragma warning(disable:4244)
#pragma warning(disable:4305)

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define K15_EPSILON				1e-5
#define K15_TWO_PI				6.28318531
#define K15_PI					3.14159265
#define K15_HALF_PI				1.57079632
#define K15_QUATER_PI			0.78539816

#define K15_MAX(a, b) (a) > (b) ? (a) : (b)
#define K15_MIN(a, b) (a) < (b) ? (a) : (b)
#define K15_CLAMP(v, ma, mi) ((v) > (ma)) ? (ma) : ((v) < (mi)) ? (mi) : (v)
#define K15_ROUND(f) (f) > INT_MAX ? INT_MAX : (f) < -INT_MAX ? -INT_MAX : (f) > 0.f ? (f) + 0.5f : (f) - 0.5f

typedef float real32;

struct K15_Vector2;
struct K15_Vector3;
struct K15_Vector4;
struct K15_Matrix3;
struct K15_Matrix4;
struct K15_Quaternion;

#endif //_K15_Math_Prerequisties_h_
