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

#ifndef _K15Engine_Math_Vector3_h_
#define _K15Engine_Math_Vector3_h_

#include "K15_MathPrerequisites.h"

namespace K15_Engine { namespace Math {
	class Vector3
	{
	public:
		/*********************************************************************************/
		static const Vector3 Up;
		static const Vector3 Right;
		static const Vector3 Forward;
		static const Vector3 Zero;
		/*********************************************************************************/
	public:
		Vector3();
		Vector3(float p_Values[3]);
		Vector3(float x, float y, float z);
		Vector3(const Vector3& p_Vector);

		~Vector3();

		void normalize();
		float length() const;
		float dot(const Vector3& p_Vector) const;
		Vector3 cross(const Vector3& p_Vector) const;
		void invert();

		bool isNull() const;
		bool isUnit() const;

		Vector3 operator/(float p_Scalar) const;
		Vector3 operator*(float p_Scalar) const;
		Vector3 operator+(const Vector3& p_Vector) const;
		Vector3 operator-(const Vector3& p_Vector) const;
		Vector3 operator*(const Vector3& p_Vector) const;

		const Vector3& operator=(const Vector3& p_Vector);
		const Vector3& operator+=(const Vector3& p_Vector);
		const Vector3& operator-=(const Vector3& p_Vector);
		const Vector3& operator*=(const Vector3& p_Vector);
		const Vector3& operator*=(float p_Scalar);
		const Vector3& operator/=(float p_Scalar);

		bool operator<(const Vector3& p_Vector) const;
		bool operator>(const Vector3& p_Vector) const;

		bool operator<=(const Vector3& p_Vector) const;
		bool operator>=(const Vector3& p_Vector) const;

		bool operator==(const Vector3& p_Vector) const;
		bool operator!=(const Vector3& p_Vector) const;

  public:
    static float Dot(const Vector3& p_Vector, const Vector3& p_Vector2);
    static float Length(const Vector3& p_Vector);
    static Vector3 Cross(const Vector3& p_Vector, const Vector3& p_Vector2);

	public:
		union{
			struct
			{
				float x,y,z;
			};//struct
			float m_VectorArray[3];
		};//union
	};// end of Vector3 class declaration
}}// end of K15_Engine::Math namespace

#endif //_K15Engine_Math_Vector3_h_