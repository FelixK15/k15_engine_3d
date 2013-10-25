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
#ifndef _K15Engine_Math_Matrix3x3_h_
#define _K15Engine_Math_Matrix3x3_h_

#ifndef K15_MATH_USE_PRECOMPILED_HEADER
#	include "K15_MathPrerequisites.h"
#endif //K15_MATH_USE_PRECOMPILED_HEADER

#include "K15_Vector3.h"

namespace K15_Engine { namespace Math { 
	class Matrix3x3
	{
	public:
		enum MatrixFlags{
			MF_ZERO,				
			MF_IDENT,						
		};

		Matrix3x3(MatrixFlags flag = MF_IDENT);
		Matrix3x3(float p_Values[9]);
		Matrix3x3(float _1_1,float _1_2,float _1_3,
			float _2_1,float _2_2,float _2_3,
			float _3_1,float _3_2,float _3_3);
		Matrix3x3(const Matrix3x3& p_Other);

		~Matrix3x3();

		void Identity();
		void Zero();

		Matrix3x3 columnChange(Matrix3x3& p_Other, int column1, int column2);
		Matrix3x3 rowChange(Matrix3x3& p_Other, int row1, int row2);

		float determinant() const;

		void inverse();
		void transpose();
		void Rotate(const Vector3& p_Vector,float angleInRad);
		void Scale(const Vector3& scaleVec);
		void Translate(const Vector3& posVec);

		bool isTriangle() const;
		bool isLowerTriangle() const;
		bool isUpperTriangle() const;
		bool isIdentity() const;
		bool isZero() const;
		bool hasZeroVector() const;
		int getMaxDiagonalColumn() const;

		Matrix3x3 operator*(const Matrix3x3& p_Other);
		Vector3 operator*(const Vector3& p_Vector) const;
		Matrix3x3 operator*(float scalar);

		Matrix3x3& operator+(const Matrix3x3& p_Other);
		Matrix3x3& operator-(const Matrix3x3& p_Other);
		Matrix3x3& operator-();
		Matrix3x3& operator=(const Matrix3x3& p_Other);

		Matrix3x3& operator+=(const Matrix3x3& p_Other);
		Matrix3x3& operator-=(const Matrix3x3& p_Other);

		bool operator==(const Matrix3x3& p_Other);
		bool operator!=(const Matrix3x3& p_Other);

		static const Matrix3x3 UnitMatrix;
	public:
		union
		{
			struct 
			{
				float _1_1,_1_2,_1_3;
				float _2_1,_2_2,_2_3;
				float _3_1,_3_2,_3_3;
			};//struct

			float m_MatrixValue[9];
		};//union
	};// end of Matrix3x3 class declaration
 }}//end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Matrix3x3_h_