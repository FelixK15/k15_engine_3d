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
#include "math.h"

using namespace K15_Math;

Quaternion::Quaternion()
{
	Identity();
}

Quaternion::Quaternion(float fWValue, float fXValue, float fYValue, float fZValue)
{
	m_fValues[0] = fWValue;
	m_fValues[1] = fXValue;
	m_fValues[2] = fYValue;
	m_fValues[3] = fZValue;
}

Quaternion::Quaternion(float fSValue, Vector3 vecVec)
{
	m_fValues[0] = fSValue;
	m_fValues[1] = vecVec.x;
	m_fValues[2] = vecVec.y;
	m_fValues[3] = vecVec.z;
}

Quaternion::Quaternion(const Quaternion &qQuat)
{
	m_fValues[0] = qQuat.m_fValues[0];
	m_fValues[1] = qQuat.m_fValues[1];
	m_fValues[2] = qQuat.m_fValues[2];
	m_fValues[3] = qQuat.m_fValues[3];
}

Quaternion::~Quaternion()
{

}

float Quaternion::Magnitude() const
{
	return sqrt(m_fValues[0]*m_fValues[0]+m_fValues[1]*m_fValues[1]+m_fValues[2]*m_fValues[2]+m_fValues[3]*m_fValues[3]);
}

void Quaternion::Normalize()
{
	float fMag = Magnitude();
	if(fMag != 0){
		m_fValues[0] = m_fValues[0] / fMag;
		m_fValues[1] = m_fValues[1] / fMag;
		m_fValues[2] = m_fValues[2] / fMag;
		m_fValues[3] = m_fValues[3] / fMag;
	}
}

void Quaternion::Conjugate()
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

void Quaternion::Identity()
{
	s = 1.f;
	v.x = 0.f;
	v.y = 0.f;
	v.z = 0.f;
}

void Quaternion::Invert()
{
	Conjugate();
	*this / Magnitude();
}

bool Quaternion::IsUnitQuaternion() const
{
	return (1.f == Magnitude());
}

// ACHTUNG: Multiplikation NICHT kommutativ, Hamilton-Produkt
Quaternion Quaternion::operator*(const Quaternion &qQuat)
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

Vector3 Quaternion::operator*(Vector3 &vec)
{
	Vector3 vTmp1, vTmp2;
	vTmp1 = v.Cross(vec);
	vTmp2 = v.Cross(vTmp1);
	vTmp1 *= (2.0f * s);
	vTmp2 *= 2.0f;
	return vec + vTmp1 + vTmp2;
}

Quaternion &Quaternion::operator*=(const Quaternion &qQuat)
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

Quaternion Quaternion::operator*(float fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

Quaternion &Quaternion::operator*=(float fScalar)
{
	m_fValues[0] *= fScalar;
	m_fValues[1] *= fScalar;
	m_fValues[2] *= fScalar;
	m_fValues[3] *= fScalar;
	return *this;
}

Quaternion Quaternion::operator+(const Quaternion &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

Quaternion Quaternion::operator-(const Quaternion &qQuat)
{
	for(int i = 0; i < 4; i++){
		m_fValues[i] -= qQuat.m_fValues[i];
	}
	return *this;
}

Quaternion &Quaternion::operator-()
{
	for(int i =0; i < 4; i++){
		m_fValues[i] = -m_fValues[i]; 
	}
	return *this;
}

Quaternion &Quaternion::operator+=(const Quaternion &qQuat)
{
	m_fValues[0] += qQuat.m_fValues[0];
	m_fValues[1] += qQuat.m_fValues[1];
	m_fValues[2] += qQuat.m_fValues[2];
	m_fValues[3] += qQuat.m_fValues[3];
	return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &qQuat)
{
	m_fValues[0] -= qQuat.m_fValues[0];
	m_fValues[1] -= qQuat.m_fValues[1];
	m_fValues[2] -= qQuat.m_fValues[2];
	m_fValues[3] -= qQuat.m_fValues[3];
	return *this;
}

Quaternion Quaternion::operator/(float fScalar)
{
	m_fValues[0] /= fScalar;
	m_fValues[1] /= fScalar;
	m_fValues[2] /= fScalar;
	m_fValues[3] /= fScalar;
	return *this;
}

Quaternion Quaternion::operator/(const Quaternion &qQuat)
{
	Quaternion qTmp(qQuat);
	qTmp.Invert();
	return *this * qTmp; 
}

bool Quaternion::operator==(const Quaternion &qQuat)
{
	return ((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

bool Quaternion::operator!=(const Quaternion &qQuat)
{
	return !((m_fValues[0] == qQuat.m_fValues[0]) && (m_fValues[1] == qQuat.m_fValues[1]) && (m_fValues[2] == qQuat.m_fValues[2]) && (m_fValues[3] == qQuat.m_fValues[3]));
}

Quaternion &Quaternion::operator=(const Quaternion &qQuat)
{
	s = qQuat.s;
	v = qQuat.v;
	return *this;
}

float Quaternion::Dot(const Quaternion &qQuat1, const Quaternion &qQuat2)
{
	return qQuat1.v.Dot(qQuat2.v) + qQuat1.s * qQuat2.s;
}

Quaternion Quaternion::Lerp(const Quaternion &qQuat1, const Quaternion &qQuat2, float fValue)
{
	Quaternion qTmp1 = qQuat1;
	Quaternion qTmp2 = qQuat2;
	Quaternion qTmp = qTmp1 * (1-fValue) + qTmp2 * fValue;
	qTmp.Normalize();
	return qTmp; 
}

Quaternion Quaternion::Slerp(const Quaternion &qQuat1, const Quaternion &qQuat2, float fValue)
{
	Quaternion qTmp;
	Quaternion qTmp2 = qQuat2;
	Quaternion qTmp1 = qQuat1;
	float fDot = Quaternion::Dot(qQuat1, qQuat2);
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
void Quaternion::QuaternionToMatrix(const Quaternion &qQuat, Matrix4x4* matDest)
{
	//Normalize();
	matDest->_1_1 = 1 - 2*(qQuat.m_fValues[2]*qQuat.m_fValues[2] + qQuat.m_fValues[3]*qQuat.m_fValues[3]);
	matDest->_1_2 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[2] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[3];
	matDest->_1_3 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[3] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[2];
	matDest->_1_4 = 0;
	matDest->_2_1 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[2] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[3];
	matDest->_2_2 = 1 - 2*(qQuat.m_fValues[3]*qQuat.m_fValues[3] + qQuat.m_fValues[1]*qQuat.m_fValues[1]);
	matDest->_2_3 = 2*qQuat.m_fValues[2]*qQuat.m_fValues[3] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[1];
	matDest->_2_4 = 0;
	matDest->_3_1 = 2*qQuat.m_fValues[1]*qQuat.m_fValues[3] - 2*qQuat.m_fValues[0]*qQuat.m_fValues[2];
	matDest->_3_2 = 2*qQuat.m_fValues[2]*qQuat.m_fValues[3] + 2*qQuat.m_fValues[0]*qQuat.m_fValues[1];
	matDest->_3_3 = 1- 2*(qQuat.m_fValues[2]*qQuat.m_fValues[2] + qQuat.m_fValues[1]*qQuat.m_fValues[1]);
	matDest->_3_4 = 0;
	matDest->_4_1 = 0;
	matDest->_4_2 = 0;
	matDest->_4_3 = 0;
	matDest->_4_4 = 1;
}

void Quaternion::ToMatrix(Matrix4x4* matDest)
{
	//Normalize();
	matDest->_1_1 = 1 - 2*(m_fValues[2]*m_fValues[2] + m_fValues[3]*m_fValues[3]);
	matDest->_1_2 = 2*m_fValues[1]*m_fValues[2] - 2*m_fValues[0]*m_fValues[3];
	matDest->_1_3 = 2*m_fValues[1]*m_fValues[3] + 2*m_fValues[0]*m_fValues[2];
	matDest->_1_4 = 0;
	matDest->_2_1 = 2*m_fValues[1]*m_fValues[2] + 2*m_fValues[0]*m_fValues[3];
	matDest->_2_2 = 1 - 2*(m_fValues[3]*m_fValues[3] + m_fValues[1]*m_fValues[1]);
	matDest->_2_3 = 2*m_fValues[2]*m_fValues[3] - 2*m_fValues[0]*m_fValues[1];
	matDest->_2_4 = 0;
	matDest->_3_1 = 2*m_fValues[1]*m_fValues[3] - 2*m_fValues[0]*m_fValues[2];
	matDest->_3_2 = 2*m_fValues[2]*m_fValues[3] + 2*m_fValues[0]*m_fValues[1];
	matDest->_3_3 = 1 - 2*(m_fValues[2]*m_fValues[2] + m_fValues[1]*m_fValues[1]);
	matDest->_3_4 = 0;
	matDest->_4_1 = 0;
	matDest->_4_2 = 0;
	matDest->_4_3 = 0;
	matDest->_4_4 = 1;
}