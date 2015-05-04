/**
 * @file K15_Vector4.cpp
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

#include "K15_Vector4.h"
#include "K15_MathUtil.h"
#include "K15_Vector3.h"

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	const Vector4 Vector4::Up = Vector4(0.0f,1.0f,0.0f,1.0f);
	const Vector4 Vector4::Right = Vector4(1.0f,0.f,0.0f,1.0f);
	const Vector4 Vector4::Forward = Vector4(0.0f,0.0f,1.0f,1.0f);
	const Vector4 Vector4::X_Axis = Vector4::Right;
	const Vector4 Vector4::Y_Axis = Vector4::Up;
	const Vector4 Vector4::Z_Axis = Vector4::Forward;
	const Vector4 Vector4::Zero = Vector4(0.0f,0.0f,0.0f,0.0f);
	/*********************************************************************************/
	
	/*********************************************************************************/
	Vector4::Vector4()
	{
		memset(m_VectorArray,0,sizeof(m_VectorArray));
		m_VectorArray[3] = 1.0f;
	}
	/*********************************************************************************/
	Vector4::Vector4(float p_Values[3])
	{
		memcpy(m_VectorArray,p_Values,sizeof(m_VectorArray));
	}
	/*********************************************************************************/
	Vector4::Vector4(float x,float y, float z, float w)
    : x(x),
    y(y),
    z(z),
    w(w)
	{

	}
	/*********************************************************************************/
	Vector4::Vector4(const Vector3& p_Vector, float w)
	{
		x = p_Vector.x;
		y = p_Vector.y;
		z = p_Vector.z;
		
		this->w = w;
	}
	/*********************************************************************************/
	Vector4::Vector4(const Vector4& p_Vector)
	{
		memcpy(m_VectorArray,p_Vector.m_VectorArray,sizeof(m_VectorArray));
	}
	/*********************************************************************************/
	Vector4::~Vector4()
	{

	}
	/*********************************************************************************/
	void Vector4::normalize()
	{
		float len = length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	/*********************************************************************************/
	float Vector4::length() const
	{
		return MathUtil::sqrt(x*x + y*y + z*z + w*w);
	}
	/*********************************************************************************/
	void Vector4::invert()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
	/*********************************************************************************/
	bool Vector4::isNull() const
	{
		return (x == y) && (y == z) && (x == 0) && (w == 0);
	}
	/*********************************************************************************/
	bool Vector4::isUnit() const
	{
		return (length() == 1.0f);
	}
	/*********************************************************************************/
	float Vector4::dot(const Vector4& p_Vector) const
	{
		return x*p_Vector.x + y*p_Vector.y + z*p_Vector.z + w*p_Vector.w;
	}
	/*********************************************************************************/
	Vector4 Vector4::cross(const Vector4& p_Vector) const
	{
		Vector3 vec, vec2, vec3;
		Vector4 crossVec;

		memcpy(&vec, m_VectorArray, sizeof(Vector3));
		memcpy(&vec2, &p_Vector, sizeof(Vector3));

		vec3 = vec.cross(vec2);

		memcpy(&crossVec, &vec3, sizeof(Vector3));
		crossVec.w = 1.f;

		return crossVec;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator=(const Vector4& p_Vector)
	{
		x = p_Vector.x;
		y = p_Vector.y;
		z = p_Vector.z;
		w = p_Vector.w;
		return *this;
	}
	/*********************************************************************************/
	Vector4 Vector4::operator*(float p_Scale) const
	{
		Vector4 vector = *this;
		vector *= p_Scale;
		return vector;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator*=(float p_Scale)
	{
		x *= p_Scale;
		y *= p_Scale;
		z *= p_Scale;
		w *= p_Scale;

		return *this;
	}
	/*********************************************************************************/
	Vector4 Vector4::operator+(const Vector4& p_Vector) const
	{
		Vector4 vector = *this;
		vector += p_Vector;
		
		return vector;
	}
	/*********************************************************************************/
	Vector4 Vector4::operator-(const Vector4& p_Vector) const
	{
		Vector4 vector = *this;
		vector -= p_Vector;
		return vector;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator+=(const Vector4& p_Vector)
	{
		x += p_Vector.x;
		y += p_Vector.y;
		z += p_Vector.z;
		w += p_Vector.w;

		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator-=(const Vector4& p_Vector)
	{
		x -= p_Vector.x;
		y -= p_Vector.y;
		z -= p_Vector.z;
		w -= p_Vector.w;

		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator/=(float p_Scalar)
	{
		x /= p_Scalar;
		y /= p_Scalar;
		z /= p_Scalar;
		w /= p_Scalar;

		return *this;
	}
	/*********************************************************************************/
	bool Vector4::operator<(const Vector4& p_Vector) const
	{
		if(length()<p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector4::operator>(const Vector4& p_Vector) const
	{
		if(length()>p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector4::operator<=(const Vector4& p_Vector) const
	{
		if(length()<=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector4::operator>=(const Vector4& p_Vector) const
	{
		if(length()>=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector4::operator==(const Vector4& p_Vector) const
	{
		for(int i = 0; i < 4; ++i)
    {
      if(m_VectorArray[i] != p_Vector[i])
      {
        return false;
      }
    }
		return true;
	}
	/*********************************************************************************/
	bool Vector4::operator!=(const Vector4& p_Vector) const
	{
		if(length() == p_Vector.length())
		{
			return false;
		}
		return true;
	}
  /*********************************************************************************/
  float Vector4::operator[](int p_Index) const
  {
	  if(p_Index < 0) p_Index = 0;
	  if(p_Index > 3) p_Index = 3;

	  return m_VectorArray[p_Index];
  }
  /*********************************************************************************/
}}//end of K15_Engine::Math namespace