#ifndef _K15_Math_Math_h_
#define _K15_Math_Math_h_

#include "K15_MathPrerequisites.h"

real32 K15_Lerp(real32 p_Start, real32 p_End, real32 p_Time);
K15_Vector2 K15_Lerp(K15_Vector2& p_Start, K15_Vector2& p_End, real32 p_Time);
K15_Vector3 K15_Lerp(K15_Vector3& p_Start, K15_Vector3& p_End, real32 p_Time);

real32 K15_ClampReal(real32 p_Value, real32 p_Max, real32 p_Min);

real32 K15_FastSqrt(real32 p_Number);

real32 K15_LowPrecisionSin(real32 p_Number);
real32 K15_LowPrecisionCos(real32 p_Number);

real32 K15_HighPrecisionSin(real32 p_Number);
real32 K15_HighPrecisionCos(real32 p_Number);

#endif //_K15_Math_Math_h_