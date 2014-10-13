/**
 * @file K15_MathUtil.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifndef _K15Engine_Math_MathUtil_h_
#define _K15Engine_Math_MathUtil_h_

#include "K15_MathPrerequisites.h"

namespace K15_Engine { namespace Math {
	class MathUtil
	{
	public:
		/*********************************************************************************/
		static const Real Pi;
		static const Real HalfPi;
		static const Real QuarterPi;
		static const Real TwoPi;
		static const Real DegreeToRadian;
		static const Real RadianToDegree;
		/*********************************************************************************/

		/*********************************************************************************/
		inline static Real sin(Real p_Angle);
		inline static Real cos(Real p_Angle);
		inline static Real tan(Real p_Angle);
		inline static Real asin(Real p_Sin);
		inline static Real acos(Real p_Cos);
		inline static Real atan(Real p_Tan);
		inline static Real atan2(Real p_X, Real p_Y);
		inline static Real sqrt(Real p_Value);
		inline static Real pow(Real p_Base, Real p_Exponent);
		inline static Real exp(Real p_Value);
		inline static Real abs(Real p_Value);
		inline static Real fabs(Real p_Value);
		inline static Real log(Real p_Value);
		inline static Real log10(Real p_Value);
		inline static Real sign(Real p_Valie);
		/*********************************************************************************/
	};// end of MathUtil class declaration
#	include "K15_MathUtil.inl"
}}// end of K15_Engine::Math namespace

#endif //_K15Engine_Math_MathUtil_h_