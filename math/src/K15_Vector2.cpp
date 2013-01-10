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

#include "K15_Vector2.h"
#include "math.h"

using namespace K15_Math;

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2( float vec[2] )
{
	x = vec[0];
	y = vec[1];
}

Vector2::Vector2( float x,float y )
{
	vec[0] = x;
	vec[1] = y;
}

Vector2::Vector2( const Vector2 &vec2 )
{
	x = vec2.x;
	y = vec2.y;
}

Vector2::~Vector2()
{

}

void Vector2::Normalize()
{
	float tmp = Magnitude();
	x /= tmp;
	y /= tmp;
}

float Vector2::Magnitude() const
{
	return sqrt((x*x+y*y));
}

float Vector2::Dot( const Vector2 &vec ) const
{
	return (x * vec.x + y * vec.y);
}

Vector2 &Vector2::operator*( float scale )
{
	x *= scale;
	y *= scale;
	return *this;
}


Vector2 &Vector2::operator+( const Vector2 &vec )
{
	return _Add(vec);
}

Vector2 &Vector2::operator-( const Vector2 &vec )
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2 &Vector2::operator+=( const Vector2 &vec )
{
	return _Add(vec);
}

Vector2 &Vector2::operator-=( const Vector2 &vec )
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

bool Vector2::operator>( const Vector2 &vec )
{
	if(Magnitude()>vec.Magnitude()){
		return true;
	}
	return false;
}

bool Vector2::operator<=( const Vector2 &vec )
{
	if(Magnitude()<=vec.Magnitude()){
		return true;
	}
	return false;
}

bool Vector2::operator==( const Vector2 &vec )
{
	if(x==vec.x && y==vec.y){
		return true;
	}
	return false;
}

bool Vector2::operator!=( const Vector2 &vec )
{
	if(x==vec.x && y==vec.y){
		return false;
	}
	return true;
}

Vector2 &Vector2::_Add( const Vector2 &vec )
{
	x += vec.x;
	y += vec.y;
	return *this;
}
