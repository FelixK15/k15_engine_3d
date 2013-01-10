/**
 * @file K15_Matrix3x3.h
 * @author  Tobias Funke <t.funke@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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
 * 
 */
#pragma once

#ifndef __K15_MATRIX3X3__
#define __K15_MATRIX3X3__

#include "K15_Vector3.h"
#include "K15_Mathematic.h"

namespace K15_Math
{
	template <class Real>
	class Matrix3x3
	{
	public:
		enum MatrixFlags{
			MF_ZERO,				
			MF_IDENT,						
		};

		Matrix3x3(MatrixFlags flag = MF_IDENT);
		Matrix3x3(Real mat[9]);
		Matrix3x3(Real _1_1,Real _1_2,Real _1_3,
			Real _2_1,Real _2_2,Real _2_3,
			Real _3_1,Real _3_2,Real _3_3);
		Matrix3x3(const Matrix3x3<Real> &mat);

		~Matrix3x3();

		void Identity();
		void Zero();

		Matrix3x3<Real> ColumnChange(Matrix3x3<Real> &mat, int column1, int column2);
		Matrix3x3<Real> RowChange(Matrix3x3<Real> &mat, int row1, int row2);

		Real Determinant() const;

		void Inverse();
		void Transpose();
		void Rotate(const Vector3<Real> &vec,Real angleInRad);
		void Scale(const Vector3<Real> &scaleVec);
		void Translate(const Vector3<Real> &posVec);

		bool IsTriangle() const;
		bool IsLowerTriangle() const;
		bool IsUpperTriangle() const;
		bool IsIdentity() const;
		bool IsZero() const;
		bool HasZeroVector() const;
		int GetMaxDiagonalColumn() const;

		Matrix3x3<Real> operator*(const Matrix3x3<Real> &mat);
		Vector3<Real> operator*(const Vector3<Real> &vec) const;
		Matrix3x3<Real> operator*(Real scalar);

		Matrix3x3<Real> &operator+(const Matrix3x3<Real> &mat);
		Matrix3x3<Real> &operator-(const Matrix3x3<Real> &mat);
		Matrix3x3<Real> &operator-();
		Matrix3x3<Real> &operator=(const Matrix3x3<Real> &mat);

		Matrix3x3<Real> &operator+=(const Matrix3x3<Real> &mat);
		Matrix3x3<Real> &operator-=(const Matrix3x3<Real> &mat);

		bool operator==(const Matrix3x3<Real> &mat);
		bool operator!=(const Matrix3x3<Real> &mat);


		static const Matrix3x3<Real> UnitMatrix;
	public:

		union{
			struct {
				Real _1_1,_1_2,_1_3;
				Real _2_1,_2_2,_2_3;
				Real _3_1,_3_2,_3_3;
			};
			Real m_fMatrix[9];
		};
	};
	typedef Matrix3x3<float> Matrix3x3F;
	typedef Matrix3x3<double> Matrix3x3D;
	#include "../src/K15_Matrix3x3.inl"
}

#endif //__K15_MATRIX3X3__