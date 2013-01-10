/**
 * @file K15_Vector3.inl
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
 */

template<class Real>
Vector3<Real>::Vector3()
{
 	x = (Real)0;
	y = (Real)0;
	z = (Real)0;
}

template<class Real>
Vector3<Real>::Vector3( Real vec[3] )
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

template<class Real>
Vector3<Real>::Vector3( Real x,Real y, Real z )
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

template<class Real>
Vector3<Real>::Vector3( const Vector3<Real> &vec3 )
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}

template<class Real>
Vector3<Real>::~Vector3()
{

}

template<class Real>
void Vector3<Real>::Normalize()
{
	Real tmp = Magnitude();
	assert(tmp != 0);
	x /= tmp;
	y /= tmp;
	z /= tmp;
}

template<class Real>
Real Vector3<Real>::Magnitude() const
{
	return Mathematic<Real>::Sqrt((x*x+y*y+z*z));
}

template<class Real>
void Vector3<Real>::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

template<class Real>
bool Vector3<Real>::IsNull() const
{
	return (((Real)x == (Real)y) && ((Real)y == (Real)z) && ((Real)x == (Real)0));
}

template<class Real>
bool Vector3<Real>::IsUnitVector() const
{
	return (Magnitude() == (Real)1);
}

template<class Real>
Real Vector3<Real>::Dot( const Vector3<Real> &vec ) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}

template<class Real>
Vector3<Real> Vector3<Real>::Cross( const Vector3<Real> &vec ) const
{
	Real tmpx = y * vec.z - z * vec.y;
	Real tmpy = z * vec.x - x * vec.z;
	Real tmpz = x * vec.y - y * vec.x;
	return Vector3<Real>(tmpx, tmpy, tmpz);
}

template<class Real>
Vector3<Real> &Vector3<Real>::operator=(const Vector3<Real> &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

template<class Real>
Vector3<Real> &Vector3<Real>::operator*(Real fScale)
{
	x *= fScale;
	y *= fScale;
	z *= fScale;
	return *this;
}

template<class Real>
Vector3<Real> &Vector3<Real>::operator*=(Real fScale)
{
	x *= fScale;
	y *= fScale;
	z *= fScale;
	return *this;
}

template<class Real>
Real Vector3<Real>::operator*( const Vector3<Real> &vec ) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}

template<class Real>
Vector3<Real> Vector3<Real>::operator+( const Vector3<Real> &vec )
{
	Vector3<Real> vecNewVec;
	vecNewVec.x = this->x + vec.x;
	vecNewVec.y = this->y + vec.y;
	vecNewVec.z = this->z + vec.z;
	return vecNewVec;
}

template<class Real>
Vector3<Real> Vector3<Real>::operator-( const Vector3<Real> &vec )
{
	Vector3<Real> vecNewVec;
	vecNewVec.x = this->x - vec.x;
	vecNewVec.y = this->y - vec.y;
	vecNewVec.z = this->z - vec.z;
	return vecNewVec;
}

template<class Real>
Vector3<Real> &Vector3<Real>::operator+=( const Vector3<Real> &vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template<class Real>
Vector3<Real> &Vector3<Real>::operator-=( const Vector3<Real> &vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template<class Real>
bool Vector3<Real>::operator<( const Vector3<Real> &vec ) const
{
	if(Magnitude()<vec.Magnitude()){
		return true;
	}
	return false;
}

template<class Real>
bool Vector3<Real>::operator>( const Vector3<Real> &vec ) const
{
	if(Magnitude()>vec.Magnitude()){
		return true;
	}
	return false;
}

template<class Real>
bool Vector3<Real>::operator<=( const Vector3<Real> &vec ) const
{
	if(Magnitude()<=vec.Magnitude()){
		return true;
	}
	return false;
}

template<class Real>
bool Vector3<Real>::operator>=( const Vector3<Real> &vec ) const
{
	if(Magnitude()>=vec.Magnitude()){
		return true;
	}
	return false;
}

template<class Real>
bool Vector3<Real>::operator==( const Vector3<Real> &vec ) const
{
	if((x==vec.x) && (y==vec.y) && (z==vec.z)){
		return true;
	}
	return false;
}

template<class Real>
bool Vector3<Real>::operator!=( const Vector3<Real> &vec ) const
{
	if((x==vec.x) && (y==vec.y) && (z==vec.z)){
		return false;
	}
	return true;
}