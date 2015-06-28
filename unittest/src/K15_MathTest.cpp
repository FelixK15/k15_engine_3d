#include "K15_Vector2.h"
#include "K15_Vector3.h"

#include "K15_Matrix3.h"
#include "K15_Math.h"

/*********************************************************************************/
real32 K15_NormalizedVector2LengthTest()
{
	K15_Vector2 normalizedVector2 = {};
	normalizedVector2.x = 1.f;
	normalizedVector2.y = 1.f;

	K15_NormalizeVector(normalizedVector2);

	return K15_GetVectorLength(normalizedVector2);	
}
/*********************************************************************************/
real32 K15_DotProductVector2Test()
{
	K15_Vector2 vectorA = {};
	K15_Vector2 vectorB = {};

	vectorA.x = 1.0f;
	vectorA.y = 0.0f;

	vectorB.x = 0.0f;
	vectorB.y = 1.0f;

	return K15_GetVectorDotProduct(vectorA, vectorB);
}
/*********************************************************************************/
real32 K15_AngleVector2Test()
{
	K15_Vector2 vectorA = {};
	K15_Vector2 vectorB = {};

	vectorA.x = 1.0f;
	vectorA.y = 0.0f;

	vectorB.x = 0.5f;
	vectorB.y = 0.5f;

	return K15_GetVectorAngle(vectorA, vectorB);
}
/*********************************************************************************/



/*********************************************************************************/
real32 K15_NormalizedVector3LengthTest()
{
	K15_Vector3 normalizedVector = {};
	normalizedVector.x = 1.f;
	normalizedVector.y = 1.f;
	normalizedVector.z = 1.f;

	K15_NormalizeVector(normalizedVector);

	return K15_GetVectorLength(normalizedVector);	
}
/*********************************************************************************/
real32 K15_DotProductVector3Test()
{
	K15_Vector3 vectorA = {};
	K15_Vector3 vectorB = {};

	vectorA.x = 1.0f;
	vectorA.y = 0.0f;
	vectorA.z = 0.0f;

	vectorB.x = 0.0f;
	vectorB.y = 0.0f;
	vectorB.z = 1.0f;

	return K15_GetVectorDotProduct(vectorA, vectorB);
}
/*********************************************************************************/


/*********************************************************************************/
bool8 K15_Vector2Test()
{
	K15_START_TESTS("Vector2 Test");
		K15_TEST_FLOAT_FUNCTION_APPROX(K15_NormalizedVector2LengthTest, 1.0f, 0.01f);
		K15_TEST_FLOAT_FUNCTION_APPROX(K15_DotProductVector2Test, 0.0f, 0.01f);
		K15_TEST_FLOAT_FUNCTION_APPROX(K15_AngleVector2Test, K15_QUATER_PI, 0.01f);
	K15_END_TESTS;

	return K15_TEST_RESULT;
}
/*********************************************************************************/
bool8 K15_Vector3Test()
{
	K15_START_TESTS("Vector3 Test");
		K15_TEST_FLOAT_FUNCTION_APPROX(K15_NormalizedVector3LengthTest, 1.0f, 0.01f);
		K15_TEST_FLOAT_FUNCTION_APPROX(K15_DotProductVector3Test, 1.0f, 0.01f);
	K15_END_TESTS;

	return K15_TEST_RESULT;
}
/*********************************************************************************/