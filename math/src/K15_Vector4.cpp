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

namespace K15_Engine { namespace Math { 
	/*********************************************************************************/
	const Vector4 Vector4::Up = Vector4(0.0f,1.0f,0.0f,1.0f);
	const Vector4 Vector4::Right = Vector4(1.0f,0.f,0.0f,1.0f);
	const Vector4 Vector4::Forward = Vector4(0.0f,0.0f,1.0f,1.0f);
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
	{
		#if defined K15_SIMD_SUPPORT
			m_VectorSIMD = _mm_set_ps(w,z,y,x);
		#else
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		#endif //K15_SIMD_SUPPORT
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
		#if defined K15_SIMD_SUPPORT
			__m128 length = _mm_set_ps(len,len,len,len);
			m_VectorSIMD = _mm_div_ps(m_VectorSIMD,length);
		#else 
			x /= len;
			y /= len;
			z /= len;
			w /= len;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	float Vector4::length() const
	{
		#if defined K15_SIMD_SUPPORT
			__m128 temp = _mm_mul_ps(m_VectorSIMD,m_VectorSIMD);

			return MathUtil::sqrt(temp.m128_f32[0] + temp.m128_f32[1] + temp.m128_f32[2] + temp.m128_f32[3]);
		#else 
			return MathUtil::sqrt(x*x + y*y + z*z + w*w);
		#endif //K15_SIMD_SUPPRT
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
		#if defined K15_SIMD_SUPPORT
			__m128 multiplied = _mm_mul_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
			return multiplied.m128_f32[0] + multiplied.m128_f32[1] + multiplied.m128_f32[2] + multiplied.m128_f32[3];
		#else
			return x*p_Vector.x + y*p_Vector.y + z*p_Vector.z + w*p_Vector.w;
		#endif //K15_SIMD_SUPPORT
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
		Vector4 vector = *this;
		vector *= p_Scale;
		return vector;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator*=(float p_Scale)
	{
		#if defined K15_SIMD_SUPPORT
			__m128 temp = _mm_set_ps(p_Scale,p_Scale,p_Scale,p_Scale);
			m_VectorSIMD = _mm_mul_ps(m_VectorSIMD,temp);
		#else
			x *= p_Scale;
			y *= p_Scale;
			z *= p_Scale;
			w *= p_Scale;
		#endif //K15_SIMD_SUPPORT

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
		#if defined K15_SIMD_SUPPORT
			m_VectorSIMD = _mm_add_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
		#else
			x += p_Vector.x;
			y += p_Vector.y;
			z += p_Vector.z;
			w += p_Vector.w;
		#endif //K15_SIMD_SUPPORT

		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator-=(const Vector4& p_Vector)
	{
		#if defined K15_SIMD_SUPPORT
			m_VectorSIMD = _mm_sub_ps(m_VectorSIMD,p_Vector.m_VectorSIMD);
		#else
			x -= p_Vector.x;
			y -= p_Vector.y;
			z -= p_Vector.z;
			w -= p_Vector.w;
		#endif //K15_SIMD_SUPPORT

		return *this;
	}
	/*********************************************************************************/
	const Vector4& Vector4::operator/=(float p_Scalar)
	{
		#if defined K15_SIMD_SUPPORT
			__m128 t = _mm_set_ps(p_Scalar,p_Scalar,p_Scalar,p_Scalar);
			m_VectorSIMD = _mm_div_ps(m_VectorSIMD,t);
		#else
			x /= p_Scalar;
			y /= p_Scalar;
			z /= p_Scalar;
			w /= p_Scalar;
		#endif //K15_SIMD_SUPPORT

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