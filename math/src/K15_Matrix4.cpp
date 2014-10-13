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

#include "glm/matrix.hpp"

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
		memset(&m_MatrixArray,0,sizeof(Matrix4));
		m_MatrixArray[0] = m_MatrixArray[5] = m_MatrixArray[10] = m_MatrixArray[15] = 1.0f;
	}
	/*********************************************************************************/
	Matrix4::Matrix4(float p_Values[16])
	{
		memcpy(m_MatrixArray,p_Values,sizeof(Matrix4));
	}
	/*********************************************************************************/
	Matrix4::Matrix4(float _1_1,float _1_2,float _1_3,float _1_4,
		float _2_1,float _2_2,float _2_3,float _2_4,
		float _3_1,float _3_2,float _3_3,float _3_4,
		float _4_1,float _4_2,float _4_3,float _4_4)
	{
		this->_1_1 = _1_1; this->_1_2 = _1_2; this->_1_3 = _1_3; this->_1_4 = _1_4;
		this->_2_1 = _2_1; this->_2_2 = _2_2; this->_2_3 = _2_3; this->_2_4 = _2_4;
		this->_3_1 = _3_1; this->_3_2 = _3_2; this->_3_3 = _3_3; this->_3_4 = _3_4;
		this->_4_1 = _4_1; this->_4_2 = _4_2; this->_4_3 = _4_3; this->_4_4 = _4_4;
	}
	/*********************************************************************************/
	Matrix4::Matrix4(const Matrix4& p_Matrix)
	{
		memcpy(m_MatrixArray,p_Matrix.m_MatrixArray,sizeof(Matrix4));
	}
	/*********************************************************************************/
	Matrix4::~Matrix4()
	{

	}
	/*********************************************************************************/
	float Matrix4::determinant() const
	{
		glm::mat4 mat;
		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));

		return glm::determinant(mat);
	}
	/*********************************************************************************/
	Matrix4 Matrix4::inverse() const
	{
 		Matrix4 invMat;
		glm::mat4 mat;
		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));

		mat = glm::inverse(mat);
		memcpy(&invMat, &mat, sizeof(Matrix4));

		return invMat;
	}
	/*********************************************************************************/
	Matrix4 Matrix4::transpose() const
	{
		Matrix4 transMat;
		glm::mat4 mat;
		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));

		mat = glm::transpose(mat);
		memcpy(&transMat, &mat, sizeof(Matrix4));

		return transMat;
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
		Vector3 transformedVec;
		glm::vec4 vec;
		glm::mat4 mat;

		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));
		memcpy(&vec, &p_Vector, sizeof(Vector3));
		vec.w = 1.f;
		vec = mat * vec;

		transformedVec.x = vec.x;
		transformedVec.y = vec.y;
		transformedVec.z = vec.z;

		return transformedVec;
	}
	/*********************************************************************************/
	Vector4 Matrix4::operator*(const Vector4& p_Vector) const
	{
		Vector4 transformedVec;
		glm::vec4 vec;
		glm::mat4 mat;

		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));
		memcpy(&vec, &p_Vector, sizeof(Vector4));

		vec = mat * vec;

		memcpy(&transformedVec, &vec, sizeof(Vector4));

		return transformedVec;
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
		glm::mat4 mat;
		glm::mat4 mat2;

		memcpy(&mat, m_MatrixArray, sizeof(Matrix4));
		memcpy(&mat2, &p_Matrix, sizeof(Matrix4));

		mat *= mat2;

		memcpy(m_MatrixArray, &mat, sizeof(Matrix4));

		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator*=(float p_Scalar)
	{
		for(int i = 0; i < 16; ++i)
		{
			m_MatrixArray[i] *= p_Scalar;
		}

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
		memcpy(m_MatrixArray,p_Matrix.m_MatrixArray,sizeof(Matrix4));

		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator+=(const Matrix4& p_Matrix)
	{
		for(int i = 0; i < 16; ++i)
		{
			m_MatrixArray[i] += p_Matrix[i];
		}
		return *this;
	}
	/*********************************************************************************/
	const Matrix4& Matrix4::operator-=(const Matrix4& p_Matrix)
	{
		for(int i = 0; i < 16; ++i)
		{
		  m_MatrixArray[i] -= p_Matrix[i];
		}
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
	float Matrix4::operator[](int p_Index) const
	{
		if(p_Index < 0)   p_Index = 0;
		if(p_Index > 15)  p_Index = 15;

		return m_MatrixArray[p_Index];
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace