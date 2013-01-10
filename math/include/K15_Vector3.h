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
 *							<--- PLEASE PUT A DESCRIPTION HERE
 */
#pragma once

#ifndef __K15_VECTOR3__
#define __K15_VECTOR3__

#include <cassert>

namespace K15_Math
{
	template <class Real>
	class Vector3
	{
	public:
		Vector3();
		Vector3(Real vec[3]);
		Vector3(Real x, Real y, Real z);
		Vector3(const Vector3<Real> &vec3);

		~Vector3();

		void Normalize();
		Real Magnitude() const;
		Real Dot(const Vector3<Real> &vec) const;
		Vector3<Real> Cross(const Vector3<Real> &vec) const;
		void Invert();

		bool IsNull() const;
		bool IsUnitVector() const;

		Vector3<Real> &operator*(Real fScalar);

		Real operator*(const Vector3<Real> &vec) const;
		Vector3<Real> &operator*=(Real fScalar);

		Vector3<Real> &operator=(const Vector3<Real> &vec);

		Vector3<Real> operator+(const Vector3<Real> &vec);
		Vector3<Real> operator-(const Vector3<Real> &vec);

		Vector3<Real> &operator+=(const Vector3<Real> &vec);
		Vector3<Real> &operator-=(const Vector3<Real> &vec);

		bool operator<(const Vector3<Real> &vec) const;
		bool operator>(const Vector3<Real> &vec) const;

		bool operator<=(const Vector3<Real> &vec) const;
		bool operator>=(const Vector3<Real> &vec) const;

		bool operator==(const Vector3<Real> &vec) const;
		bool operator!=(const Vector3<Real> &vec) const;

	public:
		static Vector3<Real> Up;
		static Vector3<Real> Right;
		static Vector3<Real> Forward;

	public:

		union{
			struct{
				Real x,y,z;
			};
			Real vec[3];
		};
	};

	typedef Vector3<float> Vector3F;
	typedef Vector3<double> Vector3D;

	#include "../src/K15_Vector3.inl"
}



#endif //__K15_VECTOR3__