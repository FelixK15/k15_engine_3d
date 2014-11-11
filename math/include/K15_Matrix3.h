/**
 * @file K15_Matrix3x4.h
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
 *							
 */

#ifndef _K15Engine_Math_Matrix3_h_
#define _K15Engine_Math_Matrix3_h_

#include "K15_MathPrerequisites.h"

#include "K15_Vector3.h"

namespace K15_Engine { namespace Math {
	class Matrix3
	{
	public:
		/*********************************************************************************/
		static const Matrix3 Identity;
		static const Matrix3 Zero;
		/*********************************************************************************/
	public:
		Matrix3();
		Matrix3(float p_Values[9]);
		Matrix3(float _1_1,float _1_2,float _1_3,
				  float _2_1,float _2_2,float _2_3,
				  float _3_1,float _3_2,float _3_3);
		Matrix3(const Matrix3& p_Other);

		~Matrix3(); 

		float determinant() const;

		Matrix3 inverse() const;
		Matrix3 transpose() const;

		bool isIdentity() const;
		bool isZero() const;
		
		float operator[](int p_Index) const;

		Vector3 operator*(const Vector3& p_Vector) const;
		Matrix3 operator*(const Matrix3& p_Matrix) const;
		Matrix3 operator*(float p_Scalar) const;
		Matrix3 operator+(const Matrix3& p_Matrix) const;
		Matrix3 operator-(const Matrix3& p_Matrix) const;

		const Matrix3& operator=(const Matrix3& p_Matrix);
		const Matrix3& operator+=(const Matrix3& p_Matrix);
		const Matrix3& operator-=(const Matrix3& p_Matrix);
		const Matrix3& operator*=(const Matrix3& p_Matrix);
		const Matrix3& operator*=(float p_Scalar);

		bool operator==(const Matrix3& p_Matrix);
		bool operator!=(const Matrix3& p_Matrix);

	public:
		union
		{
			struct 
			{
				float _1_1,_1_2,_1_3;
				float _2_1,_2_2,_2_3;
				float _3_1,_3_2,_3_3;
			};// struct
			float m_MatrixArray[9];
		};// union
	};// end of Matrix3x4 class
 }}//end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Matrix3x4_h_