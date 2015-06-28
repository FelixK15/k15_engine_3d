#ifndef _K15_Math_Vector4_h_
#define _K15_Math_Vector4_h_

#include "K15_MathPrerequisites.h"

struct K15_Vector4
{
	union
	{
		struct 
		{
			real32 x;
			real32 y;
			real32 z;
			real32 w;
		};

		real32 v[4];
	};
};

real32 K15_GetVectorDotProduct(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
real32 K15_GetVectorLength(K15_Vector4& p_Vector);
real32 K15_GetVectorSquaredLength(K15_Vector4& p_Vector);
K15_Vector4 K15_GetCrossProduct(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
K15_Vector4 K15_GetProjectedVector(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
K15_Vector4 K15_GetNormalizedVector(K15_Vector4& p_Vector);
void K15_NormalizeVector(K15_Vector4& p_Vector);

K15_Vector4 operator+(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
K15_Vector4 operator-(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
K15_Vector4 operator*(K15_Vector4& p_Vector, real32 p_Scalar);
K15_Vector4 operator*(real32 p_Scalar, K15_Vector4& p_Vector);

bool operator==(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);
bool operator!=(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2);

#endif //_K15_Math_Vector4_h_