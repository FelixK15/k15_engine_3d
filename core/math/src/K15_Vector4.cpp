#include "K15_Vector4.h"

/*********************************************************************************/
real32 K15_GetVectorDotProduct(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	return p_Vector1.x * p_Vector2.x + p_Vector1.y * p_Vector2.y + p_Vector1.z * p_Vector2.z + p_Vector1.w * p_Vector2.w;
}
/*********************************************************************************/
real32 K15_GetVectorLength(K15_Vector4& p_Vector)
{
	real32 squaredLength = K15_GetVectorSquaredLength(p_Vector);
	return (real32)::sqrt(squaredLength);
}
/*********************************************************************************/
real32 K15_GetVectorSquaredLength(K15_Vector4& p_Vector)
{
	return p_Vector.x * p_Vector.x + p_Vector.y * p_Vector.y + p_Vector.z * p_Vector.z + p_Vector.w * p_Vector.w;
}
/*********************************************************************************/
K15_Vector4 K15_CreateVector(real32 p_ComponentX, real32 p_ComponentY, real32 p_ComponentZ, real32 p_ComponentW)
{
	K15_Vector4 vec = { p_ComponentX, p_ComponentY, p_ComponentZ, p_ComponentW };
	return vec;
}
/*********************************************************************************/
K15_Vector4 K15_GetCrossProduct(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	K15_Vector4 crossProduct = {};

	//Cx = AyBz - AzBy
	//Cy = AzBx - AxBz
	//Cz = AxBy - AyBx

	crossProduct.x = p_Vector1.y * p_Vector2.z - p_Vector1.z * p_Vector2.y;
	crossProduct.y = p_Vector1.z * p_Vector2.x - p_Vector1.x * p_Vector2.z;
	crossProduct.z = p_Vector1.x * p_Vector2.y - p_Vector1.y * p_Vector2.x;
	crossProduct.w = 0.0f;

	return crossProduct;
}
/*********************************************************************************/
K15_Vector4 K15_GetProjectedVector(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	real32 dotProduct = K15_GetVectorDotProduct(p_Vector1, p_Vector2);

	return p_Vector1 * dotProduct;
}
/*********************************************************************************/
K15_Vector4 K15_GetNormalizedVector(K15_Vector4& p_Vector)
{
	real32 length = K15_GetVectorLength(p_Vector);

	K15_Vector4 normalizedVector = {};

	normalizedVector.x = p_Vector.x / length;
	normalizedVector.y = p_Vector.y / length;
	normalizedVector.z = p_Vector.z / length;
	normalizedVector.w = p_Vector.w / length;

	return normalizedVector;
}
/*********************************************************************************/
void K15_NormalizeVector(K15_Vector4& p_Vector)
{
	p_Vector = K15_GetNormalizedVector(p_Vector);
}
/*********************************************************************************/
K15_Vector4 operator+(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	K15_Vector4 vectorSum = {};

	vectorSum.x = p_Vector1.x + p_Vector2.x;
	vectorSum.y = p_Vector1.y + p_Vector2.y;
	vectorSum.z = p_Vector1.z + p_Vector2.z;
	vectorSum.w = p_Vector1.w + p_Vector2.w;

	return vectorSum;
}
/*********************************************************************************/
K15_Vector4 operator-(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	K15_Vector4 vectorDifference = {};

	vectorDifference.x = p_Vector1.x - p_Vector2.x;
	vectorDifference.y = p_Vector1.y - p_Vector2.y;
	vectorDifference.z = p_Vector1.z - p_Vector2.z;
	vectorDifference.w = p_Vector1.w - p_Vector2.w;

	return vectorDifference;
}
/*********************************************************************************/
K15_Vector4 operator*(K15_Vector4& p_Vector, real32 p_Scalar)
{
	K15_Vector4 scaledVector = {};

	scaledVector.x = p_Vector.x * p_Scalar;
	scaledVector.y = p_Vector.y * p_Scalar;
	scaledVector.z = p_Vector.z * p_Scalar;
	scaledVector.w = p_Vector.w * p_Scalar;

	return scaledVector;
}
/*********************************************************************************/
K15_Vector4 operator*(real32 p_Scalar, K15_Vector4& p_Vector)
{
	return p_Vector * p_Scalar;
}
/*********************************************************************************/
K15_Vector4 operator/(K15_Vector4& p_Vector, real32 p_Scalar)
{
	K15_Vector4 scaledVector = {};

	scaledVector.x = p_Vector.x / p_Scalar;
	scaledVector.y = p_Vector.y / p_Scalar;
	scaledVector.z = p_Vector.z / p_Scalar;
	scaledVector.w = p_Vector.w / p_Scalar;

	return scaledVector;
}
/*********************************************************************************/
bool operator==(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	return (p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y) && (p_Vector1.z == p_Vector2.z) && (p_Vector1.w == p_Vector2.w);
}
/*********************************************************************************/
bool operator!=(K15_Vector4& p_Vector1, K15_Vector4& p_Vector2)
{
	return !((p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y) && (p_Vector1.z == p_Vector2.z) && (p_Vector1.w == p_Vector2.w));
}
/*********************************************************************************/
K15_Vector4 K15_UnpackVector4(uint32 p_PackedVector)
{
	K15_Vector4 vec = {};

	vec.w = (float)(unsigned char)(p_PackedVector >> 24);
	vec.x = (float)(unsigned char)(p_PackedVector >> 16);
	vec.y = (float)(unsigned char)(p_PackedVector >> 8);
	vec.z = (float)(unsigned char)(p_PackedVector >> 0);

	return vec;
}
/*********************************************************************************/
uint32 K15_PackVector4(const K15_Vector4& p_UnpackedVector)
{
	uint32 packedVector = (((unsigned char)(p_UnpackedVector.w) << 24) |
						   ((unsigned char)(p_UnpackedVector.x) << 16) |
						   ((unsigned char)(p_UnpackedVector.y) << 8)  |
						   ((unsigned char)(p_UnpackedVector.z) << 0));

	return packedVector;
}
/*********************************************************************************/
