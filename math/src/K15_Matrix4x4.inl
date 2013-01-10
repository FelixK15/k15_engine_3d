/**
 * @file K15_Matrix4x4.inl
 * @author  Tobias Funke <t.funke@k15games.de>
 * @version 0.8
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
Matrix4x4<Real>::Matrix4x4(MatrixFlags flag)
{
	if(flag==MF_ZERO){
		Zero();
	}
	
	if(flag==MF_IDENT){
		Identity();
	}
}

template<class Real>
Matrix4x4<Real>::Matrix4x4(Real mat[16])
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = mat[i];
	}
}


template<class Real>
Matrix4x4<Real>::Matrix4x4( Real _1_1,Real _1_2,Real _1_3,Real _1_4, 
					  Real _2_1,Real _2_2,Real _2_3,Real _2_4, 
					  Real _3_1,Real _3_2,Real _3_3,Real _3_4, 
					  Real _4_1,Real _4_2,Real _4_3,Real _4_4 )
{
	Real tmpMat[16]= {	_1_1, _1_2, _1_3, _1_4,
						_2_1, _2_2, _2_3, _2_4,
						_3_1, _3_2, _3_3, _3_4,
						_4_1, _4_2, _4_3, _4_4};
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = tmpMat[i];
	}
}

template<class Real>
Matrix4x4<Real>::Matrix4x4(const Matrix4x4<Real> &mat)
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = mat.m_fMatrix[i];
	}
}

template<class Real>
Matrix4x4<Real>::~Matrix4x4()
{
	
}

template<class Real>
void Matrix4x4<Real>::Identity()
{
	Zero();
	_1_1 = 1;
	_2_2 = 1;
	_3_3 = 1;
	_4_4 = 1;
}

template<class Real>
void Matrix4x4<Real>::Zero()
{
	for(int i=0; i<16; i++){
		m_fMatrix[i]=0;
	}
}

template<class Real>
bool Matrix4x4<Real>::IsLowerTriangle() const
{
	if(m_fMatrix[1] != 0) return false;
	if(m_fMatrix[2] != 0) return false;
	if(m_fMatrix[3] != 0) return false;
	if(m_fMatrix[6] != 0) return false;
	if(m_fMatrix[7] != 0) return false;
	if(m_fMatrix[11] != 0) return false;
	return true;
}

template<class Real>
bool Matrix4x4<Real>::IsUpperTriangle() const
{
	if(m_fMatrix[4] != 0) return false;
	if(m_fMatrix[8] != 0) return false;
	if(m_fMatrix[9] != 0) return false;
	if(m_fMatrix[12] != 0) return false;
	if(m_fMatrix[13] != 0) return false;
	if(m_fMatrix[14] != 0) return false;
	return true;
}

template<class Real>
bool Matrix4x4<Real>::IsTriangle() const
{
	return (IsUpperTriangle() || IsLowerTriangle());
}

template<class Real>
bool Matrix4x4<Real>::HasZeroVector() const
{
	int count1 = 0;
	int count2 = 0;
	for(int i = 0; i < 4; i++){
		count1 = 0;
		count2 = 0;
		for(int j = 0; j < 4; j++){
			if(m_fMatrix[i*4+j] == 0) count1++;
			if(m_fMatrix[i+j*4] == 0) count2++;
		}
		if(count1 == 0 && count2 == 0) return false;
		if(count1 == 4 || count2 == 4) return true;
	}
	return false;
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::RowChange(Matrix4x4<Real> &mat, int row1, int row2)
{
	Real tmpValue;
	for(int i = 0; i < 4; i++){
		tmpValue = mat.m_fMatrix[row1 + (i*4)];
		mat.m_fMatrix[row1 + (i*4)] = mat.m_fMatrix[row2 + (i*4)];
		mat.m_fMatrix[row2 + (i*4)] = tmpValue;
	}
	return mat;
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::ColumnChange(Matrix4x4<Real> &mat, int column1, int column2)
{
	Real tmpValue;
	for(int i = 0; i < 4; i++){
		tmpValue = mat.m_fMatrix[i + (column1*4)];
		mat.m_fMatrix[i + (column1*4)] = mat.m_fMatrix[i + (column2*4)];
		mat.m_fMatrix[i + (column2*4)] = tmpValue;
	}
	return mat;
}

template<class Real>
void Matrix4x4<Real>::SubMat(Matrix4x4<Real> mat4, Matrix3x3<Real> &mat3, int column, int row) const
{
	int tmpRow = 0;
	int tmpColumn = 0;

	for(int cntRow = 0; cntRow < 4; cntRow++)
	{
		if(cntRow < row) tmpRow = cntRow;
		else if(cntRow > row) tmpRow = cntRow-1;

		for(int cntColumn = 0; cntColumn < 4; cntColumn++)
		{
			if(cntColumn < column)	tmpColumn = cntColumn;
			else if(cntColumn > column) tmpColumn = cntColumn-1;

			if((cntRow != row) && (cntColumn != column))
				mat3.m_fMatrix[tmpRow*3 + tmpColumn] = mat4.m_fMatrix[cntRow*4 + cntColumn];
		}
	}
}

template<class Real>
void Matrix4x4<Real>::RotationMatrixFromEuler(Matrix4x4<Real> &mat, Vector3<Real> &eulerAngles)
{
	Real x = MatheF::DegToRad(eulerAngles.x);
	Real y = MatheF::DegToRad(eulerAngles.y);
	Real z = MatheF::DegToRad(eulerAngles.z);
	mat.m_fMatrix[0]  = MatheF::RadToDeg(cos(y)*cos(z));
	mat.m_fMatrix[1]  = MatheF::RadToDeg(-cos(y) * sin(z));
	mat.m_fMatrix[2]  = MatheF::RadToDeg(-sin(y));
	mat.m_fMatrix[4]  = MatheF::RadToDeg(-sin(x)*sin(y) * cos(z) + cos(x) * sin(z));
	mat.m_fMatrix[5]  = MatheF::RadToDeg(sin(x)*sin(y) * sin(z) + cos(x) * cos(z));
	mat.m_fMatrix[6]  = MatheF::RadToDeg(-sin(x) * cos(y));
	mat.m_fMatrix[8]  = MatheF::RadToDeg(cos(x)*sin(y) * cos(z) + sin(x) * sin(z));
	mat.m_fMatrix[9]  = MatheF::RadToDeg(-(cos(x)*sin(y)) * sin(z) + sin(x) * cos(z));
	mat.m_fMatrix[10] = MatheF::RadToDeg(cos(x) * cos(y));

	mat.m_fMatrix[3]  =  mat.m_fMatrix[7] = mat.m_fMatrix[11] = mat.m_fMatrix[12] = mat.m_fMatrix[13] = mat.m_fMatrix[14] = 0;
	mat.m_fMatrix[15] =  1;
}

template<class Real>
Real Matrix4x4<Real>::Determinant() const
{
	if(HasZeroVector()) return 0;
	if(IsTriangle()) return _1_1*_2_2*_3_3*_4_4;
	Real det = 0;
	Real switcher = 1;
	Matrix3x3<Real> mat3;
	for(int i = 0; i < 4; i++){
		SubMat(*this, mat3, i, 0);
		det += m_fMatrix[i] * (Real)mat3.Determinant() * switcher;
		switcher *= (Real)-1;
	}
	return det;
}

template<class Real>
void Matrix4x4<Real>::Inverse()
{
	Real tmpDet = Determinant();
	assert(tmpDet != 0);

	Matrix4x4<Real> invMat;
	invMat.m_fMatrix[0] =	 m_fMatrix[5]  * m_fMatrix[10] * m_fMatrix[15] - 
							 m_fMatrix[5]  * m_fMatrix[11] * m_fMatrix[14] - 
							 m_fMatrix[9]  * m_fMatrix[6]  * m_fMatrix[15] + 
							 m_fMatrix[9]  * m_fMatrix[7]  * m_fMatrix[14] +
							 m_fMatrix[13] * m_fMatrix[6]  * m_fMatrix[11] - 
							 m_fMatrix[13] * m_fMatrix[7]  * m_fMatrix[10];

	invMat.m_fMatrix[1] =	-(m_fMatrix[1])  * m_fMatrix[10] * m_fMatrix[15] + 
							 m_fMatrix[1]  * m_fMatrix[11] * m_fMatrix[14] + 
							 m_fMatrix[9]  * m_fMatrix[2] * m_fMatrix[15] - 
							 m_fMatrix[9]  * m_fMatrix[3] * m_fMatrix[14] - 
							 m_fMatrix[13] * m_fMatrix[2] * m_fMatrix[11] + 
							 m_fMatrix[13] * m_fMatrix[3] * m_fMatrix[10];

	invMat.m_fMatrix[2] =	 m_fMatrix[1]  * m_fMatrix[6] * m_fMatrix[15] - 
							 m_fMatrix[1]  * m_fMatrix[7] * m_fMatrix[14] - 
							 m_fMatrix[5]  * m_fMatrix[2] * m_fMatrix[15] + 
							 m_fMatrix[5]  * m_fMatrix[3] * m_fMatrix[14] + 
							 m_fMatrix[13] * m_fMatrix[2] * m_fMatrix[7] - 
							 m_fMatrix[13] * m_fMatrix[3] * m_fMatrix[6];

	invMat.m_fMatrix[3] =	-m_fMatrix[1] * m_fMatrix[6] * m_fMatrix[11] + 
							 m_fMatrix[1] * m_fMatrix[7] * m_fMatrix[10] + 
							 m_fMatrix[5] * m_fMatrix[2] * m_fMatrix[11] - 
							 m_fMatrix[5] * m_fMatrix[3] * m_fMatrix[10] - 
							 m_fMatrix[9] * m_fMatrix[2] * m_fMatrix[7] + 
							 m_fMatrix[9] * m_fMatrix[3] * m_fMatrix[6];

	invMat.m_fMatrix[4] =	-m_fMatrix[4]  * m_fMatrix[10] * m_fMatrix[15] + 
							 m_fMatrix[4]  * m_fMatrix[11] * m_fMatrix[14] + 
							 m_fMatrix[8]  * m_fMatrix[6]  * m_fMatrix[15] - 
							 m_fMatrix[8]  * m_fMatrix[7]  * m_fMatrix[14] - 
							 m_fMatrix[12] * m_fMatrix[6]  * m_fMatrix[11] + 
							 m_fMatrix[12] * m_fMatrix[7]  * m_fMatrix[10];

	invMat.m_fMatrix[5] =	 m_fMatrix[0]  * m_fMatrix[10] * m_fMatrix[15] - 
							 m_fMatrix[0]  * m_fMatrix[11] * m_fMatrix[14] - 
							 m_fMatrix[8]  * m_fMatrix[2] * m_fMatrix[15] + 
							 m_fMatrix[8]  * m_fMatrix[3] * m_fMatrix[14] + 
							 m_fMatrix[12] * m_fMatrix[2] * m_fMatrix[11] - 
							 m_fMatrix[12] * m_fMatrix[3] * m_fMatrix[10];

	invMat.m_fMatrix[6] =	-m_fMatrix[0]  * m_fMatrix[6] * m_fMatrix[15] + 
							 m_fMatrix[0]  * m_fMatrix[7] * m_fMatrix[14] + 
							 m_fMatrix[4]  * m_fMatrix[2] * m_fMatrix[15] - 
							 m_fMatrix[4]  * m_fMatrix[3] * m_fMatrix[14] - 
							 m_fMatrix[12] * m_fMatrix[2] * m_fMatrix[7] + 
							 m_fMatrix[12] * m_fMatrix[3] * m_fMatrix[6];

	invMat.m_fMatrix[7] =	 m_fMatrix[0] * m_fMatrix[6] * m_fMatrix[11] - 
							 m_fMatrix[0] * m_fMatrix[7] * m_fMatrix[10] - 
							 m_fMatrix[4] * m_fMatrix[2] * m_fMatrix[11] + 
							 m_fMatrix[4] * m_fMatrix[3] * m_fMatrix[10] + 
							 m_fMatrix[8] * m_fMatrix[2] * m_fMatrix[7] - 
							 m_fMatrix[8] * m_fMatrix[3] * m_fMatrix[6];

	invMat.m_fMatrix[8] =	 m_fMatrix[4]  * m_fMatrix[9] * m_fMatrix[15] - 
							 m_fMatrix[4]  * m_fMatrix[11] * m_fMatrix[13] - 
							 m_fMatrix[8]  * m_fMatrix[5] * m_fMatrix[15] + 
							 m_fMatrix[8]  * m_fMatrix[7] * m_fMatrix[13] + 
							 m_fMatrix[12] * m_fMatrix[5] * m_fMatrix[11] - 
							 m_fMatrix[12] * m_fMatrix[7] * m_fMatrix[9];

	invMat.m_fMatrix[9] =	-m_fMatrix[0]  * m_fMatrix[9] * m_fMatrix[15] + 
							 m_fMatrix[0]  * m_fMatrix[11] * m_fMatrix[13] + 
							 m_fMatrix[8]  * m_fMatrix[1] * m_fMatrix[15] - 
							 m_fMatrix[8]  * m_fMatrix[3] * m_fMatrix[13] - 
							 m_fMatrix[12] * m_fMatrix[1] * m_fMatrix[11] + 
							 m_fMatrix[12] * m_fMatrix[3] * m_fMatrix[9];

	invMat.m_fMatrix[10] =	 m_fMatrix[0]  * m_fMatrix[5] * m_fMatrix[15] - 
							 m_fMatrix[0]  * m_fMatrix[7] * m_fMatrix[13] - 
							 m_fMatrix[4]  * m_fMatrix[1] * m_fMatrix[15] + 
							 m_fMatrix[4]  * m_fMatrix[3] * m_fMatrix[13] + 
							 m_fMatrix[12] * m_fMatrix[1] * m_fMatrix[7] - 
							 m_fMatrix[12] * m_fMatrix[3] * m_fMatrix[5];

	invMat.m_fMatrix[11] =	-m_fMatrix[0] * m_fMatrix[5] * m_fMatrix[11] + 
							 m_fMatrix[0] * m_fMatrix[7] * m_fMatrix[9] + 
							 m_fMatrix[4] * m_fMatrix[1] * m_fMatrix[11] - 
							 m_fMatrix[4] * m_fMatrix[3] * m_fMatrix[9] - 
							 m_fMatrix[8] * m_fMatrix[1] * m_fMatrix[7] + 
							 m_fMatrix[8] * m_fMatrix[3] * m_fMatrix[5];

	invMat.m_fMatrix[12] =	-m_fMatrix[4]  * m_fMatrix[9] * m_fMatrix[14] + 
							 m_fMatrix[4]  * m_fMatrix[10] * m_fMatrix[13] +
							 m_fMatrix[8]  * m_fMatrix[5] * m_fMatrix[14] - 
							 m_fMatrix[8]  * m_fMatrix[6] * m_fMatrix[13] - 
							 m_fMatrix[12] * m_fMatrix[5] * m_fMatrix[10] + 
							 m_fMatrix[12] * m_fMatrix[6] * m_fMatrix[9];

    invMat.m_fMatrix[13] =	 m_fMatrix[0]  * m_fMatrix[9] * m_fMatrix[14] - 
							 m_fMatrix[0]  * m_fMatrix[10] * m_fMatrix[13] - 
							 m_fMatrix[8]  * m_fMatrix[1] * m_fMatrix[14] + 
							 m_fMatrix[8]  * m_fMatrix[2] * m_fMatrix[13] + 
							 m_fMatrix[12] * m_fMatrix[1] * m_fMatrix[10] - 
							 m_fMatrix[12] * m_fMatrix[2] * m_fMatrix[9];

    invMat.m_fMatrix[14] =	-m_fMatrix[0]  * m_fMatrix[5] * m_fMatrix[14] + 
							 m_fMatrix[0]  * m_fMatrix[6] * m_fMatrix[13] + 
							 m_fMatrix[4]  * m_fMatrix[1] * m_fMatrix[14] - 
							 m_fMatrix[4]  * m_fMatrix[2] * m_fMatrix[13] - 
							 m_fMatrix[12] * m_fMatrix[1] * m_fMatrix[6] + 
							 m_fMatrix[12] * m_fMatrix[2] * m_fMatrix[5];

    invMat.m_fMatrix[15] =	 m_fMatrix[0] * m_fMatrix[5] * m_fMatrix[10] - 
							 m_fMatrix[0] * m_fMatrix[6] * m_fMatrix[9] - 
							 m_fMatrix[4] * m_fMatrix[1] * m_fMatrix[10] + 
							 m_fMatrix[4] * m_fMatrix[2] * m_fMatrix[9] + 
							 m_fMatrix[8] * m_fMatrix[1] * m_fMatrix[6] - 
							 m_fMatrix[8] * m_fMatrix[2] * m_fMatrix[5];

	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = invMat.m_fMatrix[i]/tmpDet; 
	}

}

template<class Real>
void Matrix4x4<Real>::Transpose()
{
	Matrix4x4<Real> tempMat;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			tempMat.m_fMatrix[i*4+j] = m_fMatrix[j*4+i];
		}
	}
	memcpy(&m_fMatrix,&tempMat,sizeof(Matrix4x4<Real>));
}

template<class Real>
bool Matrix4x4<Real>::IsIdentity() const
{
	for(int i=0; i< 16; i++){
		if((i%5) == 0){
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
bool Matrix4x4<Real>::IsZero() const
{
	for(int i = 0; i < 16; i++){
		if(m_fMatrix[i]!=0){
			return false;
		}
	}
	return true;
}

template<class Real>
const Matrix4x4<Real> &Matrix4x4<Real>::operator*( const Matrix4x4<Real> &mat )
{
	Real tmpMat[16];
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			tmpMat[i*4+j] = m_fMatrix[i*4]*mat.m_fMatrix[j] + m_fMatrix[i*4+1]*mat.m_fMatrix[j+4] + m_fMatrix[i*4+2]*mat.m_fMatrix[j+8] + m_fMatrix[i*4+3]*mat.m_fMatrix[j+12];		
		}
	}
// 	for(int i = 0; i < 16; i++){
// 		m_fMatrix[i] = tmpMat[i];
// 	}
	memcpy(&m_fMatrix,&tmpMat,sizeof(Matrix4x4<Real>));
	return *this;
}

template<class Real>
Vector3<Real> Matrix4x4<Real>::operator*( const Vector3<Real> &vec )
{
	Vector3<Real> tmpVec(vec);
	tmpVec.x = m_fMatrix[0]*vec.x + m_fMatrix[1]*vec.y + m_fMatrix[2]*vec.z;
	tmpVec.y = m_fMatrix[4]*vec.x + m_fMatrix[5]*vec.y + m_fMatrix[6]*vec.z;
	tmpVec.z = m_fMatrix[8]*vec.x + m_fMatrix[9]*vec.y + m_fMatrix[10]*vec.z;
	return tmpVec;
}

template<class Real>
const Matrix4x4<Real> &Matrix4x4<Real>::operator*( Real scalar )
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] *= scalar;
	}
	return *this;
}

template<class Real>
void Matrix4x4<Real>::operator*=( const Matrix4x4<Real> &mat )
{
	this->operator*(mat);
}

template<class Real>
void Matrix4x4<Real>::operator*=( Real fScalar )
{
	this->operator*(fScalar);
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator+( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] += mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator-( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] -= mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator-()
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = -m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator=(const Matrix4x4<Real> &mat)
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] = mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator+=( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] += mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator-=( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		m_fMatrix[i] -= mat.m_fMatrix[i];
	}
	return *this;
}

template<class Real>
bool Matrix4x4<Real>::operator==( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		if(m_fMatrix[i] != mat.m_fMatrix[i]){
			return false;
		}
	}
	return true;
}

template<class Real>
bool Matrix4x4<Real>::operator!=( const Matrix4x4<Real> &mat )
{
	for(int i = 0; i < 16; i++){
		if(m_fMatrix[i] != mat.m_fMatrix[i]){
			return true;
		}
	}
	return false;
}

/* mit Vorsicht geniessen
template<class Real>
void Matrix4x4<Real>::ToQuaternion(Quaternion* pQuaternion) const
{
	Real fTrace = m_fMatrix[0] + m_fMatrix[5] + m_fMatrix[10] + 1;
	Real fW, fX, fY, fZ, fTmp;
	if(fTrace > 0){
		fTmp = 0.5f / sqrt(fTrace);
		fW = 0.25f / fTmp;
		fX = (m_fMatrix[9]*fTmp - m_fMatrix[6]*fTmp);
		fY = (m_fMatrix[2] - m_fMatrix[8]) * fTmp;
		fZ = (m_fMatrix[4] - m_fMatrix[1]) * fTmp;
	} else{
		Matrix3x3 matTmp3x3;
		SubMat(*this, matTmp3x3, 3,3);
		int iMaxColumn = matTmp3x3.GetMaxDiagonalColumn();
		if(iMaxColumn == 0){
			fTmp  = sqrt(1.0f + m_fMatrix[0] - m_fMatrix[5] - m_fMatrix[10]) * 2;
			fX = 0.5f / fTmp;
			fY = (m_fMatrix[1] + m_fMatrix[4] ) / fTmp;
			fZ = (m_fMatrix[8] + m_fMatrix[2] ) / fTmp;
			fW = (m_fMatrix[6] + m_fMatrix[9] ) / fTmp;
		}
		if(iMaxColumn == 1){
			fTmp  = sqrt(1.0f + m_fMatrix[5] - m_fMatrix[0] - m_fMatrix[10]) * 2;
			fX = (m_fMatrix[1] + m_fMatrix[4] ) / fTmp;
			fY = 0.5f / fTmp;
			fZ = (m_fMatrix[6] + m_fMatrix[9] ) / fTmp;
			fW = (m_fMatrix[2] + m_fMatrix[8] ) / fTmp;
		}
		if(iMaxColumn == 2){
			fTmp  = sqrt(1.0f + m_fMatrix[10] - m_fMatrix[0] - m_fMatrix[5]) * 2;
			fX = (m_fMatrix[2] + m_fMatrix[8] ) / fTmp;
			fY = (m_fMatrix[6] + m_fMatrix[9] ) / fTmp;
			fZ = 0.5f / fTmp;
			fW = (m_fMatrix[1] + m_fMatrix[4] ) / fTmp;
		}
	}
	pQuaternion->m_fValues[0] = fW;
	pQuaternion->m_fValues[1] = fX;
	pQuaternion->m_fValues[2] = fY;
	pQuaternion->m_fValues[3] = fZ;
}

template<class Real>
void Matrix4x4<Real>::Matrix4x4ToQuaternion(const Matrix4x4<Real> &matSource,Quaternion* pQuaternion)
{
	matSource.ToQuaternion(pQuaternion);
}
*/
template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::PerspectiveLH( float fieldOfViewY,float aspectRatio,float zNear,float zFar )
{
	/*
	xScale     0          0               0
	0        yScale       0               0
	0          0       zFar/(zFar-zNear)         1
	0          0       -zNear*zFar/(zFar-zNear)     0
	where:
	yScale = cot(fieldOfViewY/2)

	xScale = yScale / aspectRatio
	*/

	return Matrix4x4();
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::PerspectiveRH( float fieldOfViewY,float aspectRatio,float zNear,float zFar )
{
	/*
	xScale     0          0              0
	0        yScale       0              0
	0        0        zFar/(zNear-zFar)        -1
	0        0        zNear*zFar/(zNear-zFar)      0

	where:
	yScale = cot(fieldOfViewY/2)

	xScale = yScale / aspectRatio
	*/
	return Matrix4x4();
}