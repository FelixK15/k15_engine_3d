/**
 * @file K15_Quaternion.h
 * @author  Tobias Funke <t.funke@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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

#ifndef _K15Engine_Math_Quaternion_h_
#define _K15Engine_Math_Quaternion_h_

#include "K15_MathPrerequisites.h"
#include "K15_Matrix3.h"

namespace K15_Engine { namespace Math {
	class Quaternion
	{
	public:
		/*********************************************************************************/
		static const Quaternion Identity;
		static const Quaternion Zero;
		/*********************************************************************************/
	public:
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(float p_Angle, const Vector3& p_Vector);
		Quaternion(const Matrix3& p_Matrix);
		Quaternion(const Quaternion &qQuat);
		~Quaternion();

		float length() const;
		void normalize();
		void conjugate();
		void invert();

		Quaternion operator+(const Quaternion &qQuat) const;
		Quaternion operator-(const Quaternion &qQuat) const;
		Quaternion operator*(const Quaternion &qQuat) const;
		Quaternion operator/(const Quaternion &qQuat) const;
		Quaternion operator*(float fScalar) const;
		Quaternion operator/(float fScalar) const;

		const Quaternion& operator=(const Quaternion &qQuat);
		const Quaternion& operator+=(const Quaternion &qQuat);
		const Quaternion& operator-=(const Quaternion &qQuat);
		const Quaternion& operator*=(const Quaternion &qQuat);
		const Quaternion& operator/=(const Quaternion& p_Quaternion);
		const Quaternion& operator*=(float p_Scalar);
		const Quaternion& operator/=(float p_Scalar);
		
		bool operator==(const Quaternion &qQuat) const;
		bool operator!=(const Quaternion &qQuat) const;

		Vector3 operator*(const Vector3& p_Vector) const;
		
		float getRoll() const;
		float getPitch() const;
		float getYaw() const;

		bool isIdentity() const;
		
		float dot(const Quaternion& p_Quaternion) const;
		
		Matrix3 toRotationMatrix() const;
		void fromRotationMatrix(const Matrix3& p_Matrix);
		void fromAngleAxis(float p_Angle, const Vector3& p_Vector);

// 		static Quaternion Lerp(const Quaternion &qQuat1, const Quaternion &qQuat2, float fValue);
// 		static Quaternion Slerp(const Quaternion &qQuat1, const Quaternion &qQuat2, float fValue);
	
	public:
		union
		{
// 			struct 
// 			{
// 				float m_Angle;
// 				Vector3 v;
// 			};//struct

			struct  
			{
				float w, x, y, z;
			};//struct
			float m_QuaternionArray[4];
			#if defined K15_SIMD_SUPPORT
				__m128 m_QuaternionSIMD;
			#endif //K15_SIMD_SUPPORT
		};//union
	};// end of Quaternion class declaration
}} //end of K15_Engine::Math namespace

#endif //__K15_Quaternion__