/**
 * @file K15_Transformation.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#pragma once

#ifndef __K15_TRANSFORMATION__
#define __K15_TRANSFORMATION__

#include "K15_Matrix4x4.h"
#include "K15_Quaternion.h"

using namespace K15_Math;

namespace K15_Renderer
{
	class Transformation
	{
	public:
		Transformation();
		Transformation(const Transformation &tTransform);
		~Transformation();

		void Identity();
		void Inverse();

		bool IsIdentity() const;

		const Matrix4x4F &Matrix() const;

		//void RotateEulerXYZ(const Vector3 &vecEuler);
		void Rotate(const Vector3F &vecAxis,float fAngle);
		void Rotate(const Matrix4x4F &matRotate);
		void Rotate(const QuaternionF &quaRotate);

		void UniformScale(float fScale);
		void Scale(const Vector3F &vecScale);

		void Translate(const Vector3F &vecTrans);
		void LookAt(const Vector3F &vecPosition);

		Vector3F &operator*(const Vector3F &vecVector);

		const Transformation &operator*=(const Matrix4x4F &matMatrix);
		const Transformation &operator*=(const Transformation &tTransform);
		const Transformation &operator=(const Transformation &tTransform);
		bool operator==(const Transformation &tTransform);
		bool operator!=(const Transformation &tTransform);

	private:
		bool _Equal(const Transformation &tTransform);
		void _CopyFrom(const Transformation &tTransform);
		void _UpdateMatrix();

	private:
		Matrix4x4F m_matMatrix;
		//Quaternion m_quaRotation;
		Matrix4x4F m_matRotate;

		Vector3F m_vecTransform;
		Vector3F m_vecScale;
		Vector3F m_vecLookVec;
	};
	#include "../src/K15_Transformation.inl"
}

#endif //__K15_TRANSFORMATION__