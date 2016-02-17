#include "K15_Math.h"

#include "K15_Rectangle.cpp"
#include "K15_Vector2.cpp"
#include "K15_Vector3.cpp"
#include "K15_Vector4.cpp"
#include "K15_Matrix3.cpp"
#include "K15_Matrix4.cpp"
#include "K15_Quaternion.cpp"


/*********************************************************************************/
real32 K15_Lerp(real32 p_Start, real32 p_End, real32 p_Time)
{
	return (1.0f - p_Time) * p_Start + p_Time * p_End;
}
/*********************************************************************************/
K15_Vector2 K15_Lerp(K15_Vector2 p_Start, K15_Vector2 p_End, real32 p_Time)
{
	return (1.0f - p_Time) * p_Start + p_Time * p_End;
}
/*********************************************************************************/
K15_Vector3 K15_Lerp(K15_Vector3 p_Start, K15_Vector3 p_End, real32 p_Time)
{
	return (1.0f - p_Time) * p_Start + p_Time * p_End;
}
/*********************************************************************************/
K15_Vector3 K15_LerpColor(K15_Vector3 p_Start, K15_Vector3 p_End, real32 p_T)
{
// 	K15_Vector3 hsvStartColor = K15_ConvertToHSV(p_Start);
// 	K15_Vector3 hsvEndColor = K15_ConvertToHSV(p_End);
// 
// 	K15_Vector3 lerpedHSVColor = K15_Lerp(hsvStartColor, hsvEndColor, p_T);
// 
// 	return K15_ConvertToRGB(lerpedHSVColor);

	return K15_Lerp(p_Start, p_End, p_T);
}
/*********************************************************************************/
K15_Vector3 K15_ConvertToHSV(const K15_Vector3& p_RGBColor)
{
	//http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	K15_Vector3 hsv = {};
	float minimum = 0.f, maximum = 0.f, delta = 0.f;

	minimum = K15_MIN(p_RGBColor.x, p_RGBColor.y);
	minimum = K15_MIN(minimum, p_RGBColor.z);

	maximum = K15_MAX(p_RGBColor.x, p_RGBColor.y);
	maximum = K15_MAX(maximum, p_RGBColor.z);

	hsv.z = maximum;
	delta = maximum - minimum;

	if (delta < 0.00001f)
	{
		hsv.x = 0.f;
		hsv.y = 0.f;
	}

	hsv.y = K15_SafeDivide(delta, maximum);

	if (p_RGBColor.x >= maximum)
	{
		hsv.x = K15_SafeDivide(p_RGBColor.y - p_RGBColor.z, delta);
	}
	else if (p_RGBColor.y >= maximum)
	{
		hsv.x = 2.f + K15_SafeDivide(p_RGBColor.z - p_RGBColor.x, delta);
	}
	else
	{
		hsv.x = 4.f + K15_SafeDivide(p_RGBColor.x - p_RGBColor.y, delta);
	}

	hsv.x *= 60.f;

	if (hsv.x < 0.f)
	{
		hsv.x += 360.f;
	}

	return hsv;
}
/*********************************************************************************/
K15_Vector3 K15_ConvertToRGB(const K15_Vector3& p_HSVColor)
{
	//http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	float hh, p, q, t, ff;
	int32 i;
	K15_Vector3	rgb;

	if (p_HSVColor.y <= 0.f) 
	{
		rgb.x = p_HSVColor.z;
		rgb.y = p_HSVColor.z;
		rgb.z = p_HSVColor.z;
		return rgb;
	}

	hh = p_HSVColor.x;
	if (hh >= 360.f) hh = 0.f;
	hh /= 60.f;
	i = (int32)hh;
	ff = hh - i;
	p = p_HSVColor.z * (1.f - p_HSVColor.y);
	q = p_HSVColor.z * (1.f - (p_HSVColor.y * ff));
	t = p_HSVColor.z * (1.f - (p_HSVColor.y * (1.f - ff)));

	switch (i) {
	case 0:
		rgb.x = p_HSVColor.z;
		rgb.y = t;
		rgb.z = p;
		break;
	case 1:
		rgb.x = q;
		rgb.y = p_HSVColor.z;
		rgb.z = p;
		break;
	case 2:
		rgb.x = p;
		rgb.y = p_HSVColor.z;
		rgb.z = t;
		break;

	case 3:
		rgb.x = p;
		rgb.y = q;
		rgb.z = p_HSVColor.z;
		break;
	case 4:
		rgb.x = t;
		rgb.y = p;
		rgb.z = p_HSVColor.z;
		break;
	case 5:
	default:
		rgb.x = p_HSVColor.z;
		rgb.y = p;
		rgb.z = q;
		break;
	}
	return rgb;
}
/*********************************************************************************/
real32 K15_Log2(real32 p_Number)
{
	return (real32)(floor(log10(p_Number) / log10(2.0)));
}
/*********************************************************************************/
real32 K15_SafeDivide(real32 p_Dividend, real32 p_Divisor)
{
	if (p_Divisor < K15_EPSILON)
	{
		p_Divisor = 1.0;
	}

	return p_Dividend / p_Divisor;
}
/*********************************************************************************/
real32 K15_FastSqrt(real32 p_Number)
{
	//http://en.wikipedia.org/wiki/Fast_inverse_square_root
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = p_Number * 0.5F;
	y  = p_Number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

	return y;
}
/*********************************************************************************/
real32 K15_LowPrecisionSin(real32 p_Number)
{
	//http://lab.polygonal.de/?p=205
	real32 sine = 0.0;

	//always wrap input angle to -PI..PI
	if (p_Number < -3.14159265)
	{
		p_Number += 6.28318531;
	}
	else if (p_Number > 3.14159265)
	{
		p_Number -= 6.28318531;
	}

	if (p_Number < 0.0)
	{
		sine = 1.27323954 * p_Number + .405284735 * p_Number * p_Number;
	}
	else
	{
		sine = 1.27323954 * p_Number - 0.405284735 * p_Number * p_Number;
	}

	return sine;
}
/*********************************************************************************/
real32 K15_LowPrecisionCos(real32 p_Number)
{
	//http://lab.polygonal.de/?p=205

	real32 cosine = 0.0;

	p_Number += 1.57079632;

	if (p_Number > 3.14159265)
	{
		p_Number -= 6.28318531;
	}

	if (p_Number < 0.0)
	{
		cosine = 1.27323954 * p_Number + 0.405284735 * p_Number * p_Number;
	}
	else
	{
		cosine = 1.27323954 * p_Number - 0.405284735 * p_Number * p_Number;
	}

	return cosine;
}
/*********************************************************************************/
real32 K15_HighPrecisionSin(real32 p_Number)
{
	real32 sine = 0.0;

	//http://lab.polygonal.de/?p=205
	//always wrap input angle to -PI..PI
	if (p_Number < -3.14159265)
		p_Number += 6.28318531;
	else
		if (p_Number >  3.14159265)
			p_Number -= 6.28318531;

	//compute sine
	if (p_Number < 0)
	{
		sine = 1.27323954 * p_Number + .405284735 * p_Number * p_Number;

		if (sine < 0)
			sine = .225 * (sine *-sine - sine) + sine;
		else
			sine = .225 * (sine * sine - sine) + sine;
	}
	else
	{
		sine = 1.27323954 * p_Number - 0.405284735 * p_Number * p_Number;

		if (sine < 0)
			sine = .225 * (sine *-sine - sine) + sine;
		else
			sine = .225 * (sine * sine - sine) + sine;
	}

	return sine;
}
/*********************************************************************************/
real32 K15_HighPrecisionCos(real32 p_Number)
{
	real32 cosine = 0.0;

	//http://lab.polygonal.de/?p=205
	//compute cosine: sin(x + PI/2) = cos(x)
	p_Number += 1.57079632;
	if (p_Number >  3.14159265)
		p_Number -= 6.28318531;

	if (p_Number < 0)
	{
		cosine = 1.27323954 * p_Number + 0.405284735 * p_Number * p_Number;

		if (cosine < 0)
			cosine = .225 * (cosine *-cosine - cosine) + cosine;
		else
			cosine = .225 * (cosine * cosine - cosine) + cosine;
	}
	else
	{
		cosine = 1.27323954 * p_Number - 0.405284735 * p_Number * p_Number;

		if (cosine < 0)
			cosine = .225 * (cosine *-cosine - cosine) + cosine;
		else
			cosine = .225 * (cosine * cosine - cosine) + cosine;
	}

	return cosine;
}
/*********************************************************************************/
uint32 K15_GetNextPowerOfTwoSize(uint32 p_NonPowerOfTwoNumber)
{
	uint32 powerOfTwoNumber = 2;

	while (powerOfTwoNumber < p_NonPowerOfTwoNumber)
	{
		powerOfTwoNumber *= 2;
	}

	return powerOfTwoNumber;
}
/*********************************************************************************/
bool8 K15_Collision2DBoxPoint(float p_BoxLeft, float p_BoxTop, float p_BoxRight, float p_BoxBottom, float p_PointX, float p_PointY)
{
	return (p_PointX > p_BoxLeft && p_PointX < p_BoxRight && p_PointY > p_BoxTop && p_PointY < p_BoxBottom);
}
/*********************************************************************************/