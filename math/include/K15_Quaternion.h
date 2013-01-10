/**
 * @file K15_Quaternion<Real>.h
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
#pragma once

#ifndef __K15_QUATERNION__
#define __K15_QUATERNION__

#include "K15_Vector3.h"
#include "K15_Matrix4x4.h"
#include "K15_Mathematic.h"
#include <cassert>

namespace K15_Math
{
 template <class Real>
 class Quaternion
 {
 public:

	Quaternion<Real>();
	Quaternion<Real>(Real fWValue, Real fXValue, Real fYValue, Real fZValue);
	Quaternion<Real>(Real fSAngle, Vector3<Real> vecVec);
	Quaternion<Real>(const Quaternion<Real> &qQuat);
	~Quaternion<Real>();


	Quaternion<Real> operator+(const Quaternion<Real> &qQuat);
	Quaternion<Real> &operator-();
	Quaternion<Real> &operator+=(const Quaternion<Real> &qQuat);
	Quaternion<Real> operator-(const Quaternion<Real> &qQuat);
	Quaternion<Real> &operator-=(const Quaternion<Real> &qQuat);
	Quaternion<Real> operator*(const Quaternion<Real> &qQuat);
	Vector3<Real> operator*(Vector3<Real> &vec);
	Quaternion<Real> &operator*=(const Quaternion<Real> &qQuat);
	Quaternion<Real> operator*(Real fScalar);
	Quaternion<Real> &operator*=(Real fScalar);
	Quaternion<Real> operator/(Real fScalar);
	Quaternion<Real> operator/(const Quaternion<Real> &qQuat);
	Quaternion<Real> &operator=(const Quaternion<Real> &qQuat);
	bool operator==(const Quaternion<Real> &qQuat);
	bool operator!=(const Quaternion<Real> &qQuat);
	static Real Dot(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2);
	static Quaternion<Real> Lerp(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2, Real fValue);
	static Quaternion<Real> Slerp(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2, Real fValue);
	void QuaternionToMatrix(const Quaternion<Real> &qQuat, Matrix4x4<Real>* matDest);
	void ToMatrix(Matrix4x4<Real>* matDest);
	Quaternion<Real> AxisAngleToQuaternion(Real rAngel, Vector3<Real> vecVec);
	Quaternion<Real> AxisAngleToQuaternion(Real rWValue, Real rXValue, Real rYValue, Real rZValue);
	Quaternion<Real> EulerAnglesToQuaternion(Real rXEuler, Real rYEuler, Real rZEuler);
	Quaternion<Real> EulerAnglesToQuaternion(Vector3<Real> vecVec);
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
			Vector3<Real> v;
		 };
		Real m_fValues[4];
	 };
 };

 typedef Quaternion<float> QuaternionF;
 typedef Quaternion<double> QuaternionD;

#include "../src/K15_Quaternion.inl"
}

#endif //__K15_Quaternion__