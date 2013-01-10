/**
 * @file K15_Mathematic.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/08
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

template<class Real>
Real Mathematic<Real>::DegToRad(Real fDegree)
{
	return fDegree * DEGREE_TO_RAD;
}

template<class Real>
Real Mathematic<Real>::RadToDeg(Real fRadiant)
{
	return fRadiant * RAD_TO_DEGREE;
}

template<class Real>
Real Mathematic<Real>::Abs( Real fValue )
{
	return (Real)abs((double)fValue);
}

template<class Real>
Real Mathematic<Real>::FAbs(Real fValue)
{
	return (Real)fabs((double)fValue);
}

template<class Real>
Real Mathematic<Real>::Sin(Real fAngle)
{
	return (Real)sin((double)fAngle);
}

template<class Real>
Real Mathematic<Real>::Cos(Real fAngle)
{
	return (Real)cos((double)fAngle);
}

template<class Real>
Real Mathematic<Real>::Tan(Real fAngle)
{
	return (Real)tan((double)fAngle);
}

template<class Real>
Real Mathematic<Real>::ASin(Real fSin)
{
	return (Real)asin((double)fSin);
}

template<class Real>
Real Mathematic<Real>::ACos(Real fCos)
{
	return (Real)acos((double)fCos);
}

template<class Real>
Real Mathematic<Real>::ATan(Real fTan)
{
	return (Real)atan((double)fTan);
}

template<class Real>
Real Mathematic<Real>::ATan2(Real fTan)
{
	return (Real)atan2((double)fTan);
}

template<class Real>
Real Mathematic<Real>::Sqrt(Real fValue)
{
	return (Real)sqrt((double)fValue);
}

template<class Real>
Real Mathematic<Real>::Pow(Real fBase,int iExponent)
{
	return (Real)pow((double)fBase,iExponent);
}

template<class Real>
Real Mathematic<Real>::InvSqrt(Real fValue)
{
	return (Real)(1.0/sqrt((double)fValue));
}

template<class Real>
Real Mathematic<Real>::Log(Real fValue)
{
	return (Real)log((double)fValue);
}

template<class Real>
Real Mathematic<Real>::Log2(Real fValue)
{
	return Mathematic<Real>::INV_LN_2 * (Real)log((double)fValue);
}

template<class Real>
Real Mathematic<Real>::Log10(Real fValue)
{
	return Mathematic<Real>::INV_LN_10 * (Real)log((double)fValue);
}

template<class Real>
Real Mathematic<Real>::FastSin1(Real fAngle)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = (Real)7.61e-03;
	fResult *= fASqr;
	fResult -= (Real)1.6605e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	fResult *= fAngle;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastSin2(Real fAngle)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = -(Real)2.39e-08;
	fResult *= fASqr;
	fResult += (Real)2.7526e-06;
	fResult *= fASqr;
	fResult -= (Real)1.98409e-04;
	fResult *= fASqr;
	fResult += (Real)8.3333315e-03;
	fResult *= fASqr;
	fResult -= (Real)1.666666664e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	fResult *= fAngle;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastCos1(Real fValue)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = (Real)3.705e-02;
	fResult *= fASqr;
	fResult -= (Real)4.967e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastCos2(Real fValue)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = -(Real)2.605e-07;
	fResult *= fASqr;
	fResult += (Real)2.47609e-05;
	fResult *= fASqr;
	fResult -= (Real)1.3888397e-03;
	fResult *= fASqr;
	fResult += (Real)4.16666418e-02;
	fResult *= fASqr;
	fResult -= (Real)4.999999963e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastTan1(Real fValue)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = (Real)2.033e-01;
	fResult *= fASqr;
	fResult += (Real)3.1755e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	fResult *= fAngle;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastTan2(Real fValue)
{
	Real fASqr = fAngle*fAngle;
	Real fResult = (Real)9.5168091e-03;
	fResult *= fASqr;
	fResult += (Real)2.900525e-03;
	fResult *= fASqr;
	fResult += (Real)2.45650893e-02;
	fResult *= fASqr;
	fResult += (Real)5.33740603e-02;
	fResult *= fASqr;
	fResult += (Real)1.333923995e-01;
	fResult *= fASqr;
	fResult += (Real)3.333314036e-01;
	fResult *= fASqr;
	fResult += (Real)1.0;
	fResult *= fAngle;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcSin1(Real fValue)
{
	Real fRoot = Math<Real>::Sqrt(((Real)1.0)-fValue);
	Real fResult = -(Real)0.0187293;
	fResult *= fValue;
	fResult += (Real)0.0742610;
	fResult *= fValue;
	fResult -= (Real)0.2121144;
	fResult *= fValue;
	fResult += (Real)1.5707288;
	fResult = HALF_PI - fRoot*fResult;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcSin2(Real fValue)
{
	Real fRoot = Math<Real>::Sqrt(FAbs(((Real)1.0)-fValue));
	Real fResult = -(Real)0.0012624911;
	fResult *= fValue;
	fResult += (Real)0.0066700901;
	fResult *= fValue;
	fResult -= (Real)0.0170881256;
	fResult *= fValue;
	fResult += (Real)0.0308918810;
	fResult *= fValue;
	fResult -= (Real)0.0501743046;
	fResult *= fValue;
	fResult += (Real)0.0889789874;
	fResult *= fValue;
	fResult -= (Real)0.2145988016;
	fResult *= fValue;
	fResult += (Real)1.5707963050;
	fResult = HALF_PI - fRoot*fResult;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcCos1(Real fValue)
{
	Real fRoot = Math<Real>::Sqrt(((Real)1.0)-fValue);
	Real fResult = -(Real)0.0187293;
	fResult *= fValue;
	fResult += (Real)0.0742610;
	fResult *= fValue;
	fResult -= (Real)0.2121144;
	fResult *= fValue;
	fResult += (Real)1.5707288;
	fResult *= fRoot;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcCos2(Real fValue)
{
	Real fRoot = Math<Real>::Sqrt(FAbs(((Real)1.0)-fValue));
	Real fResult = -(Real)0.0012624911;
	fResult *= fValue;
	fResult += (Real)0.0066700901;
	fResult *= fValue;
	fResult -= (Real)0.0170881256;
	fResult *= fValue;
	fResult += (Real)0.0308918810;
	fResult *= fValue;
	fResult -= (Real)0.0501743046;
	fResult *= fValue;
	fResult += (Real)0.0889789874;
	fResult *= fValue;
	fResult -= (Real)0.2145988016;
	fResult *= fValue;
	fResult += (Real)1.5707963050;
	fResult *= fRoot;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcTan1(Real fValue)
{
	Real fVSqr = fValue*fValue;
	Real fResult = (Real)0.0208351;
	fResult *= fVSqr;
	fResult -= (Real)0.085133;
	fResult *= fVSqr;
	fResult += (Real)0.180141;
	fResult *= fVSqr;
	fResult -= (Real)0.3302995;
	fResult *= fVSqr;
	fResult += (Real)0.999866;
	fResult *= fValue;
	return fResult;
}

template<class Real>
Real Mathematic<Real>::FastArcTan2(Real fValue)
{
	Real fVSqr = fValue*fValue;
	Real fResult = (Real)0.0028662257;
	fResult *= fVSqr;
	fResult -= (Real)0.0161657367;
	fResult *= fVSqr;
	fResult += (Real)0.0429096138;
	fResult *= fVSqr;
	fResult -= (Real)0.0752896400;
	fResult *= fVSqr;
	fResult += (Real)0.1065626393;
	fResult *= fVSqr;
	fResult -= (Real)0.1420889944;
	fResult *= fVSqr;
	fResult += (Real)0.1999355085;
	fResult *= fVSqr;
	fResult -= (Real)0.3333314528;
	fResult *= fVSqr;
	fResult += (Real)1.0;
	fResult *= fValue;
	return fResult;
}