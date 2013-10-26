/**
 * @file K15_Vector4.h
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

#ifndef _K15Engine_Math_Vector4_h_
#define _K15Engine_Math_Vector4_h_

#include "K15_MathPrerequisites.h"

#pragma message("FK - Add lazy evaluation and SIMD semantics")

namespace K15_Engine { namespace Math {
	class Vector4
	{
	public:
		/*********************************************************************************/
		static const Vector4 Up;
		static const Vector4 Right;
		static const Vector4 Forward;
		/*********************************************************************************/
	public:
		Vector4();
		Vector4(float p_Values[4]);
		Vector4(float x, float y, float z,float w);
		Vector4(const Vector4& p_Vector);

		~Vector4();

		void normalize();
		float length() const;
		float dot(const Vector4& p_Vector) const;
		Vector4 cross(const Vector4& p_Vector) const;
		void invert();

		bool isNull() const;
		bool isUnit() const;

		
		float operator*(const Vector4& p_Vector) const;
		Vector4 operator/(float p_Scalar) const;
		Vector4 operator*(float p_Scalar) const;
		Vector4 operator+(const Vector4& p_Vector) const;
		Vector4 operator-(const Vector4& p_Vector) const;

		const Vector4& operator=(const Vector4& p_Vector);
		const Vector4& operator+=(const Vector4& p_Vector);
		const Vector4& operator-=(const Vector4& p_Vector);
		const Vector4& operator*=(float p_Scalar);
		const Vector4& operator/=(float p_Scalar);

		bool operator<(const Vector4& p_Vector) const;
		bool operator>(const Vector4& p_Vector) const;

		bool operator<=(const Vector4& p_Vector) const;
		bool operator>=(const Vector4& p_Vector) const;

		bool operator==(const Vector4& p_Vector) const;
		bool operator!=(const Vector4& p_Vector) const;

	public:
		union{
			struct
			{
				float x,y,z,w;
			};//struct
			float m_VectorArray[4];
			__m128 m_VectorSIMD;
		};//union
	};// end of Vector4 class declaration
}}// end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Vector4_h_