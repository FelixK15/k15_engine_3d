/**
 * @file K15_Transformation.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#include "K15_Transformation.h"
#include "K15_Mathematic.h"
#include "windows.h"
#include "xnamath.h"

using namespace K15_Renderer;

Transformation::Transformation()
	: m_matMatrix(Matrix4x4F::MF_IDENT),
	 // m_quaRotation(),
	 m_matRotate(Matrix4x4F::MF_IDENT),
	  m_vecScale(1.f,1.f,1.f),
	  m_vecTransform(),
	  m_vecLookVec(0.f,0.f,1.f)
{
	
}

Transformation::Transformation(const Transformation &tTransform)
{
	_CopyFrom(tTransform);
}

Transformation::~Transformation()
{

}

void Transformation::Identity()
{
	m_matMatrix.Identity();
	//m_quaRotation.Identity();
	m_matRotate.Identity();

	m_vecScale.x = 1.f;
	m_vecScale.y = 1.f;
	m_vecScale.z = 1.f;

	m_vecLookVec = Vector3F::Forward;

	_UpdateMatrix();
}

// void Transformation::RotateEulerXYZ(const Vector3 &vecEuler)
// {
// 	
// }

void Transformation::Rotate(const Vector3F &vecAxis,float fAngle)
{
	XMMATRIX rotMat = XMMatrixRotationAxis(XMVectorSet(vecAxis.x,vecAxis.y,vecAxis.z,1.0),fAngle);
// 	float fHalfAngle = 0.5f / fAngle;
// 	float fSin = MatheF::Sin(fHalfAngle);
// 
// 	m_quaRotation.s = MatheF::Cos(fHalfAngle);
// 	m_quaRotation.v.x = fSin * vecAxis.x;
// 	m_quaRotation.v.y = fSin * vecAxis.y;
// 	m_quaRotation.v.z = fSin * vecAxis.z;
	memcpy(&m_matRotate,&rotMat,sizeof(Matrix4x4F));

	_UpdateMatrix();
}

void Transformation::Rotate(const Matrix4x4F &matRotate)
{
	//Matrix4x4::Matrix4x4ToQuaternion(matRotate,&m_quaRotation);

	_UpdateMatrix();
}

void Transformation::Rotate(const QuaternionF &quaRotate)
{
	//m_quaRotation = quaRotate;

	_UpdateMatrix();
}

void Transformation::UniformScale(float fScale)
{
	m_vecScale.x = fScale;
	m_vecScale.y = fScale;
	m_vecScale.z = fScale;

	_UpdateMatrix();
}

void Transformation::Scale(const Vector3F &vecScale)
{
	m_vecScale = vecScale;

	_UpdateMatrix();
}

void Transformation::Translate(const Vector3F &vecTrans)
{
	m_vecTransform = vecTrans;

	_UpdateMatrix();
}

void Transformation::LookAt(const Vector3F &vecPosition)
{
	float fYCosAngle = m_vecTransform.Dot(vecPosition);
	float fYAngle = MatheF::ACos(fYCosAngle);

	float fXCosAngle = m_vecLookVec.Dot(vecPosition);
	float fXAngle = MatheF::ACos(fXAngle);

	Rotate(Vector3F::Up,fYAngle);
	Rotate(Vector3F::Right,fXAngle);
}

Vector3F &Transformation::operator*(const Vector3F &vecVector)
{
	return m_matMatrix * vecVector;
}

const Transformation &Transformation::operator*=(const Matrix4x4F &matMatrix)
{
	Rotate(matMatrix);
	return *this;
}

const Transformation &Transformation::operator*=(const Transformation &tTransform)
{
	Rotate(tTransform.m_matMatrix);
	return *this;
}

const Transformation &Transformation::operator=(const Transformation &tTransform)
{
	_CopyFrom(tTransform);
	return *this;
}

bool Transformation::operator==(const Transformation &tTransform)
{
	return _Equal(tTransform);
}

bool Transformation::operator!=(const Transformation &tTransform)
{
	return !_Equal(tTransform);
}

bool Transformation::_Equal(const Transformation &tTransform)
{
	return m_matMatrix == tTransform.m_matMatrix;
}

void Transformation::_CopyFrom(const Transformation &tTransform)
{
	m_matMatrix = tTransform.m_matMatrix;
//	m_quaRotation = tTransform.m_quaRotation;
	m_vecLookVec = tTransform.m_vecLookVec;
	m_vecScale = tTransform.m_vecScale;
	m_vecTransform = tTransform.m_vecTransform;
}

void Transformation::_UpdateMatrix()
{
	m_matMatrix._1_1 = m_matMatrix._1_1 * m_vecScale.x;
	m_matMatrix._2_2 = m_matMatrix._2_2 * m_vecScale.y;
	m_matMatrix._3_3 = m_matMatrix._3_3 * m_vecScale.z;

	//Rotation
	//m_quaRotation.ToMatrix(&m_matMatrix);
	m_matMatrix = m_matMatrix * m_matRotate;

	//Translation
	m_matMatrix._4_1 = m_vecTransform.x;
	m_matMatrix._4_2 = m_vecTransform.y;
	m_matMatrix._4_3 = m_vecTransform.z;

	//Scaling
	
// 	m_matMatrix._1_2 = m_matMatrix._1_2 * m_vecScale.y;
// 	m_matMatrix._1_3 = m_matMatrix._1_3 * m_vecScale.z;

/*	m_matMatrix._2_1 = m_matMatrix._2_1 * m_vecScale.x;*/
	
/*	m_matMatrix._2_3 = m_matMatrix._2_3 * m_vecScale.z;*/

// 	m_matMatrix._3_1 = m_matMatrix._3_1 * m_vecScale.x;
// 	m_matMatrix._3_2 = m_matMatrix._3_2 * m_vecScale.y;
	
}
