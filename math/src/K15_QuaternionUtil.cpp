/**
 * @file K15_QuaternionUtil.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
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

#include "K15_QuaternionUtil.h"

#include "K15_MathUtil.h"
#include "glm/gtx/quaternion.hpp"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	Quaternion QuaternionUtil::fromAxisAngles(const Vector3& p_Axis, float p_Angle)
	{
		glm::quat quat;
		glm::vec3 axis;
		Quaternion quaternion;

		memcpy(&axis, &p_Axis, sizeof(Vector3));

		quat = glm::angleAxis(p_Angle, axis);

		quaternion.x = quat.x;
		quaternion.y = quat.y;
		quaternion.z = quat.z;
		quaternion.w = quat.w;

		return quaternion;
	}
	/*********************************************************************************/
	Quaternion QuaternionUtil::fromPitchYawRoll(float p_Pitch, float p_Yaw, float p_Roll)
	{
		Quaternion xAxis = fromAxisAngles(Vector3::Right, p_Pitch);
		Quaternion yAxis = fromAxisAngles(Vector3::Up, p_Yaw);
		Quaternion zAxis = fromAxisAngles(Vector3::Forward, p_Roll);

		return xAxis * yAxis * zAxis;
	}
	/*********************************************************************************/
	Quaternion QuaternionUtil::fromTransformation( const Matrix4& p_Matrix )
	{
		//Quaternion quat;

		/*quat.w = MathUtil::sqrt(std::max(0.f, 1.f + p_Matrix._1_1 + p_Matrix._2_2 + p_Matrix._3_3)) * 0.5f;
		quat.x = MathUtil::sqrt(std::max(0.f, 1.f + p_Matrix._1_1 - p_Matrix._2_2 - p_Matrix._3_3)) * 0.5f;
		quat.y = MathUtil::sqrt(std::max(0.f, 1.f - p_Matrix._1_1 + p_Matrix._2_2 - p_Matrix._3_3)) * 0.5f;
		quat.z = MathUtil::sqrt(std::max(0.f, 1.f - p_Matrix._1_1 - p_Matrix._2_2 + p_Matrix._3_3)) * 0.5f;

		quat.x *= MathUtil::sign(quat.x * (p_Matrix._3_2 - p_Matrix._2_3));
		quat.y *= MathUtil::sign(quat.y * (p_Matrix._1_3 - p_Matrix._3_1));
		quat.z *= MathUtil::sign(quat.z * (p_Matrix._2_1 - p_Matrix._1_2));*/

		float trace = 1.f + p_Matrix._1_1 + p_Matrix._2_2 + p_Matrix._3_3;
		float S = 0.f, X = 0.f, Y = 0.f, Z = 0.f, W = 0.f;

		if(trace > K15_EPSILON)
		{
			S = MathUtil::sqrt(trace) * 2.f;
			X = (p_Matrix._1_2 - p_Matrix._2_1) / S;
			Y = (p_Matrix._3_1 - p_Matrix._1_3) / S;
			Z = (p_Matrix._2_3 - p_Matrix._3_2) / S;
			W = 0.25f * S;
		}
		else
		{
			if(p_Matrix._1_1 > p_Matrix._2_2 && p_Matrix._1_1 > p_Matrix._3_3)
			{
				S = MathUtil::sqrt(1.f + p_Matrix._1_1 - p_Matrix._2_2 - p_Matrix._3_3) * 2.f;
				X = 0.25f * S;
				Y = (p_Matrix._1_2 + p_Matrix._2_1) / S;
				Z = (p_Matrix._3_1 + p_Matrix._1_3) / S;
				W = (p_Matrix._2_3 + p_Matrix._3_2) / S;
			}
			else if(p_Matrix._2_2 > p_Matrix._3_3)
			{
				S = MathUtil::sqrt(1.f + p_Matrix._2_2 - p_Matrix._1_1 - p_Matrix._3_3) * 2.f;
				X = (p_Matrix._1_2 + p_Matrix._2_1) / S;
				Y = 0.25f * S;
				Z = (p_Matrix._2_3 + p_Matrix._3_2) / S;
				W = (p_Matrix._3_1 - p_Matrix._1_3) / S;
			}
			else
			{
				S = MathUtil::sqrt(1.f + p_Matrix._3_3 - p_Matrix._1_1 - p_Matrix._2_2) * 2.f;
				X = (p_Matrix._3_1 + p_Matrix._1_3) / S;
				Y = (p_Matrix._2_3 + p_Matrix._3_2) / S;
				Z = 0.25f * S;
				W = (p_Matrix._1_2 + p_Matrix._2_1) / S;
			}
		}

		//return quat;
		return Quaternion(X, Y, Z, W);
	}
	/*********************************************************************************/
}}//end of K15_Engine::Math namespace
