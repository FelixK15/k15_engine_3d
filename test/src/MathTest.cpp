#include "UnitTest++.h"
#include "K15_Vector3.h"
#include "K15_Matrix4x4.h"
#include "K15_Matrix3x3.h"
#include "K15_Quaternion.h"
#include "K15_Mathematic.h"

using namespace K15_Math;

SUITE(Vector3Tests)
{
	Vector3<float> TestVec1;
	Vector3<float> TestVec2;

	TEST(SimpleOperations)
	{
		TestVec1 = Vector3<float>(1.f,1.f,1.f);
		TestVec2 = Vector3<float>(2.f,2.f,2.f);
		TestVec1 += TestVec2;
		CHECK_EQUAL(TestVec1 == Vector3<float>(3.f,3.f,3.f), true);
		CHECK_EQUAL((TestVec1 * TestVec2) == 18.f,true);
		CHECK_EQUAL(TestVec1 * 3.f == Vector3F(9.f,9.f,9.f), true);
		TestVec1 = Vector3<float>(1.f,2.f,2.f);
		CHECK_EQUAL(TestVec1.Magnitude() == 3.f, true);
		TestVec1.Normalize();
		CHECK_EQUAL(TestVec1 == Vector3<float>(1.f/3.f,2.f/3.f,2.f/3.f), true);
		CHECK_EQUAL(TestVec1 < TestVec2, true);
		TestVec1 = Vector3<float>(1.f,2.f,2.f);
		TestVec1.Invert();
		CHECK_EQUAL(TestVec1 == Vector3<float>(-1.f,-2.f,-2.f), true);
		CHECK_EQUAL(TestVec1.IsNull()== false, true);
		TestVec1.Invert();
		CHECK_EQUAL(TestVec1.Cross(TestVec2) == Vector3<float>(0,2.f,-2.f), true);
	}

	TEST(UnitTests)
	{
		TestVec1 = Vector3<float>(0,1.f,0);
		CHECK_EQUAL(TestVec1.Magnitude(), 1.f);
		TestVec1 = Vector3<float>(1.f,1.f,1.f);
		CHECK_EQUAL(TestVec1.Magnitude() == 1.f, false);
		TestVec1.Normalize();
		CHECK_CLOSE(TestVec1.Magnitude(), 1.f, 0.2f);
	}

}

SUITE(Matrix4x4Tests)
{
	Matrix4x4<double> TestMat1;
	Matrix4x4<double> TestMat2;
	Matrix3x3<double> TestMat3x3;

	TEST(SimpleOperations)
	{
		TestMat1.Identity();
		TestMat2.Zero();
		CHECK_EQUAL(TestMat2 == Matrix4x4<double>(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0), true);
		CHECK_EQUAL(TestMat1 == Matrix4x4<double>(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), true);
		CHECK_EQUAL(TestMat1.HasZeroVector(), false);
		CHECK_EQUAL(TestMat2.HasZeroVector(), true);
		CHECK_EQUAL(TestMat1.IsIdentity(), true);
		CHECK_EQUAL(TestMat2.IsIdentity(), false);
		TestMat1 = Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8);
		CHECK_EQUAL(TestMat1.HasZeroVector(), false);
		CHECK_EQUAL(TestMat1.IsTriangle(), false);
	}

	TEST(ChangeTest)
	{
		TestMat1.Identity();
		CHECK_EQUAL(TestMat1.ColumnChange(TestMat1, 0, 1) == Matrix4x4<double>(0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1), true);
		TestMat1.ColumnChange(TestMat1, 0, 1);
		CHECK_EQUAL(TestMat1.ColumnChange(TestMat1, 3, 1) == Matrix4x4<double>(1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0), true);
		TestMat1.ColumnChange(TestMat1, 3, 1);
		CHECK_EQUAL(TestMat1.RowChange(TestMat1, 0, 1) == Matrix4x4<double>(0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1), true);
		TestMat1.RowChange(TestMat1, 0, 1);
		CHECK_EQUAL(TestMat1.RowChange(TestMat1, 3, 1) == Matrix4x4<double>(1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0), true);
	}

	TEST(SubMatTest)
	{
		TestMat1 = Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8);
		TestMat1.SubMat(TestMat1, TestMat3x3, 0, 0);
		CHECK_EQUAL(TestMat3x3 == Matrix3x3<double>(2,11,9,-33,-1,3,0,-7,8), true);
		TestMat1.SubMat(TestMat1, TestMat3x3, 1, 2);
		CHECK_EQUAL(TestMat3x3 == Matrix3x3<double>(3,6,4,7,11,9,6,-7,8), true);
	}
	TEST(SpecificTest)
	{
		TestMat1.Identity();
		CHECK_EQUAL(TestMat1.Determinant() == 1, true);
		TestMat1 = Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8);
		CHECK_EQUAL(TestMat1.Determinant() == 23182, true);
		TestMat1 = Matrix4x4<double>(1,1,1,0,1,0,0,0,1,0,1,0,1,0,0,1);
		CHECK_EQUAL(TestMat1.Determinant() == -1, true);
	}
	TEST(InvertTest)
	{
		TestMat2.Identity();
		TestMat2.Inverse();
		TestMat1 = Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8);
		TestMat1.Inverse();
		CHECK_EQUAL(TestMat2.IsIdentity(), true);
		CHECK_ARRAY_CLOSE(TestMat1.m_fMatrix, Matrix4x4<double>(0.22, -0.11, 0.05, 0, 0.13, -0.06, 0, 0, 0, 0.06, 0, -0.06, -0.18, 0.14, 0, 0.07).m_fMatrix, 16, 0.5);
		TestMat1.Inverse();
		CHECK_ARRAY_CLOSE(TestMat1.m_fMatrix, Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8).m_fMatrix, 16, 0.5);
	}
	TEST(TransposeTest)
	{
		TestMat1.Identity();
		TestMat2.Identity();
		TestMat2.Transpose();
		CHECK_EQUAL(TestMat2 == TestMat1, true);
		TestMat1 = Matrix4x4<double>(3,9,6,4,7,2,11,9,22,-33,-1,3,6,0,-7,8);
		TestMat1.Transpose();
		CHECK_EQUAL(TestMat1 == Matrix4x4<double>(3, 7, 22, 6, 9, 2, -33, 0, 6, 11, -1, -7, 4, 9, 3, 8), true);
		TestMat2 = Matrix4x4<double>(1, 2, 3, 4, 4, 3, 2, 1, 0, -1, 2, -3, 1, 2, 3, 4);
		TestMat1 *= TestMat2;
		CHECK_EQUAL(TestMat1 == Matrix4x4<double>(37, 17, 85, -23, 17, 57, -35, 137, 43, 32, 17, 10, 48, 48, 60, 48), true);
	}
	TEST(FunctionTest)
	{

	}
}

SUITE(Matrix3x3Tests)
{
	Matrix3x3<double> TestMat1;
	Matrix3x3<double> TestMat2;

	TEST(SimpleOperations)
	{
		TestMat1.Identity();
		CHECK_EQUAL(TestMat1.IsIdentity(), true);
	}
}

SUITE(QuaternionTest)
{
	Quaternion<double> TestQuat1;
	Quaternion<double> TestQuat2;
	Vector3<double> TestVec1;

	TEST(SimpleOperations)
	{
		TestQuat1 = Quaternion<double>(1,1,1,1); 
		TestQuat2 = Quaternion<double>(2,3,2,1);
		CHECK_EQUAL(TestQuat1 * TestQuat2 == Quaternion<double>(-4,4,6,2), true);
		TestQuat1 = Quaternion<double>(2,3,2,1);
		TestQuat1.Conjugate();
		CHECK_EQUAL(TestQuat1 * TestQuat2 == Quaternion<double>(18,0,0,0), true);
	}

	TEST(MagnitudeTest)
	{
		TestQuat1 = Quaternion<double>(1,1,1,1);
		CHECK_EQUAL(TestQuat1.Magnitude(), Mathematic<double>::Sqrt(4));
	}

	TEST(BooleanTests)
	{
		TestQuat1 = Quaternion<double>(1,1,1,1); 
		CHECK_EQUAL(TestQuat1.IsUnitQuaternion(), false);
		TestQuat1.Normalize();
		CHECK_EQUAL(TestQuat1.IsUnitQuaternion(), true);
	}

	TEST(ConvertionTests)
	{
		Matrix4x4<double> TestMat;
		TestQuat1 = Quaternion<double>(1,1,1,1); 
		TestQuat1.QuaternionToMatrix(TestQuat1, &TestMat);
		CHECK_EQUAL(TestMat == Matrix4x4<double>(-3,4,0,0,0,-3,4,0,4,0,-3,0,0,0,0,1), true);
		TestQuat1.AxisAngleToQuaternion(1,1,1,1);

	}
}