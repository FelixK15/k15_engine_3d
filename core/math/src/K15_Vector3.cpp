#include "K15_Vector3.h"

/*********************************************************************************/
real32 K15_GetVectorDotProduct(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	return p_Vector1.x * p_Vector2.x + p_Vector1.y * p_Vector2.y + p_Vector1.z * p_Vector2.z;
}
/*********************************************************************************/
real32 K15_GetVectorLength(K15_Vector3& p_Vector)
{
	real32 squaredLength = K15_GetVectorSquaredLength(p_Vector);
	return (real32)::sqrt(squaredLength);
}
/*********************************************************************************/
real32 K15_GetVectorSquaredLength(K15_Vector3& p_Vector)
{
	return p_Vector.x * p_Vector.x + p_Vector.y * p_Vector.y + p_Vector.z * p_Vector.z;
}
/*********************************************************************************/
K15_Vector3 K15_CreateVector(real32 p_ComponentX, real32 p_ComponentY, real32 p_ComponentZ)
{
	K15_Vector3 vec3 = {};

	return K15_InitializeVector(vec3, p_ComponentX, p_ComponentY, p_ComponentZ);
}
/*********************************************************************************/
K15_Vector3 K15_InitializeVector(K15_Vector3& p_Vector, real32 p_ComponentX, real32 p_ComponentY, real32 p_ComponentZ)
{
	p_Vector.x = p_ComponentX;
	p_Vector.y = p_ComponentY;
	p_Vector.z = p_ComponentZ;

	return p_Vector;
}
/*********************************************************************************/
K15_Vector3 K15_GetCrossProduct(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	K15_Vector3 crossProduct = {};

	//Cx = AyBz - AzBy
	//Cy = AzBx - AxBz
	//Cz = AxBy - AyBx

	crossProduct.x = p_Vector1.y * p_Vector2.z - p_Vector1.z * p_Vector2.y;
	crossProduct.y = p_Vector1.z * p_Vector2.x - p_Vector1.x * p_Vector2.z;
	crossProduct.z = p_Vector1.x * p_Vector2.y - p_Vector1.y * p_Vector2.x;

	return crossProduct;
}
/*********************************************************************************/
K15_Vector3 K15_GetProjectedVector(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	real32 dotProduct = K15_GetVectorDotProduct(p_Vector1, p_Vector2);

	return p_Vector1 * dotProduct;
}
/*********************************************************************************/
K15_Vector3 K15_GetNormalizedVector(K15_Vector3& p_Vector)
{
	real32 length = K15_GetVectorLength(p_Vector);

	K15_Vector3 normalizedVector = {};

	normalizedVector.x = p_Vector.x / length;
	normalizedVector.y = p_Vector.y / length;
	normalizedVector.z = p_Vector.z / length;

	return normalizedVector;
}
/*********************************************************************************/
void K15_NormalizeVector(K15_Vector3& p_Vector)
{
	p_Vector = K15_GetNormalizedVector(p_Vector);
}
/*********************************************************************************/
K15_Vector3 operator+(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	K15_Vector3 vectorSum = {};

	vectorSum.x = p_Vector1.x + p_Vector2.x;
	vectorSum.y = p_Vector1.y + p_Vector2.y;
	vectorSum.z = p_Vector1.z + p_Vector2.z;

	return vectorSum;
}
/*********************************************************************************/
K15_Vector3 operator-(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	K15_Vector3 vectorDifference = {};

	vectorDifference.x = p_Vector1.x - p_Vector2.x;
	vectorDifference.y = p_Vector1.y - p_Vector2.y;
	vectorDifference.z = p_Vector1.z - p_Vector2.z;

	return vectorDifference;
}
/*********************************************************************************/
K15_Vector3 operator*(K15_Vector3& p_Vector, real32 p_Scalar)
{
	K15_Vector3 scaledVector = {};

	scaledVector.x = p_Vector.x * p_Scalar;
	scaledVector.y = p_Vector.y * p_Scalar;
	scaledVector.z = p_Vector.z * p_Scalar;

	return scaledVector;
}
/*********************************************************************************/
K15_Vector3 operator*(real32 p_Scalar, K15_Vector3& p_Vector)
{
	return p_Vector * p_Scalar;
}
/*********************************************************************************/
bool operator==(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	return (p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y) && (p_Vector1.z == p_Vector2.z);
}
/*********************************************************************************/
bool operator!=(K15_Vector3& p_Vector1, K15_Vector3& p_Vector2)
{
	return !((p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y) && (p_Vector1.z == p_Vector2.z));
}
/*********************************************************************************/