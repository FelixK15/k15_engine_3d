/**
 * @file K15_Matrix3x4.cpp
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

#include "K15_Matrix3.h"
#include "K15_MathUtil.h"

//glm include
#include "glm/matrix.hpp"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	const Matrix3 Matrix3::Identity;
	const Matrix3 Matrix3::Zero(0.0f,0.0f,0.0f,
								0.0f,0.0f,0.0f,
								0.0f,0.0f,0.0f);
	/*********************************************************************************/

	/*********************************************************************************/
	Matrix3::Matrix3()
	{
		for(int i = 0;i < 9;++i)
		{
			m_MatrixArray[i] = 0.0f;
		}
		_1_1 = _2_2 = _3_3 = 1.0f;
	}
	/*********************************************************************************/
	Matrix3::Matrix3(float p_Values[9])
	{
		memcpy(m_MatrixArray,p_Values,sizeof(p_Values));
	}
	/*********************************************************************************/
	Matrix3::Matrix3(float _1_1,float _1_2,float _1_3,
		float _2_1,float _2_2,float _2_3,
		float _3_1,float _3_2,float _3_3)
	{
		this->_1_1 = _1_1; this->_1_2 = _1_2; this->_1_3 = _1_3;
		this->_2_1 = _2_1; this->_2_2 = _2_2; this->_2_3 = _2_3;
		this->_3_1 = _3_1; this->_3_2 = _3_2; this->_3_3 = _3_3;
	}
	/*********************************************************************************/
	Matrix3::Matrix3(const Matrix3& p_Other)
	{
		memcpy(m_MatrixArray,p_Other.m_MatrixArray,sizeof(m_MatrixArray));
	}
	/*********************************************************************************/
	Matrix3::~Matrix3()
	{

	}
	/*********************************************************************************/
	float Matrix3::determinant() const
	{
		glm::mat3 mat;
		memcpy(&mat, m_MatrixArray, sizeof(m_MatrixArray));
		return glm::determinant(mat);
	}
	/*********************************************************************************/
	Matrix3 Matrix3::inverse() const
	{
		Matrix3 invMat;
		glm::mat3 mat;
		memcpy(&mat, m_MatrixArray, sizeof(mat));

		mat = glm::inverse(mat);
		memcpy(&invMat, &mat, sizeof(mat));
		
		return invMat;
	}
	/*********************************************************************************/
	Matrix3 Matrix3::transpose() const
	{
		Matrix3 transMat;
		glm::mat3 mat;
		memcpy(&mat, m_MatrixArray, sizeof(mat));

		mat = glm::transpose(mat);
		memcpy(&transMat, &mat, sizeof(mat));
		return transMat;
	}
	/*********************************************************************************/
	Vector3 Matrix3::getXAxis() const
	{
		Vector3 xAxis;

		xAxis.x = _1_1;
		xAxis.y = _1_2;
		xAxis.z = _1_3;

		return xAxis;
	}
	/*********************************************************************************/
	Vector3 Matrix3::getYAxis() const
	{
		Vector3 yAxis;

		yAxis.x = _2_1;
		yAxis.y = _2_2;
		yAxis.z = _2_3;

		return yAxis;
	}
	/*********************************************************************************/
	Vector3 Matrix3::getZAxis() const
	{
		Vector3 zAxis;

		zAxis.x = _3_1;
		zAxis.y = _3_2;
		zAxis.z = _3_3;

		return zAxis;
	}
	/*********************************************************************************/
	bool Matrix3::isIdentity() const
	{
		for(int i=0; i< 9; ++i)
		{
			if((i%4) == 0)
			{
				if(m_MatrixArray[i] != 1.0f)
				{
					return false;
				}
			} else{
				if(m_MatrixArray[i] != 0.0f)
				{
					return false;
				}
			}
		}
		return true;
	}
	/*********************************************************************************/
	bool Matrix3::isZero() const
	{
		for(int i=0;i < 9; ++i)
		{
			if(m_MatrixArray[i] != 0.0f)
			{
				return false;
			}
		}

		return true;
	}
	/*********************************************************************************/
	Matrix3 Matrix3::operator*(const Matrix3& p_Matrix) const
	{
		Matrix3 matrix = *this;

		matrix *= p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	Vector3 Matrix3::operator*(const Vector3& p_Vector) const
	{
		Vector3 vec(p_Vector);
		vec.x = m_MatrixArray[0]*p_Vector.x + m_MatrixArray[1]*p_Vector.y + m_MatrixArray[2]*p_Vector.z;
		vec.y = m_MatrixArray[4]*p_Vector.x + m_MatrixArray[5]*p_Vector.y + m_MatrixArray[6]*p_Vector.z;
		vec.z = m_MatrixArray[8]*p_Vector.x + m_MatrixArray[9]*p_Vector.y + m_MatrixArray[10]*p_Vector.z;
		return vec;
	}
	/*********************************************************************************/
	Matrix3 Matrix3::operator*(float p_Scalar) const
	{
		Matrix3 matrix = *this;

		matrix *= p_Scalar;

		return matrix;
	}
	/*********************************************************************************/
	Matrix3 Matrix3::operator+(const Matrix3& p_Matrix) const
	{
		Matrix3 matrix = *this;

		matrix += p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	Matrix3 Matrix3::operator-(const Matrix3& p_Matrix) const
	{ 
		Matrix3 matrix = *this;

		matrix -= p_Matrix;

		return matrix;
	}
	/*********************************************************************************/
	const Matrix3& Matrix3::operator*=(const Matrix3& p_Matrix)
	{
		glm::mat3 mat, mat2;
		memcpy(&mat, m_MatrixArray, sizeof(mat));
		memcpy(&mat2, &p_Matrix, sizeof(mat2));

		mat *= mat2;

		memcpy(m_MatrixArray,&mat,sizeof(mat));

		return *this; 
	}
	/*********************************************************************************/
	const Matrix3& Matrix3::operator*=(float p_Scalar)
	{
		for(int i = 0;i < 9; ++i)
		{
			m_MatrixArray[i] *= p_Scalar;
		}

		return *this;
	}
	/*********************************************************************************/
	const Matrix3& Matrix3::operator=(const Matrix3& p_Matrix)
	{
		memcpy(m_MatrixArray, p_Matrix.m_MatrixArray, sizeof(Matrix3));

		return *this;
	}
	/*********************************************************************************/
	const Matrix3& Matrix3::operator+=(const Matrix3& p_Matrix)
	{
		for(int i = 0;i < 9;++i)
		{
			m_MatrixArray[i] += p_Matrix.m_MatrixArray[i];
		}

		return *this;
	}
	/*********************************************************************************/
	const Matrix3& Matrix3::operator-=(const Matrix3& p_Matrix)
	{
		for(int i = 0;i < 9;++i)
		{
			m_MatrixArray[i] -= p_Matrix.m_MatrixArray[i];
		}

		return *this;
	}
	/*********************************************************************************/
	bool Matrix3::operator==(const Matrix3& p_Matrix)
	{
		for(int i = 0; i < 9; i++)
		{
			if(m_MatrixArray[i] != p_Matrix.m_MatrixArray[i])
			{
				return false;
			}
		}
		return true;
	}
	/*********************************************************************************/
	bool Matrix3::operator!=(const Matrix3& p_Matrix)
	{
		return !this->operator==(p_Matrix);
	}
	/*********************************************************************************/
	float Matrix3::operator[](int p_Index) const
	{
		if(p_Index < 0) p_Index = 0;
		if(p_Index > 8) p_Index = 8;

		return m_MatrixArray[p_Index];
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace