/**
 * @file K15_Mathematic.cpp
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/03
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

#include "K15_Mathematic.h" 
#include "math.h"

using namespace K15_Math;

template<> const float Mathematic<float>::PI = 4.f*atan(1.f);
template<> const float Mathematic<float>::HALF_PI = Mathematic<float>::PI * 0.5f;
template<> const float Mathematic<float>::TWO_PI = Mathematic<float>::PI * 2.f;
template<> const float Mathematic<float>::DEGREE_TO_RAD = Mathematic<float>::PI / 180.f;
template<> const float Mathematic<float>::RAD_TO_DEGREE = 180.f / Mathematic<float>::PI;
template<> const float Mathematic<float>::LN_2 = Mathematic<float>::Log(2.f);
template<> const float Mathematic<float>::LN_10 = Mathematic<float>::Log(10.f);
template<> const float Mathematic<float>::INV_LN_2 = 1.f / Mathematic<float>::LN_2;
template<> const float Mathematic<float>::INV_LN_10 = 1.f / Mathematic<float>::LN_10;

template<> const double Mathematic<double>::PI = 4.0*atan(1.0);
template<> const double Mathematic<double>::HALF_PI = Mathematic<double>::PI * 0.5;
template<> const double Mathematic<double>::TWO_PI = Mathematic<double>::PI * 2.0;
template<> const double Mathematic<double>::DEGREE_TO_RAD = Mathematic<double>::PI / 180.0;
template<> const double Mathematic<double>::RAD_TO_DEGREE = 180.0 / Mathematic<double>::PI;
template<> const double Mathematic<double>::LN_2 = Mathematic<double>::Log(2.0);
template<> const double Mathematic<double>::LN_10 = Mathematic<double>::Log(10.0);
template<> const double Mathematic<double>::INV_LN_2 = 1.0 / Mathematic<double>::LN_2;
template<> const double Mathematic<double>::INV_LN_10 = 1.0 / Mathematic<double>::LN_10;


template <>
float Mathematic<float>::FastInvSqrt(float fValue)
{
	float fHalf = 0.5f*fValue;
	int i  = *(int*)&fValue;
	i = 0x5f3759df - (i >> 1);
	fValue = *(float*)&i;
	fValue = fValue*(1.5f - fHalf*fValue*fValue);
	return fValue;
}

template<>
double Mathematic<double>::FastInvSqrt(double dValue)
{
	double dHalf = 0.5*dValue;
	long long i  = *(long long*)&dValue;
	i = 0x5fe6ec85e7de30daLL - (i >> 1);
	dValue = *(double*)&i;
	dValue = dValue*(1.5 - dHalf*dValue*dValue);
	return dValue;
}