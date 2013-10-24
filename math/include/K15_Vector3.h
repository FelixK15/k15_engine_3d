/**
 * @file K15_Vector3.h
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
#pragma once

#ifndef _K15Engine_Math_Vector3_h_
#define _K15Engine_Math_Vector3_h_

#ifndef K15_MATH_USE_PREDEFINED_HEADER
#	include "K15_Math_Prerequisites.h"
#endif //K15_MATH_USE_PREDEFINED_HEADER

#pragma message("FK - Add lazy evaluation and SIMD semantics")

namespace K15_Engine { namespace Math {
	class Vector3
	{
	public:
		Vector3();
		Vector3(float p_Values[3]);
		Vector3(float x, float y, float z);
		Vector3(const Vector3& p_Other);

		~Vector3();

		void normalize();
		float magnitude() const;
		float dot(const Vector3& p_Other) const;
		Vector3 cross(const Vector3& p_Other) const;
		void invert();

		bool isNull() const;
		bool isUnit() const;

		Vector3& operator*(float p_Scalar);

		float operator*(const Vector3& p_Other) const;
		Vector3& operator*=(float p_Scalar);

		Vector3& operator=(const Vector3& p_Other);

		Vector3 operator+(const Vector3& p_Other);
		Vector3 operator-(const Vector3& p_Other);

		Vector3& operator+=(const Vector3& p_Other);
		Vector3& operator-=(const Vector3& p_Other);

		bool operator<(const Vector3& p_Other) const;
		bool operator>(const Vector3& p_Other) const;

		bool operator<=(const Vector3& p_Other) const;
		bool operator>=(const Vector3& p_Other) const;

		bool operator==(const Vector3& p_Other) const;
		bool operator!=(const Vector3& p_Other) const;

	public:
		static Vector3 Up;
		static Vector3 Right;
		static Vector3 Forward;

	public:
		union{
			struct
			{
				float x,y,z;
			};//struct
			float vec[3];
		};//union
	};// end of Vector3 class declaration
}}// end of K15_Engine::Math namespace



#endif //_K15Engine_Math_Vector3_h_