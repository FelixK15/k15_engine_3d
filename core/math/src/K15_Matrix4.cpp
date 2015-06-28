#include "K15_Matrix4.h"
#include "K15_Vector3.h"
#include "K15_Vector4.h"
#include "K15_Quaternion.h"

/*********************************************************************************/
K15_Matrix4 K15_GetIdentityMatrix4()
{
	static K15_Matrix4 identityMatrix = {1.0, 0.0, 0.0, 0.0, 
										 0.0, 1.0, 0.0, 0.0,
										 0.0, 0.0, 1.0, 0.0,
										 0.0, 0.0, 0.0, 1.0};
	return identityMatrix;
}
/*********************************************************************************/
K15_Matrix4 K15_GetMatrix4Inverse(K15_Matrix4& p_Matrix)
{
	K15_Matrix4 inverseMatrix = {};

	return inverseMatrix;
}
/*********************************************************************************/
K15_Matrix4 K15_GetMatrix4Transpose(K15_Matrix4& p_Matrix)
{
	K15_Matrix4 transposeMatrix = {};


	return transposeMatrix;
}
/*********************************************************************************/
K15_Matrix4 K15_CreateComposedMatrix(K15_Vector3& p_Position, K15_Vector3& p_Scale, K15_Quaternion& p_Orientation)
{
	K15_Matrix4 composedMatrix = K15_CreateFromQuaternion(p_Orientation);

	composedMatrix._1_4 = p_Position.x;
	composedMatrix._2_4 = p_Position.y;
	composedMatrix._3_4 = p_Position.z;

	composedMatrix._1_1 *= p_Scale.x;
	composedMatrix._2_2 *= p_Scale.y;
	composedMatrix._3_3 *= p_Scale.z;

	return composedMatrix;
}
/*********************************************************************************/
K15_Matrix4 K15_CreateFromQuaternion(K15_Quaternion& p_Quaternion)
{
	K15_Matrix4 matrix = K15_GetIdentityMatrix4();

	matrix._1_1 = 1.0f - 2.0f * p_Quaternion.y * p_Quaternion.y - 2.0f * p_Quaternion.z * p_Quaternion.z;
	matrix._1_2 = 2.0f * p_Quaternion.x * p_Quaternion.y + 2.0f * p_Quaternion.w * p_Quaternion.z;
	matrix._1_3 = 2.0f * p_Quaternion.x * p_Quaternion.z - 2.0f * p_Quaternion.w * p_Quaternion.y;

	matrix._2_1 = 2.0f * p_Quaternion.x * p_Quaternion.y - 2.0f * p_Quaternion.w * p_Quaternion.z;
	matrix._2_2 = 1.0f - 2.0f * p_Quaternion.x * p_Quaternion.x - 2.0f * p_Quaternion.z * p_Quaternion.z;
	matrix._2_3 = 2.0f * p_Quaternion.y * p_Quaternion.z + 2.0f * p_Quaternion.w * p_Quaternion.x;

	matrix._3_1 = 2.0f * p_Quaternion.x * p_Quaternion.z + 2.0f * p_Quaternion.w * p_Quaternion.y;
	matrix._3_2 = 2.0f * p_Quaternion.y * p_Quaternion.z - 2.0f * p_Quaternion.w * p_Quaternion.x;
	matrix._3_3 = 1.0f - 2.0f * p_Quaternion.x * p_Quaternion.x - 2.0f * p_Quaternion.y * p_Quaternion.y;

	return matrix;
}
/*********************************************************************************/
void K15_InverseMatrix4(K15_Matrix4& p_Matrix)
{
	p_Matrix = K15_GetMatrix4Inverse(p_Matrix);
}
/*********************************************************************************/
void K15_TransposeMatrix4(K15_Matrix4& p_Matrix)
{
	p_Matrix = K15_GetMatrix4Transpose(p_Matrix);
}
/*********************************************************************************/
real32 K15_GetMatrix4Determinant(K15_Matrix4& p_Matrix)
{
	return 0.0;
}
/*********************************************************************************/
K15_Vector3 K15_GetTransformedVector(K15_Vector3& p_Vector, K15_Matrix4& p_Matrix)
{
	K15_Vector4 transformedVector = {};

	transformedVector.x = p_Vector.x;
	transformedVector.y = p_Vector.y;
	transformedVector.z = p_Vector.z;
	transformedVector.w = 0.0f;

	transformedVector = K15_GetTransformedVector(transformedVector, p_Matrix);

	K15_Vector3 transformedVector2 = {};

	transformedVector2.x = transformedVector.x;
	transformedVector2.y = transformedVector.y;
	transformedVector2.y = transformedVector.y;

	return transformedVector2;
}
/*********************************************************************************/
K15_Vector4 K15_GetTransformedVector(K15_Vector4& p_Vector, K15_Matrix4& p_Matrix)
{
	K15_Vector4 transformedVector = {};

	transformedVector.x = p_Matrix._1_1 * p_Vector.x + p_Matrix._2_1 * p_Vector.y + p_Matrix._3_1 * p_Vector.z + p_Matrix._4_1 * p_Vector.w;
	transformedVector.y = p_Matrix._1_2 * p_Vector.x + p_Matrix._2_2 * p_Vector.y + p_Matrix._3_2 * p_Vector.z + p_Matrix._4_2 * p_Vector.w;
	transformedVector.z = p_Matrix._1_3 * p_Vector.x + p_Matrix._2_3 * p_Vector.y + p_Matrix._3_3 * p_Vector.z + p_Matrix._4_3 * p_Vector.w;
	transformedVector.w = p_Matrix._1_4 * p_Vector.x + p_Matrix._2_4 * p_Vector.y + p_Matrix._3_4 * p_Vector.z + p_Matrix._4_4 * p_Vector.w;

	return transformedVector;
}
/*********************************************************************************/
K15_Vector3 K15_GetXAxis(K15_Matrix4& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_1;
	axis.y = p_Matrix._2_1;
	axis.z = p_Matrix._3_1;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetYAxis(K15_Matrix4& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_2;
	axis.y = p_Matrix._2_2;
	axis.z = p_Matrix._3_2;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetZAxis(K15_Matrix4& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_3;
	axis.y = p_Matrix._2_3;
	axis.z = p_Matrix._3_3;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedXAxis(K15_Matrix4& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetXAxis(p_Matrix));
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedYAxis(K15_Matrix4& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetYAxis(p_Matrix));
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedZAxis(K15_Matrix4& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetZAxis(p_Matrix));
}
/*********************************************************************************/
K15_Matrix4 operator*(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2)
{
	K15_Matrix4 matrixProduct = {};

	matrixProduct._1_1 = p_Matrix1._1_1 * p_Matrix2._1_1 + p_Matrix1._1_2 * p_Matrix2._2_1 + p_Matrix1._1_3 * p_Matrix2._3_1 + p_Matrix1._1_4 * p_Matrix2._4_1;
	matrixProduct._1_2 = p_Matrix1._1_1 * p_Matrix2._1_2 + p_Matrix1._1_2 * p_Matrix2._2_2 + p_Matrix1._1_3 * p_Matrix2._3_2 + p_Matrix1._1_4 * p_Matrix2._4_2;
	matrixProduct._1_3 = p_Matrix1._1_1 * p_Matrix2._1_3 + p_Matrix1._1_2 * p_Matrix2._2_3 + p_Matrix1._1_3 * p_Matrix2._3_3 + p_Matrix1._1_4 * p_Matrix2._4_3;
	matrixProduct._1_4 = p_Matrix1._1_1 * p_Matrix2._1_4 + p_Matrix1._1_2 * p_Matrix2._2_4 + p_Matrix1._1_3 * p_Matrix2._3_4 + p_Matrix1._1_4 * p_Matrix2._4_4;

	matrixProduct._2_1 = p_Matrix1._2_1 * p_Matrix2._1_1 + p_Matrix1._2_2 * p_Matrix2._2_1 + p_Matrix1._2_3 * p_Matrix2._3_1 + p_Matrix1._2_4 * p_Matrix2._4_1;
	matrixProduct._2_2 = p_Matrix1._2_1 * p_Matrix2._1_2 + p_Matrix1._2_2 * p_Matrix2._2_2 + p_Matrix1._2_3 * p_Matrix2._3_2 + p_Matrix1._2_4 * p_Matrix2._4_2;
	matrixProduct._2_3 = p_Matrix1._2_1 * p_Matrix2._1_3 + p_Matrix1._2_2 * p_Matrix2._2_3 + p_Matrix1._2_3 * p_Matrix2._3_3 + p_Matrix1._2_4 * p_Matrix2._4_3;
	matrixProduct._2_4 = p_Matrix1._2_1 * p_Matrix2._1_4 + p_Matrix1._2_2 * p_Matrix2._2_4 + p_Matrix1._2_3 * p_Matrix2._3_4 + p_Matrix1._2_4 * p_Matrix2._4_4;

	matrixProduct._3_1 = p_Matrix1._3_1 * p_Matrix2._1_1 + p_Matrix1._3_2 * p_Matrix2._2_1 + p_Matrix1._3_3 * p_Matrix2._3_1 + p_Matrix1._3_4 * p_Matrix2._4_1;
	matrixProduct._3_2 = p_Matrix1._3_1 * p_Matrix2._1_2 + p_Matrix1._3_2 * p_Matrix2._2_2 + p_Matrix1._3_3 * p_Matrix2._3_2 + p_Matrix1._3_4 * p_Matrix2._4_2;
	matrixProduct._3_3 = p_Matrix1._3_1 * p_Matrix2._1_3 + p_Matrix1._3_2 * p_Matrix2._2_3 + p_Matrix1._3_3 * p_Matrix2._3_3 + p_Matrix1._3_4 * p_Matrix2._4_3;
	matrixProduct._3_4 = p_Matrix1._3_1 * p_Matrix2._1_4 + p_Matrix1._3_2 * p_Matrix2._2_4 + p_Matrix1._3_3 * p_Matrix2._3_4 + p_Matrix1._3_4 * p_Matrix2._4_4;

	matrixProduct._4_1 = p_Matrix1._4_1 * p_Matrix2._1_1 + p_Matrix1._4_2 * p_Matrix2._2_1 + p_Matrix1._4_3 * p_Matrix2._3_1 + p_Matrix1._4_4 * p_Matrix2._4_1;
	matrixProduct._4_2 = p_Matrix1._4_1 * p_Matrix2._1_2 + p_Matrix1._4_2 * p_Matrix2._2_2 + p_Matrix1._4_3 * p_Matrix2._3_2 + p_Matrix1._4_4 * p_Matrix2._4_2;
	matrixProduct._4_3 = p_Matrix1._4_1 * p_Matrix2._1_3 + p_Matrix1._4_2 * p_Matrix2._2_3 + p_Matrix1._4_3 * p_Matrix2._3_3 + p_Matrix1._4_4 * p_Matrix2._4_3;
	matrixProduct._4_4 = p_Matrix1._4_1 * p_Matrix2._1_4 + p_Matrix1._4_2 * p_Matrix2._2_4 + p_Matrix1._4_3 * p_Matrix2._3_4 + p_Matrix1._4_4 * p_Matrix2._4_4;

	return matrixProduct;
}
/*********************************************************************************/
K15_Matrix4 operator*(K15_Matrix4& p_Matrix1, real32 p_Scalar)
{
	K15_Matrix4 matrixProduct = {};

	matrixProduct._1_1 = p_Matrix1._1_1 * p_Scalar;
	matrixProduct._1_2 = p_Matrix1._1_2 * p_Scalar;
	matrixProduct._1_3 = p_Matrix1._1_3 * p_Scalar;
	matrixProduct._1_4 = p_Matrix1._1_4 * p_Scalar;

	matrixProduct._2_1 = p_Matrix1._2_1 * p_Scalar;
	matrixProduct._2_2 = p_Matrix1._2_2 * p_Scalar;
	matrixProduct._2_3 = p_Matrix1._2_3 * p_Scalar;
	matrixProduct._2_4 = p_Matrix1._2_4 * p_Scalar;

	matrixProduct._3_1 = p_Matrix1._3_1 * p_Scalar;
	matrixProduct._3_2 = p_Matrix1._3_2 * p_Scalar;
	matrixProduct._3_3 = p_Matrix1._3_3 * p_Scalar;
	matrixProduct._3_4 = p_Matrix1._3_4 * p_Scalar;

	matrixProduct._4_1 = p_Matrix1._4_1 * p_Scalar;
	matrixProduct._4_2 = p_Matrix1._4_2 * p_Scalar;
	matrixProduct._4_3 = p_Matrix1._4_3 * p_Scalar;
	matrixProduct._4_4 = p_Matrix1._4_4 * p_Scalar;

	return matrixProduct;
}
/*********************************************************************************/
bool operator==(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2)
{
	return memcmp(p_Matrix1.m, p_Matrix2.m, sizeof(p_Matrix1.m)) == 0;
}
/*********************************************************************************/
bool operator!=(K15_Matrix4& p_Matrix1, K15_Matrix4& p_Matrix2)
{
	return memcmp(p_Matrix1.m, p_Matrix2.m, sizeof(p_Matrix1.m)) != 0;
}
/*********************************************************************************/