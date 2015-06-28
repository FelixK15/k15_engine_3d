#include "K15_Matrix3.h"
#include "K15_Vector2.h"
#include "K15_Vector3.h"

/*********************************************************************************/
K15_Matrix3 K15_GetIdentityMatrix3()
{
	static K15_Matrix3 identityMatrix = {1.0, 0.0, 0.0, 
										 0.0, 1.0, 0.0, 
										 0.0, 0.0, 1.0};
	return identityMatrix;
}
/*********************************************************************************/
K15_Matrix3 K15_GetMatrix3Inverse(K15_Matrix3& p_Matrix)
{
	K15_Matrix3 inverseMatrix = {};

	return inverseMatrix;
}
/*********************************************************************************/
K15_Matrix3 K15_GetMatrix3Transpose(K15_Matrix3& p_Matrix)
{
	K15_Matrix3 transposeMatrix = {};

	transposeMatrix._1_2 = p_Matrix._2_1;
	transposeMatrix._1_3 = p_Matrix._3_1;
	transposeMatrix._2_1 = p_Matrix._1_2;
	transposeMatrix._2_3 = p_Matrix._3_2;
	transposeMatrix._3_1 = p_Matrix._1_3;
	transposeMatrix._3_2 = p_Matrix._2_3;

	return transposeMatrix;
}
/*********************************************************************************/
void K15_InverseMatrix3(K15_Matrix3& p_Matrix)
{
	p_Matrix = K15_GetMatrix3Inverse(p_Matrix);
}
/*********************************************************************************/
void K15_TransposeMatrix3(K15_Matrix3& p_Matrix)
{
	p_Matrix = K15_GetMatrix3Transpose(p_Matrix);
}
/*********************************************************************************/
real32 K15_GetMatrix3Determinant(K15_Matrix3& p_Matrix)
{
	return	p_Matrix._1_1 * (p_Matrix._2_2 * p_Matrix._3_3 - p_Matrix._3_2 * p_Matrix._2_3) - 
		p_Matrix._2_1 * (p_Matrix._1_2 * p_Matrix._3_3 - p_Matrix._3_2 * p_Matrix._1_3) + 
		p_Matrix._3_1 * (p_Matrix._1_2 * p_Matrix._2_3 - p_Matrix._2_2 * p_Matrix._1_3);
}
/*********************************************************************************/
K15_Vector2 K15_GetTransformedVector(K15_Vector2& p_Vector, K15_Matrix3& p_Matrix)
{
	K15_Vector3 transformedVector = {};
	
	transformedVector.x = p_Vector.x;
	transformedVector.y = p_Vector.y;
	transformedVector.z = 1.f;

	transformedVector = K15_GetTransformedVector(transformedVector, p_Matrix);

	K15_Vector2 transformedVector2 = {};

	transformedVector2.x = transformedVector.x;
	transformedVector2.y = transformedVector.y;

	return transformedVector2;
}
/*********************************************************************************/
K15_Vector3 K15_GetTransformedVector(K15_Vector3& p_Vector, K15_Matrix3& p_Matrix)
{
	K15_Vector3 transformedVector = {};

	transformedVector.x = p_Matrix._1_1 * p_Vector.x + p_Matrix._2_1 * p_Vector.y + p_Matrix._3_1 * p_Vector.z;
	transformedVector.y = p_Matrix._1_2 * p_Vector.x + p_Matrix._2_2 * p_Vector.y + p_Matrix._3_2 * p_Vector.z;
	transformedVector.z = p_Matrix._1_3 * p_Vector.x + p_Matrix._2_3 * p_Vector.y + p_Matrix._3_3 * p_Vector.z;

	return transformedVector;
}
/*********************************************************************************/
K15_Vector3 K15_GetXAxis(K15_Matrix3& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_1;
	axis.y = p_Matrix._2_1;
	axis.z = p_Matrix._3_1;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetYAxis(K15_Matrix3& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_2;
	axis.y = p_Matrix._2_2;
	axis.z = p_Matrix._3_2;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetZAxis(K15_Matrix3& p_Matrix)
{
	K15_Vector3 axis = {};

	axis.x = p_Matrix._1_3;
	axis.y = p_Matrix._2_3;
	axis.z = p_Matrix._3_3;

	return axis;
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedXAxis(K15_Matrix3& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetXAxis(p_Matrix));
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedYAxis(K15_Matrix3& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetYAxis(p_Matrix));
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedZAxis(K15_Matrix3& p_Matrix)
{
	return K15_GetNormalizedVector(K15_GetZAxis(p_Matrix));
}
/*********************************************************************************/
K15_Matrix3 operator*(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2)
{
	K15_Matrix3 matrixProduct = {};

	matrixProduct._1_1 = p_Matrix1._1_1 * p_Matrix2._1_1 + p_Matrix1._1_2 * p_Matrix2._2_1 + p_Matrix1._1_3 * p_Matrix2._3_1;
	matrixProduct._1_2 = p_Matrix1._1_1 * p_Matrix2._1_2 + p_Matrix1._1_2 * p_Matrix2._2_2 + p_Matrix1._1_3 * p_Matrix2._3_2;
	matrixProduct._1_3 = p_Matrix1._1_1 * p_Matrix2._1_3 + p_Matrix1._1_2 * p_Matrix2._2_3 + p_Matrix1._1_3 * p_Matrix2._3_3;

	matrixProduct._2_1 = p_Matrix1._2_1 * p_Matrix2._1_1 + p_Matrix1._2_2 * p_Matrix2._2_1 + p_Matrix1._2_3 * p_Matrix2._3_1;
	matrixProduct._2_2 = p_Matrix1._2_1 * p_Matrix2._1_2 + p_Matrix1._2_2 * p_Matrix2._2_2 + p_Matrix1._2_3 * p_Matrix2._3_2;
	matrixProduct._2_3 = p_Matrix1._2_1 * p_Matrix2._1_3 + p_Matrix1._2_2 * p_Matrix2._2_3 + p_Matrix1._2_3 * p_Matrix2._3_3;

	matrixProduct._3_1 = p_Matrix1._3_1 * p_Matrix2._1_1 + p_Matrix1._3_2 * p_Matrix2._2_1 + p_Matrix1._3_3 * p_Matrix2._3_1;
	matrixProduct._3_2 = p_Matrix1._3_1 * p_Matrix2._1_2 + p_Matrix1._3_2 * p_Matrix2._2_2 + p_Matrix1._3_3 * p_Matrix2._3_2;
	matrixProduct._3_3 = p_Matrix1._3_1 * p_Matrix2._1_3 + p_Matrix1._3_2 * p_Matrix2._2_3 + p_Matrix1._3_3 * p_Matrix2._3_3;

	return matrixProduct;
}
/*********************************************************************************/
K15_Matrix3 operator*(K15_Matrix3& p_Matrix1, real32 p_Scalar)
{
	K15_Matrix3 matrixProduct = {};

	matrixProduct._1_1 = p_Matrix1._1_1 * p_Scalar;
	matrixProduct._1_2 = p_Matrix1._1_2 * p_Scalar;
	matrixProduct._1_3 = p_Matrix1._1_3 * p_Scalar;

	matrixProduct._2_1 = p_Matrix1._2_1 * p_Scalar;
	matrixProduct._2_2 = p_Matrix1._2_2 * p_Scalar;
	matrixProduct._2_3 = p_Matrix1._2_3 * p_Scalar;

	matrixProduct._3_1 = p_Matrix1._3_1 * p_Scalar;
	matrixProduct._3_2 = p_Matrix1._3_2 * p_Scalar;
	matrixProduct._3_3 = p_Matrix1._3_3 * p_Scalar;

	return matrixProduct;
}
/*********************************************************************************/
bool operator==(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2)
{
	return memcmp(p_Matrix1.m, p_Matrix2.m, sizeof(p_Matrix1.m)) == 0;
}
/*********************************************************************************/
bool operator!=(K15_Matrix3& p_Matrix1, K15_Matrix3& p_Matrix2)
{
	return memcmp(p_Matrix1.m, p_Matrix2.m, sizeof(p_Matrix1.m)) != 0;
}
/*********************************************************************************/