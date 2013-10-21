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


template <class float>
Quaternion<float>::Quaternion()
{
	m_fValues[0] = 0;
	m_fValues[1] = 0;
	m_fValues[2] = 0;
	m_fValues[3] = 0;
}

template <class float>
Quaternion<float>::Quaternion(float fWValue, float fXValue, float fYValue, float fZValue)
{
	m_fValues[0] = fWValue;
	m_fValues[1] = fXValue;
	m_fValues[2] = fYValue;
	m_fValues[3] = fZValue;
}

template <class float>
Quaternion<float>::Quaternion(float fSValue, Vector3 vecVec)
{
	m_fValues[0] = fSValue;
	m_fValues[1] = vecVec.x;
	m_fValues[2] = vecVec.y;
	m_fValues[3] = vecVec.z;
}

template <class float>
Quaternion<float>::Quaternion(const Quaternion<float> &qQuat)
{
	m_fValues[0] = qQuat.m_fValues[0];
	m_fValues[1] = qQuat.m_fValues[1];
	m_fValues[2] = qQuat.m_fValues[2];
	m_fValues[3] = qQuat.m_fValues[3];
}



template <class float>
Quaternion<float>::~Quaternion()
{

}

template <class float>
float Quaternion<float>::Magnitude() const
{
	return Mathematic<float>::Sqrt(m_fValues[0]*m_fValues[0]+m_fValues[1]*m_fValues[1]+m_fValues[2]*m_fValues[2]+m_fValues[3]*m_fValues[3]);
}

template <class float>
void Quaternion<float>::Normalize()
{
	float fMag = Magnitude();
	if(fMag != 0){
		m_fValues[0] = m_fValues[0] / fMag;
		m_fValues[1] = m_fValues[1] / fMag;
		m_fValues[2] = m_fValues[2] / fMag;
		m_fValues[3] = m_fValues[3] / fMag;
	}
}


template <class float>
void Quaternion<float>::Conjugate()
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

template <class float>
void Quaternion<float>::Invert()
{
	Conjugate();
	*this / Magnitude();
}


template <class float>
bool Quaternion<float>::IsUnitQuaternion() const
{
	return ((float)1 == Magnitude());
}

// ACHTUNG: Multiplikation NICHT kommutativ, Hamilton-Produkt
template <class float>
Quaternion<float> Quaternion<float>::operator*(const Quaternion<float> &qQuat)
{
	float fTmpW, fTmpX, fTmpY, fTmpZ; 
	fTmpW = m_fValues[0]*qQuat.m_fValues[0] - m_fValues[1]*qQuat.m_fValues[1] - m_fValues[2]*qQuat.m_fValues[2] - m_fValues[3]*qQuat.m_fValues[3];
	fTmpX = m_fValues[0]*qQuat.m_fValues[1] + m_fValues[1]*qQuat.m_fValues[0] + m_fValues[2]*qQuat.m_fValues[3] - m_fValues[3]*qQuat.m_fValues[2];
	fTmpY = m_fValues[0]*qQuat.m_fValues[2] - m_fValues[1]*qQuat.m_fValues[3] + m_fValues[2]*qQuat.m_fValues[0] + m_fValues[3]*qQuat.m_fValues[1];
	fTmpZ = m_fValues[0]*qQuat.m_fValues[3] + m_fValues[1]*qQuat.m_fValues[2] - m_fValues[2]*qQuat.m_fValues[1] + m_fValues[3]*qQuat.m_fValues[0];
	m_fValues[0] = fTmpW;
	m_fValues[1] = fTmpX;
	m_fValues[2] = fTmpY;
	m_fValues[3] = fTmpZ;
	return *this;
}

template <class float>
Vector3 Quaternion<float>::operator*(Vector3 &vec)
{
	Vector3 vTmp1, vTmp2;
	vTmp1 = v.Cross(vec);
	vTmp2 = v.Cross(vTmp1);
	vTmp1 *= (2 * s);
	vTmp2 *= 2;
	return vec + vTmp1 + vTmp2;
}

template <class float>
Quaternion<float> &Quaternion<float>::operator*=(const Quaternion<float> &qQuat)
{
	float fTmpW, fTmpX, fTmpY, fTmpZ; 
	fTmpW = m_fValues[0]*qQuat.m_fValues[0] - m_fValues[1]*qQuat.m_fValues[1] - m_fValues[2]*qQuat.m_fValues[2] - m_fValues[3]*qQuat.m_fValues[3];
	fTmpX = m_fValues[0]*qQuat.m_fValues[1] + m_fValues[1]*qQuat.m_fValues[0] + m_fValues[2]*qQuat.m_fValues[3] - m_fValues[3]*qQuat.m_fValues[2];
	fTmpY = m_fValues[0]*qQuat.m_fValues[2] - m_fValues[1]*qQuat.m_fValues[3] + m_fValues[2]*qQuat.m_fValues[0] + m_fValues[3]*qQuat.m_fValues[1];
	fTmpZ = m_fValues[0]*qQuat.m_fValues[3] + m_fValues[1]*qQuat.m_fValues[2] - m_fValues[2]*qQuat.m_fValues[1] + m_fValues[3]*qQuat.m_fValues[0];
	m_fValues[0] = fTmpW;
	m_fValues[1] = fTmpX;
	m_fValues[2] = fTmpY;
	m_fValues[3] = fTmpZ;
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::operator*(float fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

template <class float>
Quaternion<float> &Quaternion<float>::operator*=(float fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::operator+(const Quaternion<float> &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::operator-(const Quaternion<float> &qQuat)
{
	for(int i = 0; i < 4; i++){
		m_fValues[i] -= qQuat.m_fValues[i];
	}
	return *this;
}

template <class float>
Quaternion<float> &Quaternion<float>::operator-()
{
	for(int i =0; i < 4; i++){
		m_fValues[i] = -m_fValues[i]; 
	}
	return *this;
}

template <class float>
Quaternion<float> &Quaternion<float>::operator+=(const Quaternion<float> &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

template <class float>
Quaternion<float> &Quaternion<float>::operator-=(const Quaternion<float> &qQuat)
{
	m_fValues[0] -= qQuat.m_fValues[0];
	m_fValues[1] -= qQuat.m_fValues[1];
	m_fValues[2] -= qQuat.m_fValues[2];
	m_fValues[3] -= qQuat.m_fValues[3];
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::operator/(float fScalar)
{
	assert(fScalar != 0);
	m_fValues[0] /= fScalar;
	m_fValues[1] /= fScalar;
	m_fValues[2] /= fScalar;
	m_fValues[3] /= fScalar;
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::operator/(const Quaternion<float> &qQuat)
{
	Quaternion<float> qTmp(qQuat);
	qTmp.Invert();
	return *this * qTmp; 
}

template <class float>
bool Quaternion<float>::operator==(const Quaternion<float> &qQuat)
{
	return ((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

template <class float>
bool Quaternion<float>::operator!=(const Quaternion<float> &qQuat)
{
	return !((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

template <class float>
Quaternion<float> &Quaternion<float>::operator=(const Quaternion<float> &qQuat)
{
	s = qQuat.s;
	v = qQuat.v;
	return *this;
}

template <class float>
float Quaternion<float>::Dot(const Quaternion<float> &qQuat1, const Quaternion<float> &qQuat2)
{
	return qQuat1.v.Dot(qQuat2.v) + qQuat1.s * qQuat2.s;
}

template <class float>
Quaternion<float> Quaternion<float>::Lerp(const Quaternion<float> &qQuat1, const Quaternion<float> &qQuat2, float fValue)
{
	Quaternion<float> qTmp1 = qQuat1;
	Quaternion<float> qTmp2 = qQuat2;
	Quaternion<float> qTmp = qTmp1 * (1-fValue) + qTmp2 * fValue;
	qTmp.Normalize();
	return qTmp; 
}

template <class float>
Quaternion<float> Quaternion<float>::Slerp(const Quaternion<float> &qQuat1, const Quaternion<float> &qQuat2, float fValue)
{
	Quaternion<float> qTmp;
	Quaternion<float> qTmp2 = qQuat2;
	Quaternion<float> qTmp1 = qQuat1;
	float fDot = Quaternion<float>::Dot(qQuat1, qQuat2);
	if (fDot < 0){
		fDot = -fDot;
		qTmp2.Conjugate();
		qTmp.s = -qTmp2.s;
		qTmp.v = qTmp2.v;
	} else{
		qTmp = qQuat2;
	}
	if (fDot < 0.95f)
	{
		float fAngle = acosf(fDot);
		return (qTmp1*sinf(fAngle*(1-fValue)) + qTmp*sinf(fAngle*fValue))/sinf(fAngle);
	} else return Lerp(qTmp1,qTmp,fValue);		
}

// mit Vorsicht geniessen, normalize need? code sollte richtig sein, testen!
template <class float>
void Quaternion<float>::QuaternionToMatrix(const Quaternion<float> &qQuat, Matrix4x4<float>* matDest)
{
	//Normalize();
	matDest->_1_1 = 1 - 2*(qQuat.m_fValues[2]*qQuat.m_fValues[2] + qQuat.m_fValues[3]*qQuat.m_fValues[3]);
	matDest->_2_1 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[2] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[3];
	matDest->_3_1 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[3] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[2];
	matDest->_4_1 = 0;
	matDest->_1_2 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[2] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[3];
	matDest->_2_2 = 1 - 2*(qQuat.m_fValues[3]*qQuat.m_fValues[3] + qQuat.m_fValues[1]*qQuat.m_fValues[1]);
	matDest->_3_2 = 2*qQuat.m_fValues[2]*qQuat.m_fValues[3] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[1];
	matDest->_4_2 = 0;
	matDest->_1_3 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[3] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[2];
	matDest->_2_3 = 2*qQuat.m_fValues[2]*qQuat.m_fValues[3] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[1];
	matDest->_3_3 = 1- 2*(qQuat.m_fValues[2]*qQuat.m_fValues[2] + qQuat.m_fValues[1]*qQuat.m_fValues[1]);
	matDest->_4_3 = 0;
	matDest->_1_4 = 0;
	matDest->_2_4 = 0;
	matDest->_3_4 = 0;
	matDest->_4_4 = 1;
}

template <class float>
void Quaternion<float>::ToMatrix(Matrix4x4<float>* matDest)
{
	//Normalize();
	matDest->_1_1 = 1 - 2*(m_fValues[2]*m_fValues[2] + m_fValues[3]*m_fValues[3]);
	matDest->_2_1 = 2*m_fValues[1]*m_fValues[2] - 2*m_fValues[0]*m_fValues[3];
	matDest->_3_1 = 2*m_fValues[1]*m_fValues[3] + 2*m_fValues[0]*m_fValues[2];
	matDest->_4_1 = 0;
	matDest->_1_2 = 2*m_fValues[1]*m_fValues[2] + 2*m_fValues[0]*m_fValues[3];
	matDest->_2_2 = 1 - 2*(m_fValues[3]*m_fValues[3] + m_fValues[1]*m_fValues[1]);
	matDest->_3_2 = 2*m_fValues[2]*m_fValues[3] - 2*m_fValues[0]*m_fValues[1];
	matDest->_4_2 = 0;
	matDest->_1_3 = 2*m_fValues[1]*m_fValues[3] - 2*m_fValues[0]*m_fValues[2];
	matDest->_2_3 = 2*m_fValues[2]*m_fValues[3] + 2*m_fValues[0]*m_fValues[1];
	matDest->_3_3 = 1 - 2*(m_fValues[2]*m_fValues[2] + m_fValues[1]*m_fValues[1]);
	matDest->_4_3 = 0;
	matDest->_1_4 = 0;
	matDest->_2_4 = 0;
	matDest->_3_4 = 0;
	matDest->_4_4 = 1;
}

template <class float>
Quaternion<float> Quaternion<float>::AxisAngleToQuaternion(float rAngel, Vector3 vecVec)
{
	if(!vecVec.IsUnitVector()) vecVec.Normalize();
	m_fValues[0]   =   Mathematic::Cos(rAngel/(float)2);
	m_fValues[1]   =   vecVec.x * Mathematic<float>::Sin(rAngel/(float)2);
	m_fValues[2]   =   vecVec.y * Mathematic<float>::Sin(rAngel/(float)2);
	m_fValues[3]   =   vecVec.z * Mathematic<float>::Sin(rAngel/(float)2);
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::AxisAngleToQuaternion(float rWValue, float rXValue, float rYValue, float rZValue)
{
	Vector3 vecVec(rXValue,rYValue,rZValue);
	if(!vecVec.IsUnitVector()) vecVec.Normalize();
	m_fValues[0]   =   Mathematic<float>::Cos(rWValue/(float)2);
	m_fValues[1]   =   vecVec.x * Mathematic<float>::Sin(rWValue/(float)2);
	m_fValues[2]   =   vecVec.y * Mathematic<float>::Sin(rWValue/(float)2);
	m_fValues[3]   =   vecVec.z * Mathematic<float>::Sin(rWValue/(float)2);
	return *this;
}

template <class float>
Quaternion<float> Quaternion<float>::EulerAnglesToQuaternion(float rXEuler, float rYEuler, float rZEuler)
{
	Quaternion<float> Qx (Mathematic<float>::Cos(rXEuler/2), Mathematic<float>::Sin(rXEuler/2), 0, 0);
	Quaternion<float> Qy (Mathematic<float>::Cos(rYEuler/2), 0, Mathematic<float>::Sin(rYEuler/2), 0);
	Quaternion<float> Qz (Mathematic<float>::Cos(rZEuler/2), 0, 0, Mathematic<float>::Sin(rZEuler/2));
	return (Qx * Qy * Qz);
}

template <class float>
Quaternion<float> Quaternion<float>::EulerAnglesToQuaternion(Vector3 vecVec)
{
	Quaternion<float> Qx (Mathematic<float>::Cos(vecVec.x/2), Mathematic<float>::Sin(vecVec.x/2), 0, 0);
	Quaternion<float> Qy (Mathematic<float>::Cos(vecVec.y/2), 0, Mathematic<float>::Sin(vecVec.y/2), 0);
	Quaternion<float> Qz (Mathematic<float>::Cos(vecVec.z/2), 0, 0, Mathematic<float>::Sin(vecVec.z/2));
	return (Qx * Qy * Qz);
}