/**
 * @file K15_Vector2.h
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

#ifndef _K15Engine_Math_Vector2_h_
#define _K15Engine_Math_Vector2_h_

#ifndef K15_MATH_USE_PRECOMPILED_HEADER
#	include "K15_Math_Prerequisites.h"
#endif //K15_MATH_USE_PRECOMPILED_HEADER

#pragma message("FK - Add lazy evaluation and SIMD semantics")

namespace K15_Engine { namespace Math {

	class Vector2
	{
	public:
		Vector2();
		Vector2(float p_Values[2]);
		Vector2(float x,float y);
		Vector2(const Vector2& p_Other);

		~Vector2();

		void normalize();
		float magnitude() const;
		float dot(const Vector2& p_Other) const;

		Vector2& operator*(float p_Scalar);

		float operator*(const Vector2& p_Other) const;

		Vector2& operator+(const Vector2& p_Other);
		Vector2& operator-(const Vector2& p_Other);

		Vector2& operator+=(const Vector2& p_Other);
		Vector2& operator-=(const Vector2& p_Other);

		bool operator<(const Vector2& p_Other);
		bool operator>(const Vector2& p_Other);

		bool operator<=(const Vector2& p_Other);
		bool operator>=(const Vector2& p_Other);

		bool operator==(const Vector2& p_Other);
		bool operator!=(const Vector2& p_Other);

	public:
		union
		{
			struct
			{
				float x,y;
			};// struct
			float vec[2];
		};// union
	};// end of Vector2 class declaration
}}// end of K15_Engine::Math namespace

#endif //__K15_VECTOR2__