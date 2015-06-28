#ifndef _K15_Math_Matrix4_h_
#define _K15_Math_Matrix4_h_

#include "K15_MathPrerequisites.h"

struct K15_Matrix4
{
	union 
	{
		struct 
		{
			real32 _1_1, _2_1, _3_1, _4_1;
			real32 _1_2, _2_2, _3_2, _4_2;
			real32 _1_3, _2_3, _3_3, _4_3;
			real32 _1_4, _2_4, _3_4, _4_4;
		};

		real32 v[16];
		real32 m[4][4];
	};
};

K15_Matrix4 K15_GetIdentityMatrix4();
K15_Matrix4 K15_GetMatrix4Transpose(K15_Matrix4& p_Matrix);
K15_Matrix4 K15_CreateComposedMatrix(K15_Vector3& p_Position, K15_Vector3& p_Scale, K15_Quaternion& p_Orientation);
K15_Matrix4 K15_CreateFromQuaternion(K15_Quaternion& p_Quaternion);

void K15_TransposeMatrix4(K15_Matrix4& p_Matrix);
real32 K15_GetMatrix4Determinant(K15_Matrix4& p_Matrix);

K15_Vector3 K15_GetTransformedVector(K15_Vector3& p_Vector, K15_Matrix4& p_Matrix);
K15_Vector4 K15_GetTransformedVector(K15_Vector4& p_Vector, K15_Matrix4& p_Matrix);

K15_Vector3 K15_GetXAxis(K15_Matrix4& p_Matrix);
K15_Vector3 K15_GetYAxis(K15_Matrix4& p_Matrix);
K15_Vector3 K15_GetZAxis(K15_Matrix4& p_Matrix);

K15_Vector3 K15_GetNormalizedXAxis(K15_Matrix4& p_Matrix);
K15_Vector3 K15_GetNormalizedYAxis(K15_Matrix4& p_Matrix);
K15_Vector3 K15_GetNormalizedZAxis(K15_Matrix4& p_Matrix);

K15_Matrix4 operator*(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2);
K15_Matrix4 operator*(K15_Matrix4& p_Matrix1, real32 p_Scalar);

bool operator==(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2);
bool operator!=(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2);

#endif //_K15_Math_Matrix4_h_