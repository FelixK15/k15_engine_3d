#ifndef _K15_Math_Matrix3_h_
#define _K15_Math_Matrix3_h_

#include "K15_MathPrerequisites.h"

struct K15_Matrix3
{
	union
	{
		struct 
		{
			real32 _1_1, _2_1, _3_1;
			real32 _1_2, _2_2, _3_2;
			real32 _1_3, _2_3, _3_3;
		};

		real32 v[9];
		real32 m[3][3];
	};
};

K15_Matrix3 K15_GetIdentityMatrix3();
K15_Matrix3 K15_GetMatrix3Inverse(K15_Matrix3& p_Matrix);
K15_Matrix3 K15_GetMatrix3Transpose(K15_Matrix3& p_Matrix);
void K15_InverseMatrix3(K15_Matrix3& p_Matrix);
void K15_TransposeMatrix3(K15_Matrix3& p_Matrix);
real32 K15_GetMatrix3Determinant(K15_Matrix3& p_Matrix);

K15_Vector2 K15_GetTransformedVector(K15_Vector2& p_Vector, K15_Matrix3& p_Matrix);
K15_Vector3 K15_GetTransformedVector(K15_Vector3& p_Vector, K15_Matrix3& p_Matrix);

K15_Vector3 K15_GetXAxis(K15_Matrix3& p_Matrix);
K15_Vector3 K15_GetYAxis(K15_Matrix3& p_Matrix);
K15_Vector3 K15_GetZAxis(K15_Matrix3& p_Matrix);

K15_Vector3 K15_GetNormalizedXAxis(K15_Matrix3& p_Matrix);
K15_Vector3 K15_GetNormalizedYAxis(K15_Matrix3& p_Matrix);
K15_Vector3 K15_GetNormalizedZAxis(K15_Matrix3& p_Matrix);

K15_Matrix3 operator*(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2);
K15_Matrix3 operator*(K15_Matrix3& p_Matrix1, real32 p_Scalar);

bool operator==(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2);
bool operator!=(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2);

#endif //_K15_Math_Matrix3_h_s