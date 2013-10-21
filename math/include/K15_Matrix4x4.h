/**
 * @file K15_Matrix4x4.h
 * @author	Tobias Funke <t.funke@k15games.de>
 * @version 1.0
 * @date 2012/07/12
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 *							<--- PLEASE PUT A DESCRIPTION HERE
 */
#pragma once

#ifndef _K15Engine_Math_Matrix4x4_h_
#define _K15Engine_Math_Matrix4x4_h_

#include "K15_Vector3.h"
#include "K15_Matrix3x3.h"

namespace K15_Engine { namespace Math {
	class Matrix4x4
	{
	public:
		enum MatrixFlags{
			MF_ZERO,				
			MF_IDENT																	
		};

		Matrix4x4(MatrixFlags p_Flag = MF_ZERO);
		Matrix4x4(float p_Values[16]);
		Matrix4x4(float _1_1,float _1_2,float _1_3,float _1_4,
				  float _2_1,float _2_2,float _2_3,float _2_4,
				  float _3_1,float _3_2,float _3_3,float _3_4,
				  float _4_1,float _4_2,float _4_3,float _4_4);
		Matrix4x4(const Matrix4x4 &p_Other);

		~Matrix4x4();
		
		void Identity();
		void Zero();

		Matrix4x4 ColumnChange(Matrix4x4& p_Matrix, int p_Column1, int p_Column2);
		Matrix4x4 RowChange(Matrix4x4& p_Matrix, int p_Row1, int p_Row2);

		float Determinant() const;
		void subMat(Matrix4x4 p_Mat, Matrix3x3& p_Matrix, int p_Row, int p_Column) const;

		void Inverse();
		void Transpose();

		bool IsTriangle() const;
		bool IsLowerTriangle() const;
		bool IsUpperTriangle() const;
		bool IsIdentity() const;
		bool IsZero() const;
		bool HasZeroVector() const;

		const Matrix4x4& operator*(const Matrix4x4& p_Matrix);
		Vector3 operator*(const Vector3& p_Vector);
		const Matrix4x4& operator*(float p_Scalar);

		void operator*=(const Matrix4x4& p_Matrix);
		void operator*=(float p_Scalar);

		Matrix4x4& operator+(const Matrix4x4& p_Matrix);
		Matrix4x4& operator-(const Matrix4x4& p_Matrix);
		Matrix4x4& operator-();
		Matrix4x4& operator=(const Matrix4x4& p_Matrix);

		Matrix4x4& operator+=(const Matrix4x4& p_Matrix);
		Matrix4x4& operator-=(const Matrix4x4& p_Matrix);

		bool operator==(const Matrix4x4& p_Matrix);
		bool operator!=(const Matrix4x4& p_Matrix);

		static void rotationMatrixFromEuler(Matrix4x4& p_Matrix, Vector3& p_EulerAngles);

		static Matrix4x4 PerspectiveLH(float fieldOfViewY,float aspectRatio,float zNear,float zFar);
		static Matrix4x4 PerspectiveRH(float fieldOfViewY,float aspectRatio,float zNear,float zFar);

		static const Matrix4x4 IdentityMatrix;
	public:
		union
		{
			struct 
			{
				float _1_1,_1_2,_1_3,_1_4;
				float _2_1,_2_2,_2_3,_2_4;
				float _3_1,_3_2,_3_3,_3_4;
				float _4_1,_4_2,_4_3,_4_4;
			};// struct
			float m_fMatrix[16];
		};// union
	};// end of Matrix4x4 class
	#include "K15_Matrix4x4.inl"
 }}//end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Matrix4x4_h_