/**
 * @file K15_QuaternionUtil.h
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
 *
 * @section DESCRIPTION
 *
 *
 */

#ifndef _K15Engine_Math_QuaternionUtil_h_
#define _K15Engine_Math_QuaternionUtil_h_

#include "K15_MathPrerequisites.h"
#include "K15_Quaternion.h"

namespace K15_Engine { namespace Math {
	class QuaternionUtil
	{
	public:
		static Quaternion fromAxisAngles(const Vector3& p_Axis, float p_Angle);
		static Quaternion fromPitchYawRoll(float p_Pitch, float p_Yaw, float p_Roll);
		static Quaternion fromTransformation(const Matrix4& p_TransformationMatrix);
	}; // end of QuaternionUtil class declaration
}} // end of K15_Engine::Math namespace

#endif //_K15Engine_Math_QuaternionUtil_h_