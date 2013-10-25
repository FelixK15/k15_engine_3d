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
#ifndef _K15Engine_Math_Matrix4x4_h_
#define _K15Engine_Math_Matrix4x4_h_

#include "K15_MathPrerequisites.h"

#include "K15_Vector3.h"
#include "K15_Vector4.h"

namespace K15_Engine { namespace Math {
	class Matrix4
	{
	public:
		/*********************************************************************************/
		static const Matrix4 Identity;
		static const Matrix4 Zero;
		/*********************************************************************************/
	public:
		Matrix4();
		Matrix4(float p_Values[16]);
		Matrix4(float _1_1,float _1_2,float _1_3,float _1_4,
				  float _2_1,float _2_2,float _2_3,float _2_4,
				  float _3_1,float _3_2,float _3_3,float _3_4,
				  float _4_1,float _4_2,float _4_3,float _4_4);
		Matrix4(const Matrix4& p_Other);

		~Matrix4();

		float determinant() const;

		Matrix4 inverse() const;
		Matrix4 transpose() const;

		bool isIdentity() const;
		bool isZero() const;
		
		Vector3 operator*(const Vector3& p_Vector) const;
		Vector4 operator*(const Vector4& p_Vector) const;
		Matrix4 operator*(float p_Scalar) const;
		Matrix4 operator*(const Matrix4& p_Matrix) const;

		Matrix4 operator+(const Matrix4& p_Matrix) const;
		Matrix4 operator-(const Matrix4& p_Matrix) const;

		const Matrix4& operator=(const Matrix4& p_Matrix);
		const Matrix4& operator*=(float p_Scalar);
		const Matrix4& operator*=(const Matrix4& p_Matrix);
		const Matrix4& operator+=(const Matrix4& p_Matrix);
		const Matrix4& operator-=(const Matrix4& p_Matrix);

		bool operator==(const Matrix4& p_Matrix) const;
		bool operator!=(const Matrix4& p_Matrix) const;

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
			float m_MatrixArray[16];
			__m128 m_MatrixSIMD[4];
		};// union
	};// end of Matrix4x4 class
 }}//end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Matrix4x4_h_