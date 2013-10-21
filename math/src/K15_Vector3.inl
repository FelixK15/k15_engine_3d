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

template<class float>
Vector3::Vector3()
{
 	x = (float)0;
	y = (float)0;
	z = (float)0;
}

template<class float>
Vector3::Vector3( float vec[3] )
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

template<class float>
Vector3::Vector3( float x,float y, float z )
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

template<class float>
Vector3::Vector3( const Vector3 &vec3 )
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}

template<class float>
Vector3::~Vector3()
{

}

template<class float>
void Vector3::Normalize()
{
	float tmp = Magnitude();
	assert(tmp != 0);
	x /= tmp;
	y /= tmp;
	z /= tmp;
}

template<class float>
float Vector3::magnitude() const
{
	return Mathematic<float>::Sqrt((x*x+y*y+z*z));
}

template<class float>
void Vector3::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

template<class float>
bool Vector3::isNull() const
{
	return (((float)x == (float)y) && ((float)y == (float)z) && ((float)x == (float)0));
}

template<class float>
bool Vector3::isUnit() const
{
	return (Magnitude() == (float)1);
}

template<class float>
float Vector3::dot( const Vector3 &vec ) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}

template<class float>
Vector3 Vector3::cross( const Vector3 &vec ) const
{
	float tmpx = y * vec.z - z * vec.y;
	float tmpy = z * vec.x - x * vec.z;
	float tmpz = x * vec.y - y * vec.x;
	return Vector3(tmpx, tmpy, tmpz);
}

template<class float>
Vector3 &Vector3::operator=(const Vector3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

template<class float>
Vector3 &Vector3::operator*(float fScale)
{
	x *= fScale;
	y *= fScale;
	z *= fScale;
	return *this;
}

template<class float>
Vector3 &Vector3::operator*=(float fScale)
{
	x *= fScale;
	y *= fScale;
	z *= fScale;
	return *this;
}

template<class float>
float Vector3::operator*( const Vector3 &vec ) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}

template<class float>
Vector3 Vector3::operator+( const Vector3 &vec )
{
	Vector3 vecNewVec;
	vecNewVec.x = this->x + vec.x;
	vecNewVec.y = this->y + vec.y;
	vecNewVec.z = this->z + vec.z;
	return vecNewVec;
}

template<class float>
Vector3 Vector3::operator-( const Vector3 &vec )
{
	Vector3 vecNewVec;
	vecNewVec.x = this->x - vec.x;
	vecNewVec.y = this->y - vec.y;
	vecNewVec.z = this->z - vec.z;
	return vecNewVec;
}

template<class float>
Vector3 &Vector3::operator+=( const Vector3 &vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template<class float>
Vector3 &Vector3::operator-=( const Vector3 &vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template<class float>
bool Vector3::operator<( const Vector3 &vec ) const
{
	if(Magnitude()<vec.Magnitude()){
		return true;
	}
	return false;
}

template<class float>
bool Vector3::operator>( const Vector3 &vec ) const
{
	if(Magnitude()>vec.Magnitude()){
		return true;
	}
	return false;
}

template<class float>
bool Vector3::operator<=( const Vector3 &vec ) const
{
	if(Magnitude()<=vec.Magnitude()){
		return true;
	}
	return false;
}

template<class float>
bool Vector3::operator>=( const Vector3 &vec ) const
{
	if(Magnitude()>=vec.Magnitude()){
		return true;
	}
	return false;
}

template<class float>
bool Vector3::operator==( const Vector3 &vec ) const
{
	if((x==vec.x) && (y==vec.y) && (z==vec.z)){
		return true;
	}
	return false;
}

template<class float>
bool Vector3::operator!=( const Vector3 &vec ) const
{
	if((x==vec.x) && (y==vec.y) && (z==vec.z)){
		return false;
	}
	return true;
}