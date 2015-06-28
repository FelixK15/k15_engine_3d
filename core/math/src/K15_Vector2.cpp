#include "K15_Vector2.h"

/*********************************************************************************/
real32 K15_GetVectorDotProduct(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	return p_Vector1.x * p_Vector2.x + p_Vector1.y * p_Vector2.y;
}
/*********************************************************************************/
real32 K15_GetVectorAngle(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	real32 v1Length = K15_GetVectorLength(p_Vector1);
	real32 v2Length = K15_GetVectorLength(p_Vector2);
	real32 dotProduct = K15_GetVectorDotProduct(p_Vector1, p_Vector2);

	return ::acos(dotProduct / (v1Length * v2Length));
}
/*********************************************************************************/
real32 K15_GetVectorLength(K15_Vector2& p_Vector)
{
	real32 squaredLength = K15_GetVectorSquaredLength(p_Vector);
	return (real32)::sqrt(squaredLength);
}
/*********************************************************************************/
real32 K15_GetVectorSquaredLength(K15_Vector2& p_Vector)
{
	return p_Vector.x * p_Vector.x + p_Vector.y * p_Vector.y;
}
/*********************************************************************************/
K15_Vector2 K15_CreateVector(real32 p_ComponentX, real32 p_ComponentY)
{
	K15_Vector2 vec2 = {};

	return K15_InitializeVector(vec2, p_ComponentX, p_ComponentY);
}
/*********************************************************************************/
K15_Vector2 K15_InitializeVector(K15_Vector2& p_Vector, real32 p_ComponentX, real32 p_ComponentY)
{
	p_Vector.x = p_ComponentX;
	p_Vector.y = p_ComponentY;

	return p_Vector;
}
/*********************************************************************************/
K15_Vector2 K15_GetProjectedVector(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	real32 dotProduct = K15_GetVectorDotProduct(p_Vector1, p_Vector2);

	return p_Vector1 * dotProduct;
}
/*********************************************************************************/
K15_Vector2 K15_GetNormalizedVector(K15_Vector2& p_Vector)
{
	real32 vectorLength = K15_GetVectorLength(p_Vector);
	K15_Vector2 normalizedVector = {};

	normalizedVector.x = p_Vector.x /= vectorLength;
	normalizedVector.y = p_Vector.y /= vectorLength;

	return normalizedVector;
}
/*********************************************************************************/
void K15_NormalizeVector(K15_Vector2& p_Vector)
{
	p_Vector = K15_GetNormalizedVector(p_Vector);
}
/*********************************************************************************/
K15_Vector2 operator+(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	K15_Vector2 vectorSum = {};

	vectorSum.x = p_Vector1.x + p_Vector2.x;
	vectorSum.y = p_Vector1.y + p_Vector2.y;

	return vectorSum;
}
/*********************************************************************************/
K15_Vector2 operator-(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	K15_Vector2 vectorDifference = {};

	vectorDifference.x = p_Vector1.x - p_Vector2.x;
	vectorDifference.y = p_Vector1.y - p_Vector2.y;

	return vectorDifference;
}
/*********************************************************************************/
K15_Vector2 operator*(K15_Vector2& p_Vector, real32 p_Scalar)
{
	K15_Vector2 scaledVector = {};

	scaledVector.x = p_Vector.x * p_Scalar;
	scaledVector.y = p_Vector.y * p_Scalar;

	return scaledVector;
}
/*********************************************************************************/
K15_Vector2 operator*(real32 p_Scalar, K15_Vector2& p_Vector)
{
	return p_Vector * p_Scalar;
}
/*********************************************************************************/
bool operator==(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	return (p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y);
}
/*********************************************************************************/
bool operator!=(K15_Vector2& p_Vector1, K15_Vector2& p_Vector2)
{
	return !((p_Vector1.x == p_Vector2.x) && (p_Vector1.y == p_Vector2.y));
}
/*********************************************************************************/