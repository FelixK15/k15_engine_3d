/**
 * @file K15_Vector2.cpp
 * @author	Tobias Funke <t.funke@k15games.de>
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
 */

#include "K15_Math_PrecompiledHeader.h"
#include "K15_Vector2.h"

namespace K15_Engine { namespace Math {
	/*********************************************************************************/
	Vector2::Vector2()
		: x(0.0f),
		  y(0.0f)
	{
		
	}
	/*********************************************************************************/
	Vector2::Vector2(float p_Values[2])
		: x(p_Values[0]),
		  y(p_Values[1])
	{
		
	}
	/*********************************************************************************/
	Vector2::Vector2(float x,float y)
		: x(x),
		  y(y)
	{
	
	}
	/*********************************************************************************/
	Vector2::Vector2(const Vector2& p_Other)
		: x(p_Other.x),
		  y(p_Other.y)
	{
	
	}
	/*********************************************************************************/
	Vector2::~Vector2()
	{

	}
	/*********************************************************************************/
	void Vector2::normalize()
	{
		float tmp = magnitude();
		x /= tmp;
		y /= tmp;
	}
	/*********************************************************************************/
	float Vector2::magnitude() const
	{
		return ::sqrt(x*x+y*y);
	}
	/*********************************************************************************/
	float Vector2::dot(const Vector2& p_Other) const
	{
		return (x * p_Other.x + y * p_Other.y);
	}
	/*********************************************************************************/
	Vector2& Vector2::operator*(float p_Scale)
	{
		x *= p_Scale;
		y *= p_Scale;
		return *this;
	}
	/*********************************************************************************/
	Vector2& Vector2::operator+(const Vector2& p_Other)
	{
		x += p_Other.x;
		y += p_Other.y;
		return *this;
	}

	Vector2& Vector2::operator-(const Vector2& p_Other)
	{
		x -= p_Other.x;
		y -= p_Other.y;
		return *this;
	}
	/*********************************************************************************/
	Vector2& Vector2::operator+=( const Vector2& p_Other )
	{
		x += p_Other.x;
		y += p_Other.y;
		return *this;
	}
	/*********************************************************************************/
	Vector2& Vector2::operator-=( const Vector2& p_Other )
	{
		x -= p_Other.x;
		y -= p_Other.y;
		return *this;
	}
	/*********************************************************************************/
	bool Vector2::operator>( const Vector2& p_Other )
	{
		if(magnitude()>p_Other.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator<=( const Vector2& p_Other )
	{
		if(magnitude()<=p_Other.magnitude()){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator==( const Vector2& p_Other )
	{
		if(x==p_Other.x && y==p_Other.y){
			return true;
		}
		return false;
	}
	/*********************************************************************************/
	bool Vector2::operator!=( const Vector2& p_Other )
	{
		if(x==p_Other.x && y==p_Other.y){
			return false;
		}
		return true;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Math namespace