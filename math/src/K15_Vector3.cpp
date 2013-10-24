/**
 * @file K15_Vector3.cpp
 * @author  Tobias Funke <tobyfunke@web.de>
 * @version 1.0
 * @date 2012/10/19
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
#include "K15_Math_PrecompiledHeaders.h"
#include "K15_Vector3.h"

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	Vector3 Vector3::Up = Vector3(0.f,1.f,0.f);
	Vector3 Vector3::Right = Vector3(1.f,0.f,0.f);
	Vector3 Vector3::Forward = Vector3(0.f,0.f,1.f);
	/*********************************************************************************/
	
	/*********************************************************************************/
	Vector3::Vector3()
	{
		x = (float)0;
		y = (float)0;
		z = (float)0;
	}
	/*********************************************************************************/
	Vector3::Vector3( float vec[3] )
	{
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	/*********************************************************************************/
	Vector3::Vector3( float x,float y, float z )
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}
	/*********************************************************************************/
	Vector3::Vector3( const Vector3 &vec3 )
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
	}
	/*********************************************************************************/
	Vector3::~Vector3()
	{

	}
	/*********************************************************************************/
	void Vector3::normalize()
	{
		float tmp = magnitude();
		
		x /= tmp;
		y /= tmp;
		z /= tmp;
	}
	/*********************************************************************************/
	float Vector3::magnitude() const
	{
		return ::sqrt(x*x + y*y + z*z);
	}
	/*********************************************************************************/
	void Vector3::invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	/*********************************************************************************/
	bool Vector3::isNull() const
	{
		return (x == y) && (y == z) && (x == 0);
	}
	/*********************************************************************************/
	bool Vector3::isUnit() const
	{
		return (magnitude() == 1);
	}
	/*********************************************************************************/
	float Vector3::dot( const Vector3 &vec ) const
	{
		return (x * vec.x + y * vec.y + z * vec.z);
	}
	/*********************************************************************************/
	Vector3 Vector3::cross( const Vector3 &vec ) const
	{
		float tmpx = y * vec.z - z * vec.y;
		float tmpy = z * vec.x - x * vec.z;
		float tmpz = x * vec.y - y * vec.x;
		return Vector3(tmpx, tmpy, tmpz);
	}
	/*********************************************************************************/
	Vector3 &Vector3::operator=(const Vector3 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}
	/*********************************************************************************/
	Vector3 &Vector3::operator*(float fScale)
	{
		x *= fScale;
		y *= fScale;
		z *= fScale;
		return *this;
	}
	/*********************************************************************************/
	Vector3 &Vector3::operator*=(float fScale)
	{
		x *= fScale;
		y *= fScale;
		z *= fScale;
		return *this;
	}
	/*********************************************************************************/
	float Vector3::operator*( const Vector3 &vec ) const
	{
		return (x * vec.x + y * vec.y + z * vec.z);
	}
	/*********************************************************************************/
	Vector3 Vector3::operator+( const Vector3 &vec )
	{
		Vector3 vecNewVec;
		vecNewVec.x = this->x + vec.x;
		vecNewVec.y = this->y + vec.y;
		vecNewVec.z = this->z + vec.z;
		return vecNewVec;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator-( const Vector3 &vec )
	{
		Vector3 vecNewVec;
		vecNewVec.x = this->x - vec.x;
		vecNewVec.y = this->y - vec.y;
		vecNewVec.z = this->z - vec.z;
		return vecNewVec;
	}
	/*********************************************************************************/
	Vector3 &Vector3::operator+=( const Vector3 &vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	/*********************************************************************************/
	Vector3 &Vector3::operator-=( const Vector3 &vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	/*********************************************************************************/
	bool Vector3::operator<( const Vector3 &vec ) const
	{
		if(magnitude()<vec.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator>( const Vector3 &vec ) const
	{
		if(magnitude()>vec.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator<=( const Vector3 &vec ) const
	{
		if(magnitude()<=vec.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator>=( const Vector3 &vec ) const
	{
		if(magnitude()>=vec.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator==( const Vector3 &vec ) const
	{
		if((x==vec.x) && (y==vec.y) && (z==vec.z)){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator!=( const Vector3 &vec ) const
	{
		if((x==vec.x) && (y==vec.y) && (z==vec.z)){
			return false;
		}
		return true;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace