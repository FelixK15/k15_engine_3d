/**
 * @file K15_Vector3.cpp
 * @author  Tobias Funke <tobyfunke@web.de>
 * @version 1.0
 * @date 2012/10/19
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
#include "K15_Vector3.h"
using namespace K15_Math;


Vector3<float> Vector3<float>::Up = Vector3(0.f,1.f,0.f);
Vector3<float> Vector3<float>::Right = Vector3(1.f,0.f,0.f);
Vector3<float> Vector3<float>::Forward = Vector3(0.f,0.f,1.f);
Vector3<double> Vector3<double>::Up = Vector3(0,1,0);
Vector3<double> Vector3<double>::Right = Vector3(1,0,0);
Vector3<double> Vector3<double>::Forward = Vector3(0,0,1);
