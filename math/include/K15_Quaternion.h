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

#include "K15_Vector3.h"
#include "K15_Matrix4x4.h"

namespace K15_Engine { namespace Math {
	class Quaternion
	{
	public:

		Quaternion();
		Quaternion(Real fWValue, Real fXValue, Real fYValue, Real fZValue);
		Quaternion(Real fSAngle, Vector3 vecVec);
		Quaternion(const Quaternion &qQuat);
		~Quaternion();


		Quaternion operator+(const Quaternion &qQuat);
		Quaternion &operator-();
		Quaternion &operator+=(const Quaternion &qQuat);
		Quaternion operator-(const Quaternion &qQuat);
		Quaternion &operator-=(const Quaternion &qQuat);
		Quaternion operator*(const Quaternion &qQuat);
		Vector3 operator*(Vector3 &vec);
		Quaternion &operator*=(const Quaternion &qQuat);
		Quaternion operator*(Real fScalar);
		Quaternion &operator*=(Real fScalar);
		Quaternion operator/(Real fScalar);
		Quaternion operator/(const Quaternion &qQuat);
		Quaternion &operator=(const Quaternion &qQuat);
		bool operator==(const Quaternion &qQuat);
		bool operator!=(const Quaternion &qQuat);
		static Real Dot(const Quaternion &qQuat1, const Quaternion &qQuat2);
		static Quaternion Lerp(const Quaternion &qQuat1, const Quaternion &qQuat2, Real fValue);
		static Quaternion Slerp(const Quaternion &qQuat1, const Quaternion &qQuat2, Real fValue);
		void QuaternionToMatrix(const Quaternion &qQuat, Matrix4x4* matDest);
		void ToMatrix(Matrix4x4* matDest);
		Quaternion AxisAngleToQuaternion(Real rAngel, Vector3 vecVec);
		Quaternion AxisAngleToQuaternion(Real rWValue, Real rXValue, Real rYValue, Real rZValue);
		Quaternion EulerAnglesToQuaternion(Real rXEuler, Real rYEuler, Real rZEuler);
		Quaternion EulerAnglesToQuaternion(Vector3 vecVec);
		Real Magnitude() const;
		void Normalize();
		void Conjugate();
		void Invert();
		void Identity();
		bool IsUnitQuaternion() const;


	public:
		union{
			struct {
				Real s;
				Vector3 v;
			};
			Real m_fValues[4];
		};
	};

	#include "K15_Quaternion.inl"
}} //end of K15_Engine::Math namespace

#endif //__K15_Quaternion__