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

#ifndef __K15_MATRIX4X4__
#define __K15_MATRIX4X4__

#include "K15_Vector3.h"
#include "K15_Matrix3x3.h"
#include <cassert>


namespace K15_Math
{

	template <class Real>
	class Matrix4x4
	{
	public:
		enum MatrixFlags{
			MF_ZERO,				
			MF_IDENT																	
		};

		Matrix4x4(MatrixFlags flag = MF_ZERO);
		Matrix4x4(Real mat[16]);
		Matrix4x4(Real _1_1,Real _1_2,Real _1_3,Real _1_4,
				  Real _2_1,Real _2_2,Real _2_3,Real _2_4,
				  Real _3_1,Real _3_2,Real _3_3,Real _3_4,
				  Real _4_1,Real _4_2,Real _4_3,Real _4_4);
		Matrix4x4(const Matrix4x4<Real> &mat);

		~Matrix4x4();
		
		void Identity();
		void Zero();

		Matrix4x4<Real> ColumnChange(Matrix4x4<Real> &mat, int column1, int column2);
		Matrix4x4<Real> RowChange(Matrix4x4<Real> &mat, int row1, int row2);

		Real Determinant() const;
		void SubMat(Matrix4x4<Real> mat4, Matrix3x3<Real> &mat3, int row, int column) const;

		void Inverse();
		void Transpose();

		bool IsTriangle() const;
		bool IsLowerTriangle() const;
		bool IsUpperTriangle() const;
		bool IsIdentity() const;
		bool IsZero() const;
		bool HasZeroVector() const;

		const Matrix4x4<Real> &operator*(const Matrix4x4<Real> &mat);
		Vector3<Real> operator*(const Vector3<Real> &vec);
		const Matrix4x4<Real> &operator*(Real scalar);

		void operator*=(const Matrix4x4<Real> &mat);
		void operator*=(Real fScalar);

		Matrix4x4<Real> &operator+(const Matrix4x4<Real> &mat);
		Matrix4x4<Real> &operator-(const Matrix4x4<Real> &mat);
		Matrix4x4<Real> &operator-();
		Matrix4x4<Real> &operator=(const Matrix4x4<Real> &mat);

		Matrix4x4<Real> &operator+=(const Matrix4x4<Real> &mat);
		Matrix4x4<Real> &operator-=(const Matrix4x4<Real> &mat);

		bool operator==(const Matrix4x4<Real> &mat);
		bool operator!=(const Matrix4x4<Real> &mat);

		static void RotationMatrixFromEuler(Matrix4x4<Real> &mat, Vector3<Real> &eulerAngles);

		static Matrix4x4<Real> PerspectiveLH(float fieldOfViewY,float aspectRatio,float zNear,float zFar);
		static Matrix4x4<Real> PerspectiveRH(float fieldOfViewY,float aspectRatio,float zNear,float zFar);

		static const Matrix4x4<Real> IdentityMatrix;
	public:

		union{
			struct {
				Real _1_1,_1_2,_1_3,_1_4;
				Real _2_1,_2_2,_2_3,_2_4;
				Real _3_1,_3_2,_3_3,_3_4;
				Real _4_1,_4_2,_4_3,_4_4;
			};
			Real m_fMatrix[16];
		};
	};
	typedef Matrix4x4<float> Matrix4x4F;
	typedef Matrix4x4<double> Matrix4x4D;

	#include "../src/K15_Matrix4x4.inl"
}

#endif //__K15_MATRIX4X4__