/**
 * @file K15_Matrix4x4.cpp
 * @author  Tobias Funke <tobyfunke@web.de>
 * @version 1.0
 * @date 2012/10/20
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

#include "K15_Matrix4.h"
#include "K15_MathUtil.h"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	const Matrix4 Matrix4::Identity;
	const Matrix4 Matrix4::Zero(0.0f,0.0f,0.0f,0.0f,
								0.0f,0.0f,0.0f,0.0f,
								0.0f,0.0f,0.0f,0.0f,
								0.0f,0.0f,0.0f,0.0f);
	/*********************************************************************************/

	/*********************************************************************************/
	Matrix4::Matrix4()
	{
		memset(&m_MatrixArray,0,sizeof(m_MatrixArray));
		m_MatrixArray[3] = m_MatrixArray[6] = m_MatrixArray[9] = m_MatrixArray[12] = 1.0f;
	}
	/*********************************************************************************/
	Matrix4::Matrix4(float p_Values[16])
	{
		memcpy(m_MatrixArray,p_Values,sizeof(p_Values));
	}
	/*********************************************************************************/
	Matrix4::Matrix4(float _1_1,float _1_2,float _1_3,float _1_4,
		float _2_1,float _2_2,float _2_3,float _2_4,
		float _3_1,float _3_2,float _3_3,float _3_4,
		float _4_1,float _4_2,float _4_3,float _4_4)
	{
		#if defined K15_SIMD_SUPPORT	
			m_MatrixSIMD[0] = _mm_set_ps(_1_4,_1_3,_1_2,_1_1);
			m_MatrixSIMD[1] = _mm_set_ps(_2_4,_2_3,_3_2,_2_1);
			m_MatrixSIMD[2] = _mm_set_ps(_3_4,_3_3,_3_2,_3_1);
			m_MatrixSIMD[3] = _mm_set_ps(_4_4,_4_3,_4_2,_4_1);
		#else
			this->_1_1 = _1_1; this->_1_2 = _1_2; this->_1_3 = _1_3; this->_1_4 = _1_4;
			this->_2_1 = _2_1; this->_2_2 = _2_2; this->_2_3 = _2_3; this->_2_4 = _2_4;
			this->_3_1 = _3_1; this->_3_2 = _3_2; this->_3_3 = _3_3; this->_3_4 = _3_4;
			this->_4_1 = _4_1; this->_4_2 = _4_2; this->_4_3 = _4_3; this->_4_4 = _4_4;
		#endif //K15_SIMD_SUPPORT
	}
	/*********************************************************************************/
	Matrix4::Matrix4(const Matrix4& p_Matrix)
	{
		memcpy(m_MatrixArray,p_Matrix.m_MatrixArray,sizeof(m_MatrixArray));
	}
	/*********************************************************************************/
	Matrix4::~Matrix4()
	{

	}
	/*********************************************************************************/
	float Matrix4::determinant() const
	{
		return 0.0f;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::inverse() const
	{
 		float tmpDet = determinant();
 		//K15_ASSERT(tmpDet != 0,"Matrix is not inversable. Determinant == 0!");

		Matrix4 invMatrix;
		invMatrix.m_MatrixArray[0] =	 m_MatrixArray[5]  * m_MatrixArray[10] * m_MatrixArray[15] - 
			m_MatrixArray[5]  * m_MatrixArray[11] * m_MatrixArray[14] - 
			m_MatrixArray[9]  * m_MatrixArray[6]  * m_MatrixArray[15] + 
			m_MatrixArray[9]  * m_MatrixArray[7]  * m_MatrixArray[14] +
			m_MatrixArray[13] * m_MatrixArray[6]  * m_MatrixArray[11] - 
			m_MatrixArray[13] * m_MatrixArray[7]  * m_MatrixArray[10];

		invMatrix.m_MatrixArray[1] =	-(m_MatrixArray[1])  * m_MatrixArray[10] * m_MatrixArray[15] + 
			m_MatrixArray[1]  * m_MatrixArray[11] * m_MatrixArray[14] + 
			m_MatrixArray[9]  * m_MatrixArray[2] * m_MatrixArray[15] - 
			m_MatrixArray[9]  * m_MatrixArray[3] * m_MatrixArray[14] - 
			m_MatrixArray[13] * m_MatrixArray[2] * m_MatrixArray[11] + 
			m_MatrixArray[13] * m_MatrixArray[3] * m_MatrixArray[10];

		invMatrix.m_MatrixArray[2] =	 m_MatrixArray[1]  * m_MatrixArray[6] * m_MatrixArray[15] - 
			m_MatrixArray[1]  * m_MatrixArray[7] * m_MatrixArray[14] - 
			m_MatrixArray[5]  * m_MatrixArray[2] * m_MatrixArray[15] + 
			m_MatrixArray[5]  * m_MatrixArray[3] * m_MatrixArray[14] + 
			m_MatrixArray[13] * m_MatrixArray[2] * m_MatrixArray[7] - 
			m_MatrixArray[13] * m_MatrixArray[3] * m_MatrixArray[6];

		invMatrix.m_MatrixArray[3] =	-m_MatrixArray[1] * m_MatrixArray[6] * m_MatrixArray[11] + 
			m_MatrixArray[1] * m_MatrixArray[7] * m_MatrixArray[10] + 
			m_MatrixArray[5] * m_MatrixArray[2] * m_MatrixArray[11] - 
			m_MatrixArray[5] * m_MatrixArray[3] * m_MatrixArray[10] - 
			m_MatrixArray[9] * m_MatrixArray[2] * m_MatrixArray[7] + 
			m_MatrixArray[9] * m_MatrixArray[3] * m_MatrixArray[6];

		invMatrix.m_MatrixArray[4] =	-m_MatrixArray[4]  * m_MatrixArray[10] * m_MatrixArray[15] + 
			m_MatrixArray[4]  * m_MatrixArray[11] * m_MatrixArray[14] + 
			m_MatrixArray[8]  * m_MatrixArray[6]  * m_MatrixArray[15] - 
			m_MatrixArray[8]  * m_MatrixArray[7]  * m_MatrixArray[14] - 
			m_MatrixArray[12] * m_MatrixArray[6]  * m_MatrixArray[11] + 
			m_MatrixArray[12] * m_MatrixArray[7]  * m_MatrixArray[10];

		invMatrix.m_MatrixArray[5] =	 m_MatrixArray[0]  * m_MatrixArray[10] * m_MatrixArray[15] - 
			m_MatrixArray[0]  * m_MatrixArray[11] * m_MatrixArray[14] - 
			m_MatrixArray[8]  * m_MatrixArray[2] * m_MatrixArray[15] + 
			m_MatrixArray[8]  * m_MatrixArray[3] * m_MatrixArray[14] + 
			m_MatrixArray[12] * m_MatrixArray[2] * m_MatrixArray[11] - 
			m_MatrixArray[12] * m_MatrixArray[3] * m_MatrixArray[10];

		invMatrix.m_MatrixArray[6] =	-m_MatrixArray[0]  * m_MatrixArray[6] * m_MatrixArray[15] + 
			m_MatrixArray[0]  * m_MatrixArray[7] * m_MatrixArray[14] + 
			m_MatrixArray[4]  * m_MatrixArray[2] * m_MatrixArray[15] - 
			m_MatrixArray[4]  * m_MatrixArray[3] * m_MatrixArray[14] - 
			m_MatrixArray[12] * m_MatrixArray[2] * m_MatrixArray[7] + 
			m_MatrixArray[12] * m_MatrixArray[3] * m_MatrixArray[6];

		invMatrix.m_MatrixArray[7] =	 m_MatrixArray[0] * m_MatrixArray[6] * m_MatrixArray[11] - 
			m_MatrixArray[0] * m_MatrixArray[7] * m_MatrixArray[10] - 
			m_MatrixArray[4] * m_MatrixArray[2] * m_MatrixArray[11] + 
			m_MatrixArray[4] * m_MatrixArray[3] * m_MatrixArray[10] + 
			m_MatrixArray[8] * m_MatrixArray[2] * m_MatrixArray[7] - 
			m_MatrixArray[8] * m_MatrixArray[3] * m_MatrixArray[6];

		invMatrix.m_MatrixArray[8] =	 m_MatrixArray[4]  * m_MatrixArray[9] * m_MatrixArray[15] - 
			m_MatrixArray[4]  * m_MatrixArray[11] * m_MatrixArray[13] - 
			m_MatrixArray[8]  * m_MatrixArray[5] * m_MatrixArray[15] + 
			m_MatrixArray[8]  * m_MatrixArray[7] * m_MatrixArray[13] + 
			m_MatrixArray[12] * m_MatrixArray[5] * m_MatrixArray[11] - 
			m_MatrixArray[12] * m_MatrixArray[7] * m_MatrixArray[9];

		invMatrix.m_MatrixArray[9] =	-m_MatrixArray[0]  * m_MatrixArray[9] * m_MatrixArray[15] + 
			m_MatrixArray[0]  * m_MatrixArray[11] * m_MatrixArray[13] + 
			m_MatrixArray[8]  * m_MatrixArray[1] * m_MatrixArray[15] - 
			m_MatrixArray[8]  * m_MatrixArray[3] * m_MatrixArray[13] - 
			m_MatrixArray[12] * m_MatrixArray[1] * m_MatrixArray[11] + 
			m_MatrixArray[12] * m_MatrixArray[3] * m_MatrixArray[9];

		invMatrix.m_MatrixArray[10] =	 m_MatrixArray[0]  * m_MatrixArray[5] * m_MatrixArray[15] - 
			m_MatrixArray[0]  * m_MatrixArray[7] * m_MatrixArray[13] - 
			m_MatrixArray[4]  * m_MatrixArray[1] * m_MatrixArray[15] + 
			m_MatrixArray[4]  * m_MatrixArray[3] * m_MatrixArray[13] + 
			m_MatrixArray[12] * m_MatrixArray[1] * m_MatrixArray[7] - 
			m_MatrixArray[12] * m_MatrixArray[3] * m_MatrixArray[5];

		invMatrix.m_MatrixArray[11] =	-m_MatrixArray[0] * m_MatrixArray[5] * m_MatrixArray[11] + 
			m_MatrixArray[0] * m_MatrixArray[7] * m_MatrixArray[9] + 
			m_MatrixArray[4] * m_MatrixArray[1] * m_MatrixArray[11] - 
			m_MatrixArray[4] * m_MatrixArray[3] * m_MatrixArray[9] - 
			m_MatrixArray[8] * m_MatrixArray[1] * m_MatrixArray[7] + 
			m_MatrixArray[8] * m_MatrixArray[3] * m_MatrixArray[5];

		invMatrix.m_MatrixArray[12] =	-m_MatrixArray[4]  * m_MatrixArray[9] * m_MatrixArray[14] + 
			m_MatrixArray[4]  * m_MatrixArray[10] * m_MatrixArray[13] +
			m_MatrixArray[8]  * m_MatrixArray[5] * m_MatrixArray[14] - 
			m_MatrixArray[8]  * m_MatrixArray[6] * m_MatrixArray[13] - 
			m_MatrixArray[12] * m_MatrixArray[5] * m_MatrixArray[10] + 
			m_MatrixArray[12] * m_MatrixArray[6] * m_MatrixArray[9];

		invMatrix.m_MatrixArray[13] =	 m_MatrixArray[0]  * m_MatrixArray[9] * m_MatrixArray[14] - 
			m_MatrixArray[0]  * m_MatrixArray[10] * m_MatrixArray[13] - 
			m_MatrixArray[8]  * m_MatrixArray[1] * m_MatrixArray[14] + 
			m_MatrixArray[8]  * m_MatrixArray[2] * m_MatrixArray[13] + 
			m_MatrixArray[12] * m_MatrixArray[1] * m_MatrixArray[10] - 
			m_MatrixArray[12] * m_MatrixArray[2] * m_MatrixArray[9];

		invMatrix.m_MatrixArray[14] =	-m_MatrixArray[0]  * m_MatrixArray[5] * m_MatrixArray[14] + 
			m_MatrixArray[0]  * m_MatrixArray[6] * m_MatrixArray[13] + 
			m_MatrixArray[4]  * m_MatrixArray[1] * m_MatrixArray[14] - 
			m_MatrixArray[4]  * m_MatrixArray[2] * m_MatrixArray[13] - 
			m_MatrixArray[12] * m_MatrixArray[1] * m_MatrixArray[6] + 
			m_MatrixArray[12] * m_MatrixArray[2] * m_MatrixArray[5];

		invMatrix.m_MatrixArray[15] =	 m_MatrixArray[0] * m_MatrixArray[5] * m_MatrixArray[10] - 
			m_MatrixArray[0] * m_MatrixArray[6] * m_MatrixArray[9] - 
			m_MatrixArray[4] * m_MatrixArray[1] * m_MatrixArray[10] + 
			m_MatrixArray[4] * m_MatrixArray[2] * m_MatrixArray[9] + 
			m_MatrixArray[8] * m_MatrixArray[1] * m_MatrixArray[6] - 
			m_MatrixArray[8] * m_MatrixArray[2] * m_MatrixArray[5];

		for(int i = 0; i < 16; i++){
			invMatrix.m_MatrixArray[i] = invMatrix.m_MatrixArray[i]/tmpDet; 
		}

		return invMatrix;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::transpose() const
	{
		Matrix4 transMatrix;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				transMatrix.m_MatrixArray[i*4+j] = m_MatrixArray[j*4+i];
			}
		}
		
		return transMatrix;
	}
	/*********************************************************************************/
	bool Matrix4::isIdentity() const
	{
		for(int i=0; i< 16; ++i)
		{
			if((i%5) == 0)
			{
				if(m_MatrixArray[i] != 1)
				{
					return false;
				}
			} else{
				if(m_MatrixArray[i] != 0)
				{
					return false;
				}
			}
		}
		return true;
	}
	/*********************************************************************************/
	bool Matrix4::isZero() const
	{
		for(int i=0;i < 16; ++i)
		{
			if(m_MatrixArray[i] != 0.0f)
			{
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::operator*(const Matrix4& p_Matrix) const
	{
		Matrix4 matrix = *this;
		matrix *= p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	Vector3 Matrix4::operator*(const Vector3& p_Vector) const
	{
		Vector3 vec(p_Vector);
		vec.x = m_MatrixArray[0]*p_Vector.x + m_MatrixArray[1]*p_Vector.y + m_MatrixArray[2]*p_Vector.z;
		vec.y = m_MatrixArray[4]*p_Vector.x + m_MatrixArray[5]*p_Vector.y + m_MatrixArray[6]*p_Vector.z;
		vec.z = m_MatrixArray[8]*p_Vector.x + m_MatrixArray[9]*p_Vector.y + m_MatrixArray[10]*p_Vector.z;
		return vec;
	}
	/*********************************************************************************/
	Vector4 Matrix4::operator*(const Vector4& p_Vector) const
	{
		Vector4 vec(p_Vector);
		vec.x = m_MatrixArray[0]*p_Vector.x + m_MatrixArray[1]*p_Vector.y + m_MatrixArray[2]*p_Vector.z + m_MatrixArray[3]*p_Vector.w;
		vec.y = m_MatrixArray[4]*p_Vector.x + m_MatrixArray[5]*p_Vector.y + m_MatrixArray[6]*p_Vector.z + m_MatrixArray[7]*p_Vector.w;
		vec.z = m_MatrixArray[8]*p_Vector.x + m_MatrixArray[9]*p_Vector.y + m_MatrixArray[10]*p_Vector.z + m_MatrixArray[11]*p_Vector.w;
		vec.w = m_MatrixArray[12]*p_Vector.x + m_MatrixArray[13]*p_Vector.y + m_MatrixArray[14]*p_Vector.z + m_MatrixArray[15]*p_Vector.w;

		vec /= vec.w;
		return vec;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::operator*(float p_Scalar) const
	{
		Matrix4 matrix = *this;

		matrix *= p_Scalar;

		return matrix;
	}
/*********************************************************************************/
	const Matrix4& Matrix4::operator*=(const Matrix4& p_Matrix)
	{
		#if defined K15_SIMD_SUPPORT
			//http://drrobsjournal.blogspot.de/2012/10/fast-simd-4x4-matrix-multiplication.html

			const __m128 a = p_Matrix.m_MatrixSIMD[0];
			const __m128 b = p_Matrix.m_MatrixSIMD[1];
			const __m128 c = p_Matrix.m_MatrixSIMD[2];
			const __m128 d = p_Matrix.m_MatrixSIMD[3];

			__m128 t1, t2;

			t1 = _mm_set1_ps(_1_1);
			t2 = _mm_mul_ps(a, t1);
			t1 =_mm_set1_ps(_1_2);
			t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
			t1 =_mm_set1_ps(_1_3);
			t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
			t1 =_mm_set1_ps(_1_4);
			t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

			_mm_store_ps(&m_MatrixArray[0], t2);

			t1 = _mm_set1_ps(_2_1);
			t2 = _mm_mul_ps(a, t1);
			t1 =_mm_set1_ps(_2_2);
			t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
			t1 =_mm_set1_ps(_2_3);
			t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
			t1 =_mm_set1_ps(_2_4);
			t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

			_mm_store_ps(&m_MatrixArray[4], t2);

			t1 = _mm_set1_ps(_3_1);
			t2 = _mm_mul_ps(a, t1);
			t1 =_mm_set1_ps(_3_2);
			t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
			t1 =_mm_set1_ps(_3_3);
			t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
			t1 =_mm_set1_ps(_3_4);
			t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

			_mm_store_ps(&m_MatrixArray[8], t2);

			t1 = _mm_set1_ps(_4_1);
			t2 = _mm_mul_ps(a, t1);
			t1 =_mm_set1_ps(_4_2);
			t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
			t1 =_mm_set1_ps(_4_3);
			t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
			t1 =_mm_set1_ps(_4_4);
			t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

			_mm_store_ps(&m_MatrixArray[12], t2);
		#endif //K15_SIMD_SUPPORT
		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator*=(float p_Scalar)
	{
		#if defined K15_SIMD_SUPPORT
		__m128 temp = _mm_set_ps(p_Scalar,p_Scalar,p_Scalar,p_Scalar);

		for(int i = 0;i < 4;++i)
		{
			m_MatrixSIMD[i] = _mm_mul_ps(m_MatrixSIMD[i],temp);
		}
		#endif //K15_SIMD_SUPPORT

		return *this;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::operator+(const Matrix4& p_Matrix) const
	{
		Matrix4 matrix = *this;
		
		matrix += p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::operator-(const Matrix4& p_Matrix) const
	{
		Matrix4 matrix = *this;

		matrix -= p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator=(const Matrix4& p_Matrix)
	{
		memcpy(m_MatrixArray,p_Matrix.m_MatrixArray,sizeof(m_MatrixArray));

		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator+=(const Matrix4& p_Matrix)
	{
		#if defined K15_SIMD_SUPPORT
			for(int i = 0; i < 4; i++)
			{
				m_MatrixSIMD[i] = _mm_add_ps(m_MatrixSIMD[i],p_Matrix.m_MatrixSIMD[i]);
			}
		#endif //K15_SIMD_SUPPORT
		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator-=(const Matrix4& p_Matrix)
	{
		#if defined K15_SIMD_SUPPORT
			for(int i = 0; i < 4; i++)
			{
				m_MatrixSIMD[i] = _mm_sub_ps(m_MatrixSIMD[i],p_Matrix.m_MatrixSIMD[i]);
			}
		#endif //K15_SIMD_SUPPORT
		return *this;
	}
	/*********************************************************************************/
	bool Matrix4::operator==(const Matrix4& p_Matrix) const
	{
		for(int i = 0; i < 16; i++)
		{
			if(m_MatrixArray[i] != p_Matrix.m_MatrixArray[i])
			{
				return false;
			}
		}
		return true;
	}
	/*********************************************************************************/
	bool Matrix4::operator!=(const Matrix4& p_Matrix) const
	{
		return !this->operator==(p_Matrix);
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace