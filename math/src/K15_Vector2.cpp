/**
 * @file K15_Vector2.cpp
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

#include "K15_Vector2.h"

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	const Vector2 Vector2::Up = Vector2(0.0f,1.0f);
	const Vector2 Vector2::Right = Vector2(1.0f,0.0f);
	const Vector2 Vector2::Zero = Vector2(0.0f,0.0f);
	/*********************************************************************************/
	
	/*********************************************************************************/
	Vector2::Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	/*********************************************************************************/
	Vector2::Vector2(float p_Values[2])
	{
		x = p_Values[0];
		y = p_Values[1];
	}
	/*********************************************************************************/
	Vector2::Vector2(float x,float y)
	{
		m_VectorArray[0] = x;
		m_VectorArray[1] = y;
	}
	/*********************************************************************************/
	Vector2::Vector2(const Vector2& p_Vector)
	{
		x = p_Vector.x;
		y = p_Vector.y;
	}
	/*********************************************************************************/
	Vector2::~Vector2()
	{

	}
	/*********************************************************************************/
	void Vector2::normalize()
	{
		float magnitude = length();
		
		x /= magnitude;
		y /= magnitude;
	}
	/*********************************************************************************/
	float Vector2::length() const
	{
		return ::sqrt(x*x + y*y);
	}
	/*********************************************************************************/
	void Vector2::invert()
	{
		x = -x;
		y = -y;
	}
	/*********************************************************************************/
	bool Vector2::isNull() const
	{
		return (x == y) && (x == 0);
	}
	/*********************************************************************************/
	bool Vector2::isUnit() const
	{
		return (length() == 1);
	}
	/*********************************************************************************/
	float Vector2::dot(const Vector2& p_Vector) const
	{
		return (x * p_Vector.x + y * p_Vector.y);
	}
	/*********************************************************************************/
	const Vector2& Vector2::operator=(const Vector2& p_Vector)
	{
		x = p_Vector.x;
		y = p_Vector.y;
		return *this;
	}
	/*********************************************************************************/
	Vector2 Vector2::operator*(float p_Scale) const
	{
		Vector2 vector = *this;
		vector.x *= p_Scale;
		vector.y *= p_Scale;
		return *this;
	}
	/*********************************************************************************/
	Vector2 Vector2::operator/(float p_Scalar) const
	{
		Vector2 vector = *this;
		vector.x /= p_Scalar;
		vector.y /= p_Scalar;
		return *this;
	}
	/*********************************************************************************/
	const Vector2& Vector2::operator*=(float p_Scale)
	{
		x *= p_Scale;
		y *= p_Scale;
		return *this;
	}
	/*********************************************************************************/
	Vector2 Vector2::operator+(const Vector2& p_Vector) const
	{
		Vector2 vecNewVec;
		vecNewVec.x = this->x + p_Vector.x;
		vecNewVec.y = this->y + p_Vector.y;
		return vecNewVec;
	}
	/*********************************************************************************/
	Vector2 Vector2::operator-(const Vector2& p_Vector) const
	{
		Vector2 vecNewVec;
		vecNewVec.x = this->x - p_Vector.x;
		vecNewVec.y = this->y - p_Vector.y;
		return vecNewVec;
	}
	/*********************************************************************************/
	const Vector2& Vector2::operator+=(const Vector2& p_Vector)
	{
		x += p_Vector.x;
		y += p_Vector.y;
		return *this;
	}
	/*********************************************************************************/
	const Vector2& Vector2::operator-=(const Vector2& p_Vector)
	{
		x -= p_Vector.x;
		y -= p_Vector.y;
		return *this;
	}
	/*********************************************************************************/
	const Vector2& Vector2::operator/=(float p_Scalar)
	{
		x /= p_Scalar;
		y /= p_Scalar;
		return *this;
	}
	/*********************************************************************************/
	bool Vector2::operator<(const Vector2& p_Vector) const
	{
		if(length()<p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator>(const Vector2& p_Vector) const
	{
		if(length()>p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator<=(const Vector2& p_Vector) const
	{
		if(length()<=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator>=(const Vector2& p_Vector) const
	{
		if(length()>=p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator==(const Vector2& p_Vector) const
	{
		if(length() == p_Vector.length())
		{
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator!=(const Vector2& p_Vector) const
	{
		if(length() == p_Vector.length())
		{
			return false;
		}
		return true;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace