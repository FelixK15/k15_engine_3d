/**
 * @file K15_Matrix3x3<Real>.cpp
 * @author	Tobias Funke <tobyfunke@web.de>
 * @version 1.0
 * @date 2012/07/12
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


template<class Real>
Matrix3x3<Real>::Matrix3x3(MatrixFlags flag)
{
	if(flag==MF_ZERO){
		Zero();
	}

	if(flag==MF_IDENT){
		Identity();
	}
}

template<class Real>
Matrix3x3<Real>::Matrix3x3( Real mat[9] )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = mat[i];
	}
}

template<class Real>
Matrix3x3<Real>::Matrix3x3( Real _1_1,Real _1_2,Real _1_3,
					  Real _2_1,Real _2_2,Real _2_3,
					  Real _3_1,Real _3_2,Real _3_3)
{
	Real tmpMat[9]= {	_1_1, _1_2, _1_3,
						_2_1, _2_2, _2_3,
						_3_1, _3_2, _3_3};
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = tmpMat[i];
	}
}

template<class Real>
Matrix3x3<Real>::Matrix3x3( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = mat.m_fMatrix[i];
	}
}

template<class Real>
Matrix3x3<Real>::~Matrix3x3()
{
	
}

template<class Real>
void Matrix3x3<Real>::Identity()
{
	Zero();
	_1_1 = 1;
	_2_2 = 1;
	_3_3 = 1;
}

template<class Real>
void Matrix3x3<Real>::Zero()
{
	for(int i=0; i < 9; i++){
		m_fMatrix[i]=0;
	}
}

template<class Real>
bool Matrix3x3<Real>::IsLowerTriangle() const
{
	if(m_fMatrix[1] != 0) return false;
	if(m_fMatrix[2] != 0) return false;
	if(m_fMatrix[5] != 0) return false;
	return true;
}

template<class Real>
bool Matrix3x3<Real>::IsUpperTriangle() const
{
	if(m_fMatrix[3] != 0) return false;
	if(m_fMatrix[6] != 0) return false;
	if(m_fMatrix[7] != 0) return false;
	return true;
}

template<class Real>
bool Matrix3x3<Real>::IsTriangle() const
{
	return (IsUpperTriangle() || IsLowerTriangle());
}

template<class Real>
bool Matrix3x3<Real>::HasZeroVector() const
{
	int count1 = 0;
	int count2 = 0;
	for(int i = 0; i < 3; i++){
		count1 = 0;
		count2 = 0;
		for(int j = 0; j < 3; j++){
			if(m_fMatrix[i*3+j] == 0) count1++;
			if(m_fMatrix[i+j*3] == 0) count2++;
		}
		if(count1 == 0 && count2 == 0) return false;
		if(count1 == 3 || count2 == 3) return true;
	}
	return false;
}

template<class Real>
int Matrix3x3<Real>::GetMaxDiagonalColumn() const
{
	int iColumn = 0;
	Real fMax = _1_1;
	if(m_fMatrix[4]>fMax){
		fMax = m_fMatrix[4];
		iColumn = 1;
	}
	if(m_fMatrix[8]>fMax){
		iColumn = 2;
	}
	return iColumn;
}

template<class Real>
Matrix3x3<Real> Matrix3x3<Real>::RowChange(Matrix3x3<Real> &mat, int row1, int row2){
	Real tmpValue;
	for(int i = 0; i < 3; i++){
		tmpValue = mat.m_fMatrix[row1 + (i*3)];
		mat.m_fMatrix[row1 + (i*3)] = mat.m_fMatrix[row2 + (i*3)];
		mat.m_fMatrix[row2 + (i*3)] = tmpValue;
	}

	return mat;
}

template<class Real>
Matrix3x3<Real> Matrix3x3<Real>::ColumnChange(Matrix3x3<Real> &mat, int column1, int column2){
	Real tmpValue;
	for(int i = 0; i < 3; i++){
		tmpValue = mat.m_fMatrix[i + (column1*3)];
		mat.m_fMatrix[i + (column1*3)] = mat.m_fMatrix[i + (column2*3)];
		mat.m_fMatrix[i + (column2*3)] = tmpValue;
	}

	return mat;
}

template<class Real>
Real Matrix3x3<Real>::Determinant() const
{
	return m_fMatrix[0]*m_fMatrix[4]*m_fMatrix[8] + m_fMatrix[1]*m_fMatrix[5]*m_fMatrix[6] + m_fMatrix[2]*m_fMatrix[3]*m_fMatrix[7]
		   - m_fMatrix[0]*m_fMatrix[5]*m_fMatrix[7] - m_fMatrix[1]*m_fMatrix[3]*m_fMatrix[8] - m_fMatrix[2]*m_fMatrix[4]*m_fMatrix[6];
}

template<class Real>
void Matrix3x3<Real>::Inverse()
{
	Real tmpDet = Determinant();
	if(tmpDet == 0){
		// @todo: Fehlerbehandlung bei nicht invertierbarer Matrix
		return;
	}
	m_fMatrix[0] = (m_fMatrix[4]*m_fMatrix[8] - m_fMatrix[5]*m_fMatrix[7])/tmpDet;
	m_fMatrix[1] = (m_fMatrix[2]*m_fMatrix[7] - m_fMatrix[1]*m_fMatrix[8])/tmpDet;
	m_fMatrix[2] = (m_fMatrix[1]*m_fMatrix[5] - m_fMatrix[2]*m_fMatrix[4])/tmpDet;
	m_fMatrix[3] = (m_fMatrix[5]*m_fMatrix[6] - m_fMatrix[3]*m_fMatrix[8])/tmpDet;
	m_fMatrix[4] = (m_fMatrix[0]*m_fMatrix[8] - m_fMatrix[2]*m_fMatrix[6])/tmpDet;
	m_fMatrix[5] = (m_fMatrix[2]*m_fMatrix[3] - m_fMatrix[0]*m_fMatrix[5])/tmpDet;
	m_fMatrix[6] = (m_fMatrix[3]*m_fMatrix[7] - m_fMatrix[4]*m_fMatrix[6])/tmpDet;
	m_fMatrix[7] = (m_fMatrix[1]*m_fMatrix[6] - m_fMatrix[0]*m_fMatrix[7])/tmpDet;
	m_fMatrix[8] = (m_fMatrix[0]*m_fMatrix[4] - m_fMatrix[1]*m_fMatrix[3])/tmpDet;
}

template<class Real>
void Matrix3x3<Real>::Transpose()
{
	Matrix3x3<Real> tempMat;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			tempMat.m_fMatrix[i*3+j] = m_fMatrix[j*3+i];
		}
	}
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = tempMat.m_fMatrix[i];
	}
}

template<class Real>
void Matrix3x3<Real>::Rotate( const Vector3<Real> &vec,Real angleInRad )
{
	
}

template<class Real>
void Matrix3x3<Real>::Scale( const Vector3<Real> &scaleVec )
{
	
}

template<class Real>
void Matrix3x3<Real>::Translate( const Vector3<Real> &posVec )
{
	
}

template<class Real>
bool Matrix3x3<Real>::IsIdentity() const
{
	for(int i=0; i< 9; i++){
		if((i%4) == 0){
			if(m_fMatrix[i] != 1){
				return false;
			}
		} else{
			if(m_fMatrix[i] != 0){
				return false;
			}
		}
	}
	return true;
}

template<class Real>
bool Matrix3x3<Real>::IsZero() const
{
	for(int i = 0; i < 9; i++){
		if(m_fMatrix[i]!=0){
			return false;
		}
	}
	return true;
}

template<class Real>
Matrix3x3<Real> Matrix3x3<Real>::operator*( const Matrix3x3<Real> &mat )
{
	Real tmpMat[9];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			tmpMat[i*3+j] = m_fMatrix[i*3]*mat.m_fMatrix[j] + m_fMatrix[i*3+1]*mat.m_fMatrix[j+3] + m_fMatrix[i*3+2]*mat.m_fMatrix[j+6];		
		}
	}
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = tmpMat[i];
	}
	return *this;
}

template<class Real>
Vector3<Real> Matrix3x3<Real>::operator*( const Vector3<Real> &vec ) const
{
	Vector3<Real> tmpVec(vec);
	tmpVec.x = m_fMatrix[0]*vec.x + m_fMatrix[1]*vec.y + m_fMatrix[2]*vec.z;
	tmpVec.y = m_fMatrix[3]*vec.x + m_fMatrix[4]*vec.y + m_fMatrix[5]*vec.z;
	tmpVec.z = m_fMatrix[6]*vec.x + m_fMatrix[7]*vec.y + m_fMatrix[8]*vec.z;
	return tmpVec;
}

template<class Real>
Matrix3x3<Real> Matrix3x3<Real>::operator*( Real scalar )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] *= scalar;
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator+( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] += mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator-( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] -= mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator-()
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] = -m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator=(const Matrix3x3<Real> &mat)
{
	for(int i = 0; i < 9; i++)
	{
		m_fMatrix[i] = mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator+=( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] += mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix3x3<Real> &Matrix3x3<Real>::operator-=( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		m_fMatrix[i] -= mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
bool Matrix3x3<Real>::operator==( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		if(m_fMatrix[i] != mat.m_fMatrix[i]){
			return false;
		}
	}
	return true;
}

template<class Real>
bool Matrix3x3<Real>::operator!=( const Matrix3x3<Real> &mat )
{
	for(int i = 0; i < 9; i++){
		if(m_fMatrix[i] != mat.m_fMatrix[i]){
			return true;
		}
	}
	return false;
}