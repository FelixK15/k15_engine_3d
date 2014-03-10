/**
 * @file K15_Quaternion.cpp
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
 */

#include "K15_Quaternion.h"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	const Quaternion Quaternion::Identity;
	const Quaternion Quaternion::Zero(0.0f,0.0f,0.0f,0.0f);
	/*********************************************************************************/
	Quaternion::Quaternion()
	{
		memset(m_QuaternionArray,0,sizeof(m_QuaternionArray));
	}
	/*********************************************************************************/
	Quaternion::Quaternion(float w, float x, float y, float z)
	{
		#if defined K15_SIMD_SUPPORT
			m_QuaternionSIMD = _mm_set_ps(z,y,x,w);
		#else
			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	Quaternion::Quaternion(float p_Angle, const Vector3& p_Vector)
	{
		#if defined K15_SIMD_SUPPORT
			m_QuaternionSIMD = _mm_set_ps(p_Vector.z,p_Vector.y,p_Vector.x,p_Angle);
		#else
			x = p_Vector.x;
			y = p_Vector.y;
			z = p_Vector.z;
			w = p_Angle;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	Quaternion::Quaternion(const Quaternion& p_Quaternion)
	{
		memcpy(m_QuaternionArray,p_Quaternion.m_QuaternionArray,sizeof(m_QuaternionArray));
	}
	/*********************************************************************************/
	Quaternion::~Quaternion()
	{

	}
	/*********************************************************************************/
	float Quaternion::length() const
	{
		#if defined K15_SIMD_SUPPORT
			__m128 t = _mm_mul_ps(m_QuaternionSIMD,m_QuaternionSIMD);
			return ::sqrt(t.m128_f32[0] + t.m128_f32[1] + t.m128_f32[2] + t.m128_f32[3]);
		#else
			return ::sqrt(x*x + y*y + z*z + w*w);
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	void Quaternion::normalize()
	{
		float magnitude = length();

		#if defined K15_SIMD_SUPPORT
			__m128 t = _mm_set_ps(magnitude,magnitude,magnitude,magnitude);
			m_QuaternionSIMD = _mm_div_ps(m_QuaternionSIMD,t);
		#else
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
			w /= magnitude;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	void Quaternion::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	/*********************************************************************************/
	void Quaternion::invert()
	{
		conjugate();
		*this / length();
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator+(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion += p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator-(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion -= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator*(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion *= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator/(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion /= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator*(float p_Scalar) const
	{
		Quaternion quaternion(*this);

		quaternion *= p_Scalar;

		return *this;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator/(float p_Scalar) const
	{
		assert(p_Scalar != 0);
		Quaternion quaternion(*this);

		quaternion /= p_Scalar;

		return quaternion;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator=(const Quaternion& p_Quaternion)
	{
		memcpy((void*)p_Quaternion.m_QuaternionArray,m_QuaternionArray,sizeof(m_QuaternionArray));
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator+=(const Quaternion& p_Quaternion)
	{
		#if defined K15_SIMD_SUPPORT
			m_QuaternionSIMD = _mm_add_ps(m_QuaternionSIMD,p_Quaternion.m_QuaternionSIMD);
		#else
			x += p_Quaternion.x;
			y += p_Quaternion.y;
			z += p_Quaternion.z;
			w += p_Quaternion.w;
		#endif //K15_SIMD_SUPPORT
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator-=(const Quaternion& p_Quaternion)
	{
		#if defined K15_SIMD_SUPPORT
			m_QuaternionSIMD = _mm_sub_ps(m_QuaternionSIMD,p_Quaternion.m_QuaternionSIMD);
		#else
			x *= p_Quaternion.x;
			y *= p_Quaternion.y;
			z *= p_Quaternion.z;
			w *= p_Quaternion.w;
		#endif //K15_SIMD_SUPPORT
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator*=(const Quaternion& p_Quaternion)
	{
		//https://bitbucket.org/sinbad/ogre/src/569ec69ce2c31a66b32d741a455a1d91428079cc/OgreMain/src/OgreQuaternion.cpp?at=default
		#if defined K15_SIMD_SUPPORT
			__m128 t_1_me = _mm_set_ps(w,y,z,x);
			__m128 t_2_me = _mm_set_ps(w,z,x,y);

			__m128 t_1_other = _mm_set_ps(p_Quaternion.x,p_Quaternion.w,p_Quaternion.z,p_Quaternion.y);
			__m128 t_2_other = _mm_set_ps(p_Quaternion.y,p_Quaternion.w,p_Quaternion.x,p_Quaternion.z);
			__m128 t_3_other = _mm_set_ps(p_Quaternion.y,p_Quaternion.w,p_Quaternion.y,p_Quaternion.x);

			__m128 t_w = _mm_mul_ps(m_QuaternionSIMD,p_Quaternion.m_QuaternionSIMD);
			__m128 t_x = _mm_mul_ps(m_QuaternionSIMD,t_1_other);
			__m128 t_y = _mm_mul_ps(t_1_me,t_2_other);
			__m128 t_z = _mm_mul_ps(t_2_me,t_3_other);

			w =	t_w.m128_f32[0] - t_w.m128_f32[1] - t_w.m128_f32[2] - t_w.m128_f32[3];
			x =	t_x.m128_f32[0] - t_x.m128_f32[1] - t_x.m128_f32[2] - t_x.m128_f32[3];
			y =	t_y.m128_f32[0] - t_y.m128_f32[1] - t_y.m128_f32[2] - t_y.m128_f32[3];
			z =	t_z.m128_f32[0] - t_z.m128_f32[1] - t_z.m128_f32[2] - t_z.m128_f32[3];
		#else
			float t_w = w * p_Quaternion.w - x * p_Quaternion.x - y * p_Quaternion.y - z * p_Quaternion.z;
			float t_x = w * p_Quaternion.x + x * p_Quaternion.w + y * p_Quaternion.z - z * p_Quaternion.y;
			float t_y = w * p_Quaternion.y + y * p_Quaternion.w + z * p_Quaternion.x - x * p_Quaternion.z;
			float t_z = w * p_Quaternion.z + z * p_Quaternion.w + x * p_Quaternion.y - y * p_Quaternion.x;
		#endif //K15_SIMD_SUPPORT
		
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator/=(const Quaternion& p_Quaternion)
	{
		Quaternion quaternion(p_Quaternion);
		quaternion.invert();

		return this->operator*=(quaternion);
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator*=(float p_Scalar)
	{
		#if defined K15_SIMD_SUPPORT
			__m128 t = _mm_set_ps(p_Scalar,p_Scalar,p_Scalar,p_Scalar);
			m_QuaternionSIMD = _mm_mul_ps(m_QuaternionSIMD,t);
		#else
			x *= p_Scalar;
			y *= p_Scalar;
			z *= p_Scalar;
			w *= p_Scalar;
		#endif //K15_SIMD_SUPPORT
		
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator/=(float p_Scalar)
	{
		#if defined K15_SIMD_SUPPORT
			__m128 t = _mm_set_ps(p_Scalar,p_Scalar,p_Scalar,p_Scalar);
			m_QuaternionSIMD = _mm_div_ps(m_QuaternionSIMD,t);
		#else
			x /= p_Scalar;
			y /= p_Scalar;
			z /= p_Scalar;
			w /= p_Scalar;
		#endif //K15_SIMD_SUPPORT
		
		return *this;
	}
	/*********************************************************************************/
	bool Quaternion::operator==(const Quaternion& p_Quaternion) const
	{
		return (x == p_Quaternion.x) && (y == p_Quaternion.y) && (z == p_Quaternion.z) && (w == p_Quaternion.w);
	}
	/*********************************************************************************/
	bool Quaternion::operator!=(const Quaternion& p_Quaternion) const
	{
		return !this->operator==(p_Quaternion);
	}
	/*********************************************************************************/
	Vector3 Quaternion::operator*(const Vector3& p_Vector) const
	{
		Vector3 vTmp1, vTmp2, vTmp3;

		vTmp3.x = x;
		vTmp3.y = y;
		vTmp3.z = z;

		vTmp1 = vTmp3.cross(p_Vector);
		vTmp2 = vTmp3.cross(vTmp1);
		vTmp1 *= (2 * w);
		vTmp2 *= 2;
		return p_Vector + vTmp1 + vTmp2;
	}
	/*********************************************************************************/
	float Quaternion::getRoll() const
	{
		 return ::atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
	}
	/*********************************************************************************/
	float Quaternion::getPitch() const
	{
		return ::atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
	}
	/*********************************************************************************/
	float Quaternion::getYaw() const
	{
		return ::asin(-2*(x*z - w*y));
	}
	/*********************************************************************************/
	bool Quaternion::isIdentity() const
	{
		return w == 1.0f && (x == 0.0f && y == 0.0f && z == 0.0f);
	}
	/*********************************************************************************/
	float Quaternion::dot(const Quaternion& p_Quaternion) const
	{
		#if defined K15_SIMD_SUPPORT
			__m128 dot = _mm_mul_ps(m_QuaternionSIMD,p_Quaternion.m_QuaternionSIMD);
			return dot.m128_f32[0] + dot.m128_f32[1] + dot.m128_f32[2] + dot.m128_f32[3];
		#else
			return x*p_Quaternion.x + y*p_Quaternion.y + z*p_Quaternion.z * w*p_Quaternion.w;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	Matrix3 Quaternion::toRotationMatrix() const
	{
		//https://bitbucket.org/sinbad/ogre/src/569ec69ce2c31a66b32d741a455a1d91428079cc/OgreMain/src/OgreQuaternion.cpp?at=default
		Matrix3 matrix;

		float fTx  = x+x;
		float fTy  = y+y;
		float fTz  = z+z;
		float fTwx = fTx*w;
		float fTwy = fTy*w;
		float fTwz = fTz*w;
		float fTxx = fTx*x;
		float fTxy = fTy*x;
		float fTxz = fTz*x;
		float fTyy = fTy*y;
		float fTyz = fTz*y;
		float fTzz = fTz*z;

		matrix._1_1 = 1.0f-(fTyy+fTzz);
		matrix._1_1 = fTxy-fTwz;
		matrix._1_1 = fTxz+fTwy;

		matrix._1_1 =  fTxy+fTwz;
		matrix._1_1 = 1.0f-(fTxx+fTzz);
		matrix._1_1 = fTyz-fTwx;

		matrix._1_1 = fTxz-fTwy;
		matrix._1_1 = fTyz+fTwx;
		matrix._1_1 = 1.0f-(fTxx+fTyy);

		return matrix;
	}
	/*********************************************************************************/
	void Quaternion::fromRotationMatrix(const Matrix3& p_Matrix)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		float fTrace = p_Matrix._1_1 + p_Matrix._2_2 + p_Matrix._3_3;
		float fRoot;

		if(fTrace > 0.0f)
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = ::sqrt(fTrace + 1.0f);  // 2w
			w = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)
			x = (p_Matrix._3_2 - p_Matrix._2_3) * fRoot;
			y = (p_Matrix._1_3 - p_Matrix._3_1) * fRoot;
			z = (p_Matrix._2_1 - p_Matrix._1_2) * fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if(p_Matrix._2_2 > p_Matrix._1_1)
				i = 1;
			if(p_Matrix._3_3 > p_Matrix.m_MatrixArray[i * 3 + i])
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			//To test if this even works
			//__debugbreak();

			fRoot = ::sqrt(p_Matrix.m_MatrixArray[i * 3 + i] - p_Matrix.m_MatrixArray[j * 3 + j] - p_Matrix.m_MatrixArray[k * 3 + k] + 1.0f);
			float* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			w = (p_Matrix.m_MatrixArray[k * 3 + j] - p_Matrix.m_MatrixArray[j * 3 + k]) * fRoot;
			*apkQuat[j] = (p_Matrix.m_MatrixArray[j * 3 + i] + p_Matrix.m_MatrixArray[i * 3 + j]) * fRoot;
			*apkQuat[k] = (p_Matrix.m_MatrixArray[k * 3 + i] + p_Matrix.m_MatrixArray[i * 3 + k]) * fRoot;
		}
	}
	/*********************************************************************************/
	void Quaternion::fromAngleAxis(float p_Angle, const Vector3& p_Vector)
	{
		//https://bitbucket.org/sinbad/ogre/src/569ec69ce2c31a66b32d741a455a1d91428079cc/OgreMain/src/OgreQuaternion.cpp?at=default

		float fHalfAngle ( 0.5f*p_Angle );
		float fSin = ::sin(fHalfAngle);
		w = ::cos(fHalfAngle);
		x = fSin*p_Vector.x;
		y = fSin*p_Vector.y;
		z = fSin*p_Vector.z;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace
