#ifndef _K15_Math_Math_h_
#define _K15_Math_Math_h_

#include "K15_MathPrerequisites.h"

real32 K15_Lerp(real32 p_Start, real32 p_End, real32 p_Time);
K15_Vector2 K15_Lerp(K15_Vector2 p_Start, K15_Vector2 p_End, real32 p_T);
K15_Vector3 K15_Lerp(K15_Vector3 p_Start, K15_Vector3 p_End, real32 p_T);
K15_Vector3 K15_LerpColor(K15_Vector3 p_Start, K15_Vector3 p_End, real32 p_T);

K15_Vector3 K15_ConvertToHSV(const K15_Vector3& p_RGBColor);
K15_Vector3 K15_ConvertToRGB(const K15_Vector3& p_HSVColor);

real32 K15_Log2(real32 p_Number);

real32 K15_ClampReal(real32 p_Value, real32 p_Max, real32 p_Min);
real32 K15_SafeDivide(real32 p_Number, real32 p_Divisor);

real32 K15_FastSqrt(real32 p_Number);

real32 K15_LowPrecisionSin(real32 p_Number);
real32 K15_LowPrecisionCos(real32 p_Number);

real32 K15_HighPrecisionSin(real32 p_Number);
real32 K15_HighPrecisionCos(real32 p_Number);

uint32 K15_GetNextPowerOfTwoSize(uint32 p_NonPowerOfTwoNumber);

bool8 K15_Collision2DBoxPoint(float p_BoxLeft, float p_BoxTop, float p_BoxRight, float p_BoxBottom,
							  float p_PointX, float p_PointY);

#endif //_K15_Math_Math_h_