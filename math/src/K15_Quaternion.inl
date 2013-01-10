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


template <class Real>
Quaternion<Real>::Quaternion()
{
	m_fValues[0] = 0;
	m_fValues[1] = 0;
	m_fValues[2] = 0;
	m_fValues[3] = 0;
}

template <class Real>
Quaternion<Real>::Quaternion(Real fWValue, Real fXValue, Real fYValue, Real fZValue)
{
	m_fValues[0] = fWValue;
	m_fValues[1] = fXValue;
	m_fValues[2] = fYValue;
	m_fValues[3] = fZValue;
}

template <class Real>
Quaternion<Real>::Quaternion(Real fSValue, Vector3<Real> vecVec)
{
	m_fValues[0] = fSValue;
	m_fValues[1] = vecVec.x;
	m_fValues[2] = vecVec.y;
	m_fValues[3] = vecVec.z;
}

template <class Real>
Quaternion<Real>::Quaternion(const Quaternion<Real> &qQuat)
{
	m_fValues[0] = qQuat.m_fValues[0];
	m_fValues[1] = qQuat.m_fValues[1];
	m_fValues[2] = qQuat.m_fValues[2];
	m_fValues[3] = qQuat.m_fValues[3];
}



template <class Real>
Quaternion<Real>::~Quaternion()
{

}

template <class Real>
Real Quaternion<Real>::Magnitude() const
{
	return Mathematic<Real>::Sqrt(m_fValues[0]*m_fValues[0]+m_fValues[1]*m_fValues[1]+m_fValues[2]*m_fValues[2]+m_fValues[3]*m_fValues[3]);
}

template <class Real>
void Quaternion<Real>::Normalize()
{
	Real fMag = Magnitude();
	if(fMag != 0){
		m_fValues[0] = m_fValues[0] / fMag;
		m_fValues[1] = m_fValues[1] / fMag;
		m_fValues[2] = m_fValues[2] / fMag;
		m_fValues[3] = m_fValues[3] / fMag;
	}
}


template <class Real>
void Quaternion<Real>::Conjugate()
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

template <class Real>
void Quaternion<Real>::Invert()
{
	Conjugate();
	*this / Magnitude();
}


template <class Real>
bool Quaternion<Real>::IsUnitQuaternion() const
{
	return ((Real)1 == Magnitude());
}

// ACHTUNG: Multiplikation NICHT kommutativ, Hamilton-Produkt
template <class Real>
Quaternion<Real> Quaternion<Real>::operator*(const Quaternion<Real> &qQuat)
{
	Real fTmpW, fTmpX, fTmpY, fTmpZ; 
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

template <class Real>
Vector3<Real> Quaternion<Real>::operator*(Vector3<Real> &vec)
{
	Vector3<Real> vTmp1, vTmp2;
	vTmp1 = v.Cross(vec);
	vTmp2 = v.Cross(vTmp1);
	vTmp1 *= (2 * s);
	vTmp2 *= 2;
	return vec + vTmp1 + vTmp2;
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator*=(const Quaternion<Real> &qQuat)
{
	Real fTmpW, fTmpX, fTmpY, fTmpZ; 
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

template <class Real>
Quaternion<Real> Quaternion<Real>::operator*(Real fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator*=(Real fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::operator+(const Quaternion<Real> &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::operator-(const Quaternion<Real> &qQuat)
{
	for(int i = 0; i < 4; i++){
		m_fValues[i] -= qQuat.m_fValues[i];
	}
	return *this;
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator-()
{
	for(int i =0; i < 4; i++){
		m_fValues[i] = -m_fValues[i]; 
	}
	return *this;
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator+=(const Quaternion<Real> &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator-=(const Quaternion<Real> &qQuat)
{
	m_fValues[0] -= qQuat.m_fValues[0];
	m_fValues[1] -= qQuat.m_fValues[1];
	m_fValues[2] -= qQuat.m_fValues[2];
	m_fValues[3] -= qQuat.m_fValues[3];
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::operator/(Real fScalar)
{
	assert(fScalar != 0);
	m_fValues[0] /= fScalar;
	m_fValues[1] /= fScalar;
	m_fValues[2] /= fScalar;
	m_fValues[3] /= fScalar;
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::operator/(const Quaternion<Real> &qQuat)
{
	Quaternion<Real> qTmp(qQuat);
	qTmp.Invert();
	return *this * qTmp; 
}

template <class Real>
bool Quaternion<Real>::operator==(const Quaternion<Real> &qQuat)
{
	return ((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

template <class Real>
bool Quaternion<Real>::operator!=(const Quaternion<Real> &qQuat)
{
	return !((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

template <class Real>
Quaternion<Real> &Quaternion<Real>::operator=(const Quaternion<Real> &qQuat)
{
	s = qQuat.s;
	v = qQuat.v;
	return *this;
}

template <class Real>
Real Quaternion<Real>::Dot(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2)
{
	return qQuat1.v.Dot(qQuat2.v) + qQuat1.s * qQuat2.s;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::Lerp(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2, Real fValue)
{
	Quaternion<Real> qTmp1 = qQuat1;
	Quaternion<Real> qTmp2 = qQuat2;
	Quaternion<Real> qTmp = qTmp1 * (1-fValue) + qTmp2 * fValue;
	qTmp.Normalize();
	return qTmp; 
}

template <class Real>
Quaternion<Real> Quaternion<Real>::Slerp(const Quaternion<Real> &qQuat1, const Quaternion<Real> &qQuat2, Real fValue)
{
	Quaternion<Real> qTmp;
	Quaternion<Real> qTmp2 = qQuat2;
	Quaternion<Real> qTmp1 = qQuat1;
	Real fDot = Quaternion<Real>::Dot(qQuat1, qQuat2);
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
		Real fAngle = acosf(fDot);
		return (qTmp1*sinf(fAngle*(1-fValue)) + qTmp*sinf(fAngle*fValue))/sinf(fAngle);
	} else return Lerp(qTmp1,qTmp,fValue);		
}

// mit Vorsicht geniessen, normalize need? code sollte richtig sein, testen!
template <class Real>
void Quaternion<Real>::QuaternionToMatrix(const Quaternion<Real> &qQuat, Matrix4x4<Real>* matDest)
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

template <class Real>
void Quaternion<Real>::ToMatrix(Matrix4x4<Real>* matDest)
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

template <class Real>
Quaternion<Real> Quaternion<Real>::AxisAngleToQuaternion(Real rAngel, Vector3<Real> vecVec)
{
	if(!vecVec.IsUnitVector()) vecVec.Normalize();
	m_fValues[0]   =   Mathematic::Cos(rAngel/(Real)2);
	m_fValues[1]   =   vecVec.x * Mathematic<Real>::Sin(rAngel/(Real)2);
	m_fValues[2]   =   vecVec.y * Mathematic<Real>::Sin(rAngel/(Real)2);
	m_fValues[3]   =   vecVec.z * Mathematic<Real>::Sin(rAngel/(Real)2);
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::AxisAngleToQuaternion(Real rWValue, Real rXValue, Real rYValue, Real rZValue)
{
	Vector3<Real> vecVec(rXValue,rYValue,rZValue);
	if(!vecVec.IsUnitVector()) vecVec.Normalize();
	m_fValues[0]   =   Mathematic<Real>::Cos(rWValue/(Real)2);
	m_fValues[1]   =   vecVec.x * Mathematic<Real>::Sin(rWValue/(Real)2);
	m_fValues[2]   =   vecVec.y * Mathematic<Real>::Sin(rWValue/(Real)2);
	m_fValues[3]   =   vecVec.z * Mathematic<Real>::Sin(rWValue/(Real)2);
	return *this;
}

template <class Real>
Quaternion<Real> Quaternion<Real>::EulerAnglesToQuaternion(Real rXEuler, Real rYEuler, Real rZEuler)
{
	Quaternion<Real> Qx (Mathematic<Real>::Cos(rXEuler/2), Mathematic<Real>::Sin(rXEuler/2), 0, 0);
	Quaternion<Real> Qy (Mathematic<Real>::Cos(rYEuler/2), 0, Mathematic<Real>::Sin(rYEuler/2), 0);
	Quaternion<Real> Qz (Mathematic<Real>::Cos(rZEuler/2), 0, 0, Mathematic<Real>::Sin(rZEuler/2));
	return (Qx * Qy * Qz);
}

template <class Real>
Quaternion<Real> Quaternion<Real>::EulerAnglesToQuaternion(Vector3<Real> vecVec)
{
	Quaternion<Real> Qx (Mathematic<Real>::Cos(vecVec.x/2), Mathematic<Real>::Sin(vecVec.x/2), 0, 0);
	Quaternion<Real> Qy (Mathematic<Real>::Cos(vecVec.y/2), 0, Mathematic<Real>::Sin(vecVec.y/2), 0);
	Quaternion<Real> Qz (Mathematic<Real>::Cos(vecVec.z/2), 0, 0, Mathematic<Real>::Sin(vecVec.z/2));
	return (Qx * Qy * Qz);
}