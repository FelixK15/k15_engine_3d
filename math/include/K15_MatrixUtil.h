/**
 * @file K15_MatrixUtil.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#ifndef _K15Engine_Math_MatrixUtil_h_
#define _K15Engine_Math_MatrixUtil_h_

#include "K15_MathPrerequisites.h"
#include "K15_Matrix4.h"

namespace K15_Engine { namespace Math {
	class MatrixUtil
	{
	public:
		static Matrix4 createPerspectiveMatrix(float p_FOV, float p_AspectRatio, float p_NearPlane, float p_FarPlane);
        static Matrix4 createOrthographicMatrix(float p_Left, float p_Right, float p_Top, float p_Bottom, float p_NearPlane, float p_FarPlane);
        static Matrix4 translate(const Vector3& p_Translate);
        static Matrix4 scale(const Vector3& p_Scale);
        static Matrix4 rotate(const Vector3& p_Axis, float p_AngleInRadian);
        static Matrix4 lookAt(const Vector3& p_EyePos, const Vector3& p_LookAt, const Vector3& p_Up);
	};// end of MatrixUtil class declaration
}}//end of K15_Engine::Math namespace

#endif //_K15Engine_Math_MatrixUtil_h_
