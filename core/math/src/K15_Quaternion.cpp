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
#include "K15_MathUtil.h"

#include "glm/gtx/quaternion.hpp"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	const Quaternion Quaternion::Identity;
	const Quaternion Quaternion::Zero(0.0f,0.0f,0.0f,0.0f);
	/*********************************************************************************/
	Quaternion::Quaternion()
	{
		memset(m_QuaternionArray,0,sizeof(m_QuaternionArray));
		w = 1.f;
	}
	/*********************************************************************************/
	Quaternion::Quaternion(float w, float x, float y, float z)
		: w(w),
		x(x),
		y(y),
		z(z)
	{

	}
	/*********************************************************************************/
	Quaternion::Quaternion(const Quaternion& p_Quaternion)
	{
		memcpy(m_QuaternionArray,p_Quaternion.m_QuaternionArray,sizeof(Quaternion));
	}
	/*********************************************************************************/
	Quaternion::~Quaternion()
	{

	}
	/*********************************************************************************/
	float Quaternion::length() const
	{
		return MathUtil::sqrt(x*x + y*y + z*z + w*w);
	}
	/*********************************************************************************/
	void Quaternion::normalize()
	{
		float magnitude = length();

		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		w /= magnitude;
	}
	/*********************************************************************************/
	void Quaternion::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	/*********************************************************************************/
	void Quaternion::invert()
	{
		conjugate();
		*this / length();
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator+(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion += p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator-(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion -= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator*(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion *= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator/(const Quaternion& p_Quaternion) const
	{
		Quaternion quaternion(*this);

		quaternion /= p_Quaternion;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator*(float p_Scalar) const
	{
		Quaternion quaternion(*this);

		quaternion *= p_Scalar;

		return *this;
	}
	/*********************************************************************************/
	Quaternion Quaternion::operator/(float p_Scalar) const
	{
		if(p_Scalar == 0.f) p_Scalar = 1.f;

		Quaternion quaternion(*this);

		quaternion /= p_Scalar;

		return quaternion;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator=(const Quaternion& p_Quaternion)
	{
		memcpy(m_QuaternionArray, p_Quaternion.m_QuaternionArray, sizeof(m_QuaternionArray));
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator+=(const Quaternion& p_Quaternion)
	{
		x += p_Quaternion.x;
		y += p_Quaternion.y;
		z += p_Quaternion.z;
		w += p_Quaternion.w;

		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator-=(const Quaternion& p_Quaternion)
	{
		x *= p_Quaternion.x;
		y *= p_Quaternion.y;
		z *= p_Quaternion.z;
		w *= p_Quaternion.w;

		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator*=(const Quaternion& p_Quaternion)
	{
		glm::quat quat, quat2;

		quat.x = x; quat2.x = p_Quaternion.x;
		quat.y = y; quat2.y = p_Quaternion.y;
		quat.z = z; quat2.z = p_Quaternion.z;
		quat.w = w; quat2.w = p_Quaternion.w;

		quat *= quat2;

		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;

		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator/=(const Quaternion& p_Quaternion)
	{
		Quaternion quaternion(p_Quaternion);
		quaternion.invert();

		return this->operator*=(quaternion);
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator*=(float p_Scalar)
	{
		x *= p_Scalar;
		y *= p_Scalar;
		z *= p_Scalar;
		w *= p_Scalar;
		
		return *this;
	}
	/*********************************************************************************/
	const Quaternion& Quaternion::operator/=(float p_Scalar)
	{
		if(p_Scalar == 0.f) p_Scalar = 1.f;

		x /= p_Scalar;
		y /= p_Scalar;
		z /= p_Scalar;
		w /= p_Scalar;
		
		return *this;
	}
	/*********************************************************************************/
	bool Quaternion::operator==(const Quaternion& p_Quaternion) const
	{
		return (x == p_Quaternion.x) && (y == p_Quaternion.y) && (z == p_Quaternion.z) && (w == p_Quaternion.w);
	}
	/*********************************************************************************/
	bool Quaternion::operator!=(const Quaternion& p_Quaternion) const
	{
		return !this->operator==(p_Quaternion);
	}
	/*********************************************************************************/
	float Quaternion::getRoll() const
	{
		glm::quat quat;
		
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;

		return glm::roll(quat);
	}
	/*********************************************************************************/
	float Quaternion::getPitch() const
	{
		glm::quat quat;
		
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;

		return glm::pitch(quat);
	}
	/*********************************************************************************/
	float Quaternion::getYaw() const
	{
		glm::quat quat;
		
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;

		return glm::yaw(quat);
	}
	/*********************************************************************************/
	bool Quaternion::isIdentity() const
	{
		return w == 1.0f && (x == 0.0f && y == 0.0f && z == 0.0f);
	}
	/*********************************************************************************/
	float Quaternion::dot(const Quaternion& p_Quaternion) const
	{
		glm::quat quat, quat2;

		quat.x = x;	quat2.x = p_Quaternion.x;
		quat.y = y; quat2.y = p_Quaternion.y;
		quat.z = z; quat2.z = p_Quaternion.z;
		quat.w = w; quat2.w = p_Quaternion.w;

		return glm::dot(quat, quat2);
	}
	/*********************************************************************************/
	Matrix4 Quaternion::toRotationMatrix() const
	{
		Matrix4 rotMatrix;
		glm::mat4 mat;
		glm::quat quat;

		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;

		mat = glm::toMat4(quat);

		memcpy(&rotMatrix, &mat, sizeof(Matrix4));

		return rotMatrix;
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace
