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

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	const Vector4 Vector4::Up = Vector4(0.0f,1.0f,0.0f,1.0f);
	const Vector4 Vector4::Right = Vector4(1.0f,0.f,0.0f,1.0f);
	const Vector4 Vector4::Forward = Vector4(0.0f,0.0f,1.0f,1.0f);
	/*********************************************************************************/
	
	/*********************************************************************************/
	Vector4::Vector4()
	{
		m_VectorSIMD = _mm_set_ps(1.0f,0.0f,0.0f,0.0f);
	}
	/*********************************************************************************/
	Vector4::Vector4(float p_Values[3])
	{
		m_VectorSIMD = _mm_set_ps(p_Values[3],p_Values[2],p_Values[1],p_Values[0]);
	}
	/*********************************************************************************/
	Vector4::Vector4(float x,float y, float z, float w)
	{
		m_VectorSIMD = _mm_set_ps(w,z,y,x);
	}
	/*********************************************************************************/
	Vector4::Vector4(const Vector4& p_Vector)
	{
		m_VectorSIMD = _mm_set_ps(p_Vector.w,p_Vector.z,p_Vector.y,p_Vector.x);
	}
	/*********************************************************************************/
	Vector4::~Vector4()
	{

	}
	/*********************************************************************************/
	void Vector4::normalize()
	{
		float len = length();
		__m128 length = _mm_set_ps(len,len,len,len);
		m_VectorSIMD = _mm_div_ps(m_VectorSIMD,length);
	}
	/*********************************************************************************/
	float Vector4::length() const
	{
		__m128 temp = _mm_mul_ps(m_VectorSIMD,m_VectorSIMD);

		return ::sqrt(temp.m128_f32[0] + temp.m128_f32[1] + temp.m128_f32[2] + temp.m128_f32[3]);
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
		__m128 multiplied = _mm_mul_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
		return multiplied.m128_f32[0] + multiplied.m128_f32[1] + multiplied.m128_f32[2] + multiplied.m128_f32[3];
	}
	/*********************************************************************************/
	Vector4 Vector4::cross(const Vector4& p_Vector) const
	{
		float Pxy = x * p_Vector.y - p_Vector.x * y;
		float Pxz = x * p_Vector.z - p_Vector.x * z;
		float Pxw = x * p_Vector.w - p_Vector.x * w;
		float Pyz = y * p_Vector.z - p_Vector.y * z;
		float Pyw = y * p_Vector.w - p_Vector.y * w;
		float Pzw = z * p_Vector.w - p_Vector.z * w;

		return Vector4(
			y*Pzw - z*Pyw + w*Pyz,    
			z*Pxw - x*Pzw - w*Pxz,    
			x*Pyw - y*Pxw + w*Pxy,
			y*Pxz - x*Pyz - z*Pxy
			);
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
		__m128 temp = _mm_set_ps(p_Scale,p_Scale,p_Scale,p_Scale);
		
		Vector4 vector = *this;
		vector *= p_Scale;
		return vector;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator*=(float p_Scale)
	{
		__m128 temp = _mm_set_ps(p_Scale,p_Scale,p_Scale,p_Scale);
		m_VectorSIMD = _mm_mul_ps(m_VectorSIMD,temp);
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
		m_VectorSIMD = _mm_add_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator-=(const Vector4& p_Vector)
	{
		m_VectorSIMD = _mm_sub_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator/=(float p_Scalar)
	{
		__m128 t = _mm_set_ps(p_Scalar,p_Scalar,p_Scalar,p_Scalar);
		m_VectorSIMD = _mm_div_ps(m_VectorSIMD,t);
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
		if(length() == p_Vector.length())
		{
			return true;
		}
		return false;
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
}}//end of K15_Engine::Math namespace