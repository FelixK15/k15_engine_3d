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

#include "K15_Vector3.h"
#include "K15_MathUtil.h"

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	const Vector3 Vector3::Up = Vector3(0.f,1.f,0.f);
	const Vector3 Vector3::Right = Vector3(1.f,0.f,0.f);
	const Vector3 Vector3::Forward = Vector3(0.f,0.f,1.f);
	const Vector3 Vector3::Zero = Vector3(0.f,0.f,0.f);
	/*********************************************************************************/
	
	/*********************************************************************************/
	Vector3::Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	/*********************************************************************************/
	Vector3::Vector3(float p_Values[3])
	{
		x = p_Values[0];
		y = p_Values[1];
		z = p_Values[2];
	}
	/*********************************************************************************/
	Vector3::Vector3(float x,float y, float z)
	{
		m_VectorArray[0] = x;
		m_VectorArray[1] = y;
		m_VectorArray[2] = z;
	}
	/*********************************************************************************/
	Vector3::Vector3(const Vector3& p_Vector)
	{
		x = p_Vector.x;
		y = p_Vector.y;
		z = p_Vector.z;
	}
	/*********************************************************************************/
	Vector3::~Vector3()
	{

	}
	/*********************************************************************************/
	void Vector3::normalize()
	{
		float magnitude = length();
		
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
	/*********************************************************************************/
	float Vector3::length() const
	{
		return MathUtil::sqrt(x*x + y*y + z*z);
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
		return (length() == 1);
	}
	/*********************************************************************************/
	float Vector3::dot(const Vector3& p_Vector) const
	{
		return (x * p_Vector.x + y * p_Vector.y + z * p_Vector.z);
	}
	/*********************************************************************************/
	Vector3 Vector3::cross(const Vector3& p_Vector) const
	{
		float tmpx = y * p_Vector.z - z * p_Vector.y;
		float tmpy = z * p_Vector.x - x * p_Vector.z;
		float tmpz = x * p_Vector.y - y * p_Vector.x;
		return Vector3(tmpx, tmpy, tmpz);
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator=(const Vector3& p_Vector)
	{
		x = p_Vector.x;
		y = p_Vector.y;
		z = p_Vector.z;
		return *this;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator*(float p_Scale) const
	{
		Vector3 vector(*this);
		
		vector *= p_Scale;

		return vector;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator*(const Vector3& p_Vector) const
	{
		Vector3 vector(*this);

		vector *= p_Vector;

		return vector;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator/(float p_Scalar) const
	{
		Vector3 vector(*this);
		
		vector /= p_Scalar;

		return vector;
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator*=(float p_Scale)
	{
		x *= p_Scale;
		y *= p_Scale;
		z *= p_Scale;

		return *this;
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator*=(const Vector3& p_Vector)
	{
		x *= p_Vector.x;
		y *= p_Vector.y;
		z *= p_Vector.z;

		return *this;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator+(const Vector3& p_Vector) const
	{
		Vector3 vector(*this);

		vector += p_Vector;

		return vector;
	}
	/*********************************************************************************/
	Vector3 Vector3::operator-(const Vector3& p_Vector) const
	{
		Vector3 vector(*this);

		vector -= p_Vector;

		return vector;
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator+=(const Vector3& p_Vector)
	{
		x += p_Vector.x;
		y += p_Vector.y;
		z += p_Vector.z;
		return *this;
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator-=(const Vector3& p_Vector)
	{
		x -= p_Vector.x;
		y -= p_Vector.y;
		z -= p_Vector.z;
		return *this;
	}
	/*********************************************************************************/
	const Vector3& Vector3::operator/=(float p_Scalar)
	{
		x /= p_Scalar;
		y /= p_Scalar;
		z /= p_Scalar;
		return *this;
	}
	/*********************************************************************************/
	bool Vector3::operator<(const Vector3& p_Vector) const
	{
		if(length()<p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator>(const Vector3& p_Vector) const
	{
		if(length()>p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator<=(const Vector3& p_Vector) const
	{
		if(length()<=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator>=(const Vector3& p_Vector) const
	{
		if(length()>=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator==(const Vector3& p_Vector) const
	{
		if(length() == p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector3::operator!=(const Vector3& p_Vector) const
	{
		if(length() == p_Vector.length())
		{
			return false;
		}
		return true;
	}
  /*********************************************************************************/
  float Vector3::Dot(const Vector3& p_Vector, const Vector3& p_Vector2)
  {
    return p_Vector.dot(p_Vector2);
  } 
  /*********************************************************************************/
  float Vector3::Length(const Vector3& p_Vector)
  {
    return p_Vector.length();
  }
  /*********************************************************************************/
  Vector3 Vector3::Cross(const Vector3& p_Vector, const Vector3& p_Vector2)
  {
    return p_Vector.cross(p_Vector2);
  }
  /*********************************************************************************/
}}//end of K15_Engine::Math namespace