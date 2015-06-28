#ifndef _K15_Math_Vector2_h_
#define _K15_Math_Vector2_h_

#include "K15_MathPrerequisites.h"

struct K15_Vector2
{
	union
	{
		struct  
		{
			real32 x;
			real32 y;
		};

		real32 v[2];
	};
};

real32 K15_GetVectorDotProduct(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
real32 K15_GetVectorAngle(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
real32 K15_GetVectorLength(K15_Vector2& p_Vector);
real32 K15_GetVectorSquaredLength(K15_Vector2& p_Vector);
K15_Vector2 K15_CreateVector(real32 p_ComponentX, real32 p_ComponentY);
K15_Vector2 K15_InitializeVector(K15_Vector2& p_Vector, real32 p_ComponentX, real32 p_ComponentY);
K15_Vector2 K15_GetProjectedVector(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
K15_Vector2 K15_GetNormalizedVector(K15_Vector2& p_Vector);
void K15_NormalizeVector(K15_Vector2& p_Vector);

K15_Vector2 operator+(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
K15_Vector2 operator-(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
K15_Vector2 operator*(K15_Vector2& p_Vector, real32 p_Scalar);
K15_Vector2 operator*(real32 p_Scalar, K15_Vector2& p_Vector);

bool operator==(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);
bool operator!=(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2);

#endif //_K15_Math_Vector2_h_