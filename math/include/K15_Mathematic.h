/**
 * @file K15_Mathematic.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/12
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
 *							<--- PLEASE PUT A DESCRIPTION HERE
 * @todo replace math.h functions with K15_Mathematic functions.
 */

#pragma once

#ifndef __K15_Mathematic__
#define __K15_Mathematic__

namespace K15_Math
{
	template<class Real>
	class Mathematic
	{
	public:
		static Real RadToDeg(Real fRadiant);
		static Real DegToRad(Real fDegree);

		static Real Abs(Real fValue);
		static Real FAbs(Real fValue);
		static Real Sin(Real fAngle);
		static Real Cos(Real fAngle);
		static Real Tan(Real fAngle);
		static Real ASin(Real fSin);
		static Real ACos(Real fCos);
		static Real ATan(Real fTan);
		static Real ATan2(Real fTan);
		static Real Sqrt(Real fValue);
		static Real Pow(Real fBase,int fExponent);
		static Real InvSqrt(Real fValue);
		static Real Log(Real fValue);
		static Real Log2(Real fValue);
		static Real Log10(Real fValue);

		static Real FastSin1(Real fAngle);
		static Real FastSin2(Real fAngle);

		static Real FastCos1(Real fAngle);
		static Real FastCos2(Real fAngle);

		static Real FastTan1(Real fAngle);
		static Real FastTan2(Real fAngle);

		static Real FastArcSin1(Real fValue);
		static Real FastArcSin2(Real fValue);

		static Real FastArcCos1(Real fValue);
		static Real FastArcCos2(Real fValue);

		static Real FastArcTan1(Real fValue);
		static Real FastArcTan2(Real fValue);

		static Real FastInvSqrt(Real fValue);

	public:
		static const Real PI;
		static const Real HALF_PI;
		static const Real TWO_PI;
		static const Real LN_2;
		static const Real LN_10;
		static const Real INV_LN_2;
		static const Real INV_LN_10;
		static const Real DEGREE_TO_RAD;
		static const Real RAD_TO_DEGREE;
	};

	typedef Mathematic<float> MatheF;
	typedef Mathematic<double> MatheD;

	#include "../src/K15_Mathematic.inl"
}

#endif