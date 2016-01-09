#ifndef _K15_Math_Vector3_h_
#define _K15_Math_Vector3_h_

#include "K15_MathPrerequisites.h"

struct K15_Vector3
{
	union 
	{
		struct 
		{
			real32 x;
			real32 y;
			real32 z;
		};

		real32 v[3];
	};
};

real32 K15_GetVectorDotProduct(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
real32 K15_GetVectorLength(K15_Vector3& p_Vector);
real32 K15_GetVectorSquaredLength(K15_Vector3& p_Vector);
K15_Vector3 K15_CreateVector(real32 p_ComponentX, real32 p_ComponentY, real32 p_ComponentZ);
K15_Vector3 K15_InitializeVector(K15_Vector3& p_Vector3, real32 p_ComponentX, real32 p_ComponentY, real32 p_ComponentZ);
K15_Vector3 K15_GetCrossProduct(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
K15_Vector3 K15_GetProjectedVector(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
K15_Vector3 K15_GetNormalizedVector(K15_Vector3& p_Vector);
void K15_NormalizeVector(K15_Vector3& p_Vector);

K15_Vector3 K15_UnpackVector3(uint32 p_PackedVector);
uint32 K15_PackVector3(const K15_Vector3& p_UnpackedVector);

K15_Vector3 operator+(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
K15_Vector3 operator-(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
K15_Vector3 operator*(K15_Vector3& p_Vector, real32 p_Scalar);
K15_Vector3 operator*(real32 p_Scalar, K15_Vector3& p_Vector);
K15_Vector3 operator/(K15_Vector3& p_Vector, real32 p_Scalar);

bool operator==(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);
bool operator!=(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2);

#endif //_K15_Math_Vector3_h_